#include "Database.h"
#include <thread>
#include <cmath>

Database::Database() {
  filename_idx = 0;
}

void Database::write(Key k, Value v) {
  Pair p = Pair(k, v);
  Buffer* b = m.get_buffer();
  b->append(p);
  if (b->is_full()) {
    m.roll_buffer();
    std::thread merge_thread(&Database::merge_buffer, this, b);
    merge_thread.join();
  }
  return;
}

std::string Database::get_filename() {
  std::string s = std::to_string(filename_idx);
  filename_idx = filename_idx + 1;
  return s;
}

int calculate_hashes(int bits, int entries) {
  double hashes = (log(2) * bits) / entries;
  return std::round(hashes);
}

void Database::merge_buffer(Buffer* b) {
  b->sort();
  std::string filename = get_filename();
  b->save_to(filename);
  if (m.get_levels_number() == 0) {
    m.add_level();
  }
  FencePointer fp(b->read(0).get_key(), b->read_last().get_key());
  int hashes = calculate_hashes(bloom_filter_bits, file_length);
  RunInfo* run_info = m.get_level(0)->add_run(hashes);
  for (int i=0; i<file_length; i++) {
    run_info->add_to_filter(b->read(i).get_key());
  }
  run_info->add_file(filename, 0, fp);
  m.remove_backup();
  if (m.get_level(0)->is_full()) {
    merge(0);
  }
  return;
}

void Database::merge(int level_number) {
  int idxs [runs_per_level];
  int file_idxs [runs_per_level];
  Buffer* buffers [runs_per_level]; // initializes them all with the default constructor
  for (int i=0; i<runs_per_level; i++) {
    idxs[i] = 0;
    std::string filename = m.get_level(level_number)->get_run(i)->get_file(0)->get_filename();
    buffers[i] = new Buffer();
    buffers[i]->load_from(filename);
    file_idxs[i] = 1;
  }
  Buffer* b = new Buffer();

  int hashes = calculate_hashes(bloom_filter_bits, file_length * (int)std::pow(runs_per_level, level_number+1));
  if (m.get_levels_number() == level_number+1) {
    m.add_level();
  }
  RunInfo* run_info = m.get_level(level_number+1)->add_run(hashes);
  while (true) {
    int small_idx = runs_per_level-1;
    while (small_idx >= 0 && buffers[small_idx]->is_null_buffer()) {
      small_idx = small_idx - 1;
    }
    if (small_idx == -1) {
      break;
    }
    Pair smallest_key_pair = buffers[small_idx]->read(idxs[small_idx]);
    for (int i=small_idx-1; i>=0; i--) {
      if (buffers[i]->is_null_buffer()) {
        continue;
      }
      Pair p = buffers[i]->read(idxs[i]);
      if (p.less_than(smallest_key_pair)) {
        smallest_key_pair = p;
        small_idx = i;
      }
      else if (p.key_equals(smallest_key_pair)) {
        idxs[i] = idxs[i] + 1;
        if (idxs[i] == file_length) {
          if (file_idxs[i] == m.get_level(level_number)->get_run(i)->get_files_number()) {
            delete buffers[i];
            buffers[i] = new Buffer(true);
          }
          else {
            std::string filename = m.get_level(level_number)->get_run(i)->get_file(file_idxs[i])->get_filename();
            buffers[i]->load_from(filename);
            file_idxs[i] = file_idxs[i] + 1;
            idxs[i] = 0;
          }
        }
      }
    }
    idxs[small_idx] = idxs[small_idx] + 1;
    if (idxs[small_idx] == file_length) {
      if (file_idxs[small_idx] == m.get_level(level_number)->get_run(small_idx)->get_files_number()) {
        delete buffers[small_idx];
        buffers[small_idx] = new Buffer(true);
      }
      else {
        std::string filename = m.get_level(level_number)->get_run(small_idx)->get_file(file_idxs[small_idx])->get_filename();
        buffers[small_idx]->load_from(filename);
        file_idxs[small_idx] = file_idxs[small_idx] + 1;
        idxs[small_idx] = 0;
      }
    }
    Pair p = smallest_key_pair;
    b->append(p);
    run_info->add_to_filter(p.get_key());
    if (b->is_full()) {
      string filename = get_filename();
      b->save_to(filename);
      FencePointer fp(b->read(0).get_key(), b->read_last().get_key());
      run_info->add_file(filename, level_number+1, fp);
      delete b;
      b = new Buffer();
    }
  }
  for (int i=0; i<runs_per_level; i++) {
    delete buffers[i];
  }
  delete b;

  // update manifest
  m.get_level(level_number)->pop_runs(runs_per_level);
  if (m.get_level(level_number+1)->is_full()) {
    merge(level_number+1);
  }
  return;
}

Value Database::read(Key k) {
  Value v(0);
  if (m.get_buffer()->unordered_find(k, &v)) {
    return v;
  }
  Buffer* b_backup = m.get_buffer_backup();
  if (!b_backup->is_null_buffer()) {
    if (b_backup->unordered_find(k, &v)) {
      return v;
    }
  }

  Buffer* b = new Buffer();
  for (int i=0; i<m.get_levels_number(); i++) {
    LevelInfo* level_info = m.get_level(i);
    for (int j=level_info->get_runs_number()-1; j>=0; j--) {
      RunInfo* run_info = level_info->get_run(j);
      if (!run_info->possibly_contains(k)) {
        continue;
      }
      for (int t=0; t<run_info->get_files_number(); t++) {
        FileInfo* file_info = run_info->get_file(t);
        if (!file_info->contains_in_range(k)) {
          continue;
        }
        b->load_from(file_info->get_filename());
        if (b->ordered_find(k, &v)) {
          delete b;
          return v;
        }
      }
    }
  }
  delete b;
  return Value(0, true);
}

void Database::print_all() {
  m.get_buffer()->print();
  m.get_buffer_backup()->print();

  Buffer* b = new Buffer();
  for (int i=0; i<m.get_levels_number(); i++) {
    LevelInfo* level_info = m.get_level(i);
    for (int j=level_info->get_runs_number()-1; j>=0; j--) {
      RunInfo* run_info = level_info->get_run(j);
      for (int t=0; t<run_info->get_files_number(); t++) {
        FileInfo* file_info = run_info->get_file(t);
        b->load_from(file_info->get_filename());
        b->print();
      }
    }
  }
  delete b;
  return;
}

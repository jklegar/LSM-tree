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
    Buffer* b_new = new Buffer();
    delete m.get_buffer_backup();
    m.set_buffer_backup(b);
    m.set_buffer(b_new);
    std::thread merge_thread(&Database::merge_buffer, this, b);
    merge_thread.detach();
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
    LevelInfo* level_info = new LevelInfo();
    m.set_level(level_info, 0);
    m.increment_levels_number();
  }
  FencePointer fp(b->read(0).get_key(), b->read_last().get_key());
  FileInfo* file_info = new FileInfo(filename, 0, fp);
  BloomFilter* bf = new BloomFilter(bloom_filter_bits, calculate_hashes(bloom_filter_bits, file_length));
  for (int i=0; i<file_length; i++) {
    bf->add(b->read(i).get_key());
  }
  RunInfo* run_info = new RunInfo(bf);
  run_info->add_file(file_info);
  m.get_level(0)->add_run(run_info);
  Buffer* b_null = new Buffer(true);
  delete m.get_buffer_backup();
  m.set_buffer_backup(b_null);
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

  BloomFilter* bf = new BloomFilter(bloom_filter_bits, calculate_hashes(bloom_filter_bits, file_length * (int)std::pow(runs_per_level, level_number+1)));
  RunInfo* run_info = new RunInfo(bf);
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
      Pair p = buffers[i]->read(idxs[i]);
      if (p.less_than(smallest_key_pair)) {
        smallest_key_pair = p;
        small_idx = i;
      }
      if (p.key_equals(smallest_key_pair)) {
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
    bf->add(p.get_key());
    if (b->is_full()) {
      string filename = get_filename();
      b->save_to(filename);
      FencePointer fp(b->read(0).get_key(), b->read_last().get_key());
      FileInfo* file_info = new FileInfo(filename, level_number+1, fp);
      run_info->add_file(file_info);
      delete b;
      b = new Buffer();
    }
  }

  // update manifest
  if (m.get_levels_number() == level_number) {
    LevelInfo* level_info = new LevelInfo();
    m.set_level(level_info, level_number+1);
    m.increment_levels_number();
  }
  m.get_level(level_number+1)->add_run(run_info);
  RunInfo** runs_to_delete = m.get_level(level_number)->pop_runs(runs_per_level);
  for (int i=0; i<runs_per_level; i++) {
    RunInfo* run_info = runs_to_delete[i];
    run_info->delete_all_files();
    delete run_info;
  }
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
          return v;
        }
      }
    }
  }
  return Value(0, true);
}

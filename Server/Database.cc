#include "Database.h"
#include <cmath>
#include <set>
#include <thread>
#include <iostream>

Database::Database() {
  filename_idx = 0;
}

void Database::write(Key k, Value v) {
  Pair p = Pair(k, v);
  m.mbuffer.lock();
  Buffer* b = m.get_buffer();
  b->append(p);
  if (b->is_full()) {
    m.mbuffer_backup.lock();
    m.roll_buffer();
    m.mbuffer.unlock();
    std::thread merge_thread(&Database::merge_buffer, this, b);
    merge_thread.detach();
  }
  else {
    m.mbuffer.unlock();
  }
  return;
}

void Database::delete_key(Key k) {
  write(k, Value(0, true));
  return;
}

std::string Database::get_filename() {
  mfilename_idx.lock();
  std::string s = std::to_string(filename_idx);
  filename_idx = filename_idx + 1;
  mfilename_idx.unlock();
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
  madd_level.lock();
  if (m.get_levels_number() == 0) {
    m.add_level();
  }
  madd_level.unlock();
  FencePointer fp(b->read(0).get_key(), b->read_last().get_key());
  int hashes = calculate_hashes(bloom_filter_bits, file_length);
  int merge_num = -1; // -1 means will not merge
  m.mlevels[0].lock();
  RunInfo* run_info = m.get_level(0)->add_run(hashes);
  if (m.get_level(0)->is_full()) {
    merges_started[0]++;
    merge_num = merges_started[0] - 1;
    m.mlevels[0].unlock();
  }
  else {
    m.mlevels[0].unlock();
  }
  for (int i=0; i<b->len(); i++) {
    run_info->add_to_filter(b->read(i).get_key());
  }
  run_info->add_file(filename, 0, fp);
  m.remove_backup();
  m.mbuffer_backup.unlock();
  if (merge_num != -1) {
    std::thread merge_thread(&Database::merge, this, 0, merge_num);
    merge_thread.detach();
  }
  return;
}

void Database::merge(int level_number, int merge_num) {
  int idxs [runs_per_level];
  int file_idxs [runs_per_level];
  Buffer* buffers [runs_per_level]; // initializes them all with the default constructor
  for (int i=0; i<runs_per_level; i++) {
    idxs[i] = 0;
    m.mlevels[level_number].lock();
    std::string filename = m.get_level(level_number)->get_run((merge_num-merges_finished[level_number])*runs_per_level+i)->get_file(0)->get_filename();
    buffers[i] = new Buffer();
    buffers[i]->load_from(filename);
    m.mlevels[level_number].unlock();
    file_idxs[i] = 1;
  }
  Buffer* b = new Buffer();

  int hashes = calculate_hashes(bloom_filter_bits, file_length * (int)std::pow(runs_per_level, level_number+1));
  madd_level.lock();
  if (m.get_levels_number() == level_number+1) {
    m.add_level();
  }
  madd_level.unlock();
  int new_merge_num = -1; // -1 means will not merge
  m.mlevels[level_number+1].lock();
  RunInfo* run_info = m.get_level(level_number+1)->add_run(hashes);
  if (m.get_level(level_number+1)->is_full()) {
    merges_started[level_number+1]++;
    new_merge_num = merges_started[level_number+1] - 1;
    m.mlevels[level_number+1].unlock();
  }
  else {
    m.mlevels[level_number+1].unlock();
  }
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
        if (idxs[i] == buffers[i]->len()) {
          m.mlevels[level_number].lock();
          if (file_idxs[i] == m.get_level(level_number)->get_run((merge_num-merges_finished[level_number])*runs_per_level+i)->get_files_number()) {
            m.mlevels[level_number].unlock();
            delete buffers[i];
            buffers[i] = new Buffer(true);
          }
          else {
            std::string filename = m.get_level(level_number)->get_run((merge_num-merges_finished[level_number])*runs_per_level+i)->get_file(file_idxs[i])->get_filename();
            buffers[i]->load_from(filename);
            m.mlevels[level_number].unlock();
            file_idxs[i] = file_idxs[i] + 1;
            idxs[i] = 0;
          }
        }
      }
    }
    idxs[small_idx] = idxs[small_idx] + 1;
    if (idxs[small_idx] == buffers[small_idx]->len()) {
      m.mlevels[level_number].lock();
      if (file_idxs[small_idx] == m.get_level(level_number)->get_run((merge_num-merges_finished[level_number])*runs_per_level+small_idx)->get_files_number()) {
        m.mlevels[level_number].unlock();
        delete buffers[small_idx];
        buffers[small_idx] = new Buffer(true);
      }
      else {
        std::string filename = m.get_level(level_number)->get_run((merge_num-merges_finished[level_number])*runs_per_level+small_idx)->get_file(file_idxs[small_idx])->get_filename();
        buffers[small_idx]->load_from(filename);
        m.mlevels[level_number].unlock();
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
  if (!b->is_empty()) {
    string filename = get_filename();
    b->save_to(filename);
    FencePointer fp(b->read(0).get_key(), b->read_last().get_key());
    run_info->add_file(filename, level_number+1, fp);
  }
  for (int i=0; i<runs_per_level; i++) {
    delete buffers[i];
  }
  delete b;

  // update manifest
  m.mlevels[level_number].lock();
  m.get_level(level_number)->pop_runs(runs_per_level);
  merges_finished[level_number]++;
  m.mlevels[level_number].unlock();
  if (new_merge_num != -1) {
    std::thread merge_thread(&Database::merge, this, level_number+1, new_merge_num);
    merge_thread.detach();
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
    m.mlevels[i].lock();
    LevelInfo* level_info = m.get_level(i);
    for (int j = level_info->get_runs_number()-1; j>=0; j--) {
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
          m.mlevels[i].unlock();
          return v;
        }
      }
    }
    m.mlevels[i].unlock();
  }
  delete b;
  return Value(0, true);
}

map<Key, Value> Database::read_range(Key k_1, Key k_2) {
  map<Key, Value> results;
  m.get_buffer()->unordered_find_range(k_1, k_2, &results);
  Buffer* b_backup = m.get_buffer_backup();
  if (!b_backup->is_null_buffer()) {
    b_backup->unordered_find_range(k_1, k_2, &results);
  }

  Buffer* b = new Buffer();
  for (int i=0; i<m.get_levels_number(); i++) {
    m.mlevels[i].lock();
    LevelInfo* level_info = m.get_level(i);
    for (int j = level_info->get_runs_number()-1; j>=0; j--) {
      RunInfo* run_info = level_info->get_run(j);
      for (int t=0; t<run_info->get_files_number(); t++) {
        FileInfo* file_info = run_info->get_file(t);
        if (!file_info->is_between(k_1, k_2)) {
          continue;
        }
        b->load_from(file_info->get_filename());
        if (file_info->contains_in_range(k_1) && file_info->contains_in_range(k_2)) {
          b->ordered_find_within(k_1, k_2, &results);
        } else if (file_info->contains_in_range(k_1)) {
          b->ordered_find_upper(k_1, &results);
        } else if (file_info->contains_in_range(k_2)) {
          b->ordered_find_lower(k_2, &results);
        } else {
          b->ordered_find_all(&results);
        }
      }
    }
    m.mlevels[i].unlock();
  }
  delete b;
  return results;
}

void Database::print_stats() {
  std::set<Key> existing_keys;
  std::set<Key> deleted_keys;
  int level_counts [max_levels] = {};

  m.get_buffer()->print(-1);
  m.get_buffer_backup()->print(-1);
  m.get_buffer()->add_keys(&existing_keys, &deleted_keys);
  m.get_buffer_backup()->add_keys(&existing_keys, &deleted_keys);

  Buffer* b = new Buffer();
  for (int i=0; i<m.get_levels_number(); i++) {
    std::cout << "Level " << i << std::endl;
    m.mlevels[i].lock();
    LevelInfo* level_info = m.get_level(i);
    for (int j = level_info->get_runs_number()-1; j>=0; j--) {
      RunInfo* run_info = level_info->get_run(j);
      for (int t=0; t<run_info->get_files_number(); t++) {
        FileInfo* file_info = run_info->get_file(t);
        b->load_from(file_info->get_filename());
        b->print(i);
        b->add_keys(&existing_keys, &deleted_keys);
        level_counts[i] += b->len();
      }
    }
    m.mlevels[i].unlock();
  }
  delete b;
  std::cout << "Buffer count: " << m.get_buffer()->len()+m.get_buffer_backup()->len() << std::endl;
  for (int i=0; i<max_levels; i++) {
    std::cout << "Level " << i << " count: " << level_counts[i] << std::endl;
  }
  std::cout << "Logical Pairs: " << existing_keys.size() << std::endl;
  return;
}

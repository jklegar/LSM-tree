#include "Buffer.h"
#include <fstream>

Buffer::Buffer(bool is_n) {
  idx = 0;
  is_null = is_n;
}

bool Buffer::is_full() {
  return (idx == file_length);
}

bool Buffer::is_empty() {
  return (idx == 0);
}

int Buffer::len() {
  return idx;
}

Pair Buffer::read(int i) {
  return b[i];
}

Pair Buffer::read_last() {
  return b[idx-1];
}

void Buffer::append(Pair p) {
  b[idx] = p;
  idx = idx + 1;
  return;
}

void Buffer::save_to(std::string filename) {
  ofstream outfile;
  outfile.open(data_directory + filename);
  outfile.write(reinterpret_cast<char*>(&idx), sizeof(idx));
  outfile.write(reinterpret_cast<char*>(&b), sizeof(Pair)*idx);
  outfile.close();
  return;
}

void Buffer::load_from(std::string filename) {
  ifstream infile;
  infile.open(data_directory + filename);
  infile.read(reinterpret_cast<char*>(&idx), sizeof(idx));
  infile.read(reinterpret_cast<char*>(&b), sizeof(Pair)*idx);
  infile.close();
  return;
}

bool less_than(Pair p1, Pair p2) {
  return p1.less_than(p2);
}

bool key_equals(Pair p1, Pair p2) {
  return p1.key_equals(p2);
}

void Buffer::sort() {
  std::stable_sort(b, b+file_length, less_than);
  std::reverse(b, b+file_length);
  auto uniq_end = std::unique(b, b+file_length, key_equals);
  idx = std::distance(b, uniq_end);
  std::reverse(b, b+idx);
  return;
}

bool Buffer::is_null_buffer() {
  return is_null;
}

bool Buffer::unordered_find(Key k, Value* v) {
  for (int i=idx-1; i>=0; i--) {
    Pair p = read(i);
    if (p.get_key().equals(k)) {
      *v = p.get_value();
      return true;
    }
  }
  return false;
}

bool Buffer::ordered_find_bounds(Key k, Value* v, int low, int high) {
  int mid = (int)((low + high)/2);
  Pair p = read(mid);
  if (p.get_key().equals(k)) {
    *v = p.get_value();
    return true;
  }
  if (mid == high) {
    return false;
  }
  if (p.get_key().less_than(k)) {
    return ordered_find_bounds(k, v, mid+1, high);
  }
  else {
    return ordered_find_bounds(k, v, low, mid-1);
  }
}

bool Buffer::ordered_find(Key k, Value* v) {
  return ordered_find_bounds(k, v, 0, idx-1);
}

void Buffer::print(int level) {
  for (int i=0; i<idx; i++) {
    b[i].print(level);
  }
}

void Buffer::add_keys(std::set<Key>* existing_keys, std::set<Key>* deleted_keys) {
  for (int i=0; i<idx; i++) {
    auto k = read(i).get_key();
    if (existing_keys->find(k) != existing_keys->end() || deleted_keys->find(k) != deleted_keys->end()) {
      continue;
    }
    bool v_is_del = read(i).get_value().get_is_delete();
    if (v_is_del) {
      deleted_keys->emplace(k);
    } else {
      existing_keys->emplace(k);
    }
  }
  return;
}

#include "Buffer.h"
#include <fstream>

Buffer::Buffer(bool is_n) {
  idx = 0;
  is_null = is_n;
}

bool Buffer::is_full() {
  return (idx == file_length);
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
  outfile.write(reinterpret_cast<char*>(&b), sizeof(b));
  outfile.close();
  return;
}

void Buffer::load_from(std::string filename) {
  ifstream infile;
  infile.open(data_directory + filename);
  infile.read(reinterpret_cast<char*>(&b), sizeof(b));
  infile.close();
  idx = file_length;
  return;
}

bool less_than(Pair p1, Pair p2) {
  return p1.less_than(p2);
}

void Buffer::sort() {
  std::sort(b, b+file_length, less_than);
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

void Buffer::print() {
  for (int i=0; i<idx; i++) {
    b[i].print();
  }
}

#include "RunInfo.h"

RunInfo::RunInfo(int hashes) {
  files_number = 0;
  bloom_filter = new BloomFilter(bloom_filter_bits, hashes);
}

void RunInfo::add_file(std::string filename, int level_no, FencePointer fp) {
  FileInfo* f = new FileInfo(filename, level_no, fp);
  files.push_back(f);
  files_number = files_number + 1;
  return;
}

FileInfo* RunInfo::get_file(int idx) {
  return files.at(idx);
}

bool RunInfo::possibly_contains(Key k) {
  return bloom_filter->possibly_contains(k);
}

void RunInfo::add_to_filter(Key k) {
  bloom_filter->add(k);
  return;
}

int RunInfo::get_files_number() {
  return files_number;
}

void RunInfo::delete_all_files() {
  for (int i=0; i<files_number; i++) {
    get_file(i)->delete_file();
  }
  return;
}

RunInfo::~RunInfo() {
  delete bloom_filter;
  for (int i=0; i<get_files_number(); i++) {
    delete get_file(i);
  }
}

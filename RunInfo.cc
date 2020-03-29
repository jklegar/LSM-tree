#include "RunInfo.h"

RunInfo::RunInfo(BloomFilter bf)
  : bloom_filter(bf) {
  files_number = 0;
}

void RunInfo::add_file(FileInfo *f) {
  files[files_number] = *f;
  files_number = files_number + 1;
  return;
}

FileInfo RunInfo::get_file(int idx) {
  return files[idx];
}

bool RunInfo::possibly_contains(Key k) {
  return bloom_filter.possibly_contains(k);
}

int RunInfo::get_files_number() {
  return files_number;
}

void RunInfo::delete_all_files() {
  for (int i=0; i<files_number; i++) {
    get_file(i).delete_file();
  }
  return;
}

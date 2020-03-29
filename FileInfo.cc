#include "FileInfo.h"
#include <cstdio>
using namespace std;

FileInfo::FileInfo(std::string f, int level_no, FencePointer fp)
  : filename(f)
  , level_number(level_no)
  , fence_pointer(fp) {
}

string FileInfo::get_filename() {
  return filename;
}

int FileInfo::get_level_number() {
  return level_number;
}

bool FileInfo::contains_in_range(Key k) {
  return fence_pointer.contains_in_range(k);
}

void FileInfo::delete_file() {
  remove((data_directory + filename).c_str());
  return;
}

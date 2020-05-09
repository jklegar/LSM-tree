#ifndef FILEINFO_H
#define FILEINFO_H

#include "FencePointer.h"
#include "Key.h"
#include "Options.h"
#include <string>
using namespace std;

class FileInfo {
  public:
    FileInfo(string f, int level_no, FencePointer fp);
    string get_filename();
    int get_level_number();
    bool contains_in_range(Key k);
    bool is_between(Key k_1, Key k_2);
    void delete_file();

  private:
    string filename;
    int level_number;
    FencePointer fence_pointer;
};

#endif

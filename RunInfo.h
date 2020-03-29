#ifndef RUNINFO_H
#define RUNINFO_H

#include "BloomFilter.h"
#include "FileInfo.h"
#include "Key.h"

class RunInfo {
  public:
    RunInfo(BloomFilter bf);
    void add_file(FileInfo *f);
    bool possibly_contains(Key k);
    FileInfo get_file(int idx);
    int get_files_number();
    void increment_files_number();
    void delete_all_files();

  private:
    FileInfo *files;
    BloomFilter bloom_filter;
    int files_number;
};

#endif

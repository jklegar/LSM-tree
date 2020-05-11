#ifndef RUNINFO_H
#define RUNINFO_H

#include "BloomFilter.h"
#include "FencePointer.h"
#include "FileInfo.h"
#include "Key.h"

class RunInfo {
  public:
    RunInfo(int hashes);
    void add_file(std::string filename, int level_no, FencePointer fp);
    bool possibly_contains(Key k);
    void add_to_filter(Key k);
    FileInfo* get_file(int idx);
    int get_files_number();
    void increment_files_number();
    void delete_all_files();
    ~RunInfo();

  private:
    std::vector<FileInfo*> files; // pointer to array of pointers
    BloomFilter* bloom_filter;
    int files_number;
};

#endif

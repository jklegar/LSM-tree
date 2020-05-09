#ifndef DATABASE_H
#define DATABASE_H

#include "Buffer.h"
#include "Key.h"
#include "Manifest.h"
#include "Value.h"
#include <mutex>
#include <string>
using namespace std;

class Database {
  public:
    Database();
    void write(Key k, Value v);
    void merge_buffer(Buffer* b);
    void merge(int level_number, int merge_num);
    Value read(Key k);
    void print_all();

  private:
    Manifest m; // initialized here
    int filename_idx;
    string get_filename();
    std::mutex madd_level;
    std::mutex mfilename_idx;
    int merges_started [max_levels] = {}; // initialized to all zeros
    int merges_finished [max_levels] = {}; // initialized to all zeros
};

#endif

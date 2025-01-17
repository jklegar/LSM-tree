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
    void delete_key(Key k);
    void merge_buffer(Buffer* b);
    void merge(int level_number, int merge_num);
    Value read(Key k);
    map<Key, Value> read_range(Key k_1, Key k_2);
    void print_stats();

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

#ifndef BUFFER_H
#define BUFFER_H

#include "Key.h"
#include "Options.h"
#include "Pair.h"
#include "Value.h"
#include <map>
#include <set>
#include <string>
using namespace std;

class Buffer {
  public:
    Buffer(bool is_n = false);
    void append(Pair p);
    bool is_full();
    bool is_empty();
    int len();
    Pair read(int i);
    Pair read_last();
    void save_to(string filename);
    void load_from(string filename);
    void sort();
    bool is_null_buffer();
    bool unordered_find(Key k, Value* v);
    void unordered_find_range(Key k_1, Key k_2, map<Key, Value>* results);
    bool ordered_find(Key k, Value* v);
    void ordered_find_all(map<Key, Value>* results);
    void ordered_find_upper(Key k_1, map<Key, Value>* results);
    void ordered_find_lower(Key k_2, map<Key, Value>* results);
    void ordered_find_within(Key k_1, Key k_2, map<Key, Value>* results);
    void print(int level);
    void add_keys(std::set<Key>* existing_keys, std::set<Key>* deleted_keys);

  private:
    Pair b [file_length]; // gets initialized here using Pair's default constructor
    int idx; // always equals number of pairs in the buffer
    bool is_null;
    bool ordered_find_bounds(Key k, Value* v, int low, int high);
    void ordered_find_all_bounds(map<Key, Value>* results, int low, int high);
    int ordered_find_leq(Key k, int low, int high);
    int ordered_find_geq(Key k, int low, int high);
};

#endif

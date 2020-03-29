#ifndef DATABASE_H
#define DATABASE_H

#include "Buffer.h"
#include "Key.h"
#include "Manifest.h"
#include "Value.h"
#include <string>
using namespace std;

class Database {
  public:
    Database();
    void write(Key k, Value v);
    void merge_buffer(Buffer b);
    void merge(int level_number);
    Value read(Key k);

  private:
    Manifest m;
    int filename_idx;
    string get_filename();
};

#endif

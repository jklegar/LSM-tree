#ifndef BLOOMFILTER_H
#define BLOOMFILTER_H

#include "Key.h"
#include <vector>
using namespace std;

class BloomFilter {
  public:
    BloomFilter(int b, int hashes_no);
    bool possibly_contains(Key k);
    void add(Key k);

  private:
    int hashes_number;
    int bits;
    vector<bool> bitvector;
};

#endif

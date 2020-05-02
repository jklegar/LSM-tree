#ifndef PAIR_H
#define PAIR_H

#include "Key.h"
#include "Value.h"

class Pair {
  public:
    Pair(); // default constructor
    Pair(Key k, Value v);
    Key get_key();
    Value get_value();
    bool less_than(Pair p);
    bool key_equals(Pair p);
    void print();

  private:
    Key key;
    Value value;
};

#endif

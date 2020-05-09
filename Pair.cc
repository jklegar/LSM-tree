#include "Pair.h"
#include <iostream>

// default constructor
Pair::Pair()
  : key(Key(0))
  , value(Value(0)) {
}

Pair::Pair(Key k, Value v)
  : key(k)
  , value(v) {
}

Key Pair::get_key() {
  return key;
}

Value Pair::get_value() {
  return value;
}

bool Pair::less_than(Pair p) {
  return get_key().less_than(p.get_key());
}

bool Pair::key_equals(Pair p) {
  return get_key().equals(p.get_key());
}

void Pair::print(int level) {
  if (get_value().get_is_delete()) {
    std::cout << get_key().get() << ":" << "delete" << ":" << level << std::endl;
  } else {
  std::cout << get_key().get() << ":" << get_value().get() << ":" << level << std::endl;
  }
}

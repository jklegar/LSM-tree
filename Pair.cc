#include "Pair.h"

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

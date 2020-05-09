#include "Key.h"

Key::Key(int k) : key(k) {
}

int Key::get() {
  return key;
}

bool Key::equals(Key k) {
  return (get() == k.get());
}

bool Key::less_than(Key k) {
  return (get() < k.get());
}

bool Key::operator<(Key k) const {
  return (key < k.get());
}

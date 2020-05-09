#include "Key.h"

Key::Key(int k) : key(k) {
}

int Key::get() const {
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

bool Key::is_within(Key k_1, Key k_2) {
  return (k_1.get() <= get() && get() <= k_2.get());
}

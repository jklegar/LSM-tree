#include "FencePointer.h"

FencePointer::FencePointer(Key min, Key max)
  : minimum(min)
  , maximum(max) {
}

bool FencePointer::contains_in_range(Key k) {
  return (minimum.equals(k) || maximum.equals(k)
    || (minimum.less_than(k) && k.less_than(maximum)));
}

bool FencePointer::is_between(Key k_1, Key k_2) {
  return !(k_2.less_than(minimum) || maximum.less_than(k_1));
}

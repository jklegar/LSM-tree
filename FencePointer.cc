#include "FencePointer.h"

FencePointer::FencePointer(Key min, Key max)
  : minimum(min)
  , maximum(max) {
}

bool FencePointer::contains_in_range(Key k) {
  return (minimum.equals(k) || maximum.equals(k)
    || (minimum.less_than(k) && k.less_than(maximum)));
}

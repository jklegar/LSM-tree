#include "Value.h"

Value::Value(int v, bool is_del)
  : value(v)
  , is_delete(is_del) {
}

bool Value::get_is_delete() {
  return is_delete;
}

int Value::get() {
  return value;
}

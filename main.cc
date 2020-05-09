#include "Database.h"
#include "Key.h"
#include "Value.h"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
  Database d;
  for (int i=0; i<101; i++) {
    std::cout << "Writing " << i << std::endl;
    d.write(Key(i), Value(i));
    d.write(Key(i), Value(-i));
    d.write(Key(i), Value(-i));
    d.write(Key(i), Value(i));
    d.write(Key(i), Value(-i));
    Value v = d.read(Key(i));
    if (v.get_is_delete()) {
      std::cout << "delete" << std::endl;
    }
    else {
      std::cout << v.get() << std::endl;
    }
  }
  for (int i=9; i>4; i--) {
    std::cout << "Deleting " << i << std::endl;
    d.delete_key(Key(i));
    Value v = d.read(Key(i));
    if (v.get_is_delete()) {
      std::cout << "delete" << std::endl;
    }
    else {
      std::cout << v.get() << std::endl;
    }
  }
  std::cout << "Writing done" << std::endl;
  for (int i=0; i<101; i++) {
    std::cout << "Reading " << i << " ";
    Value v = d.read(Key(i));
    if (v.get_is_delete()) {
      std::cout << "delete" << std::endl;
    }
    else {
      std::cout << v.get() << std::endl;
    }
  }
  map<Key, Value> res = d.read_range(Key(80), Key(120));
  std::cout << "Reading Range" << std::endl;
  for (auto elem : res) {
    if (elem.second.get_is_delete()) {
      std::cout << elem.first.get() << ":" << "delete" << std::endl;
    } else {
      std::cout << elem.first.get() << ":" << elem.second.get() << std::endl;
    }
  }
  d.print_stats();
  return 0;
}

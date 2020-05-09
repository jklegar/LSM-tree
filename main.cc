#include "Database.h"
#include "Key.h"
#include "Value.h"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
  Database d;
  for (int i=0; i<59; i++) {
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
  /*for (int i=9; i>4; i--) {
    std::cout << "Writing " << i << std::endl;
    d.write(Key(i), Value(12-i));
    Value v = d.read(Key(i));
    if (v.get_is_delete()) {
      std::cout << "delete" << std::endl;
    }
    else {
      std::cout << v.get() << std::endl;
    }
  }*/
  std::cout << "Writing done" << std::endl;
  for (int i=0; i<59; i++) {
    std::cout << "Reading " << i << " ";
    Value v = d.read(Key(i));
    if (v.get_is_delete()) {
      std::cout << "delete" << std::endl;
    }
    else {
      std::cout << v.get() << std::endl;
    }
  }
  d.print_all();
  return 0;
}

#include "Database.h"
#include "Key.h"
#include "Value.h"
#include <chrono>
#include <iostream>
#include <thread>

int main() {
  Database d;
  for (int i=0; i<21; i++) {
    std::cout << "Writing " << i << std::endl;
    d.write(Key(i), Value(i));
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  d.write(Key(0), Value(0));
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  std::cout << "Writing done" << std::endl;
  bool res = 1;
  for (int i=0; i<21; i++) {
    std::cout << "Reading " << i << " ";
    Value v = d.read(Key(i));
    if (v.get_is_delete()) {
      std::cout << "delete" << std::endl;
    }
    else {
      std::cout << v.get() << std::endl;
      if (v.get() != i) {
        res = 0;
      }
    }
  }
  std::cout << "res " << res << std::endl;
  return 0;
}

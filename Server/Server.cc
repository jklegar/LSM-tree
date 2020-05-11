#include "Database.h"
#include "Key.h"
#include "Value.h"
#include "../httplib.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace httplib;

int main() {
  Server svr;
  Database d;
  svr.Get("/print_stats", [&](const Request& req, Response& res) {
    d.print_stats();
  });

  svr.Get("/put", [&](const Request& req, Response& res) {
    int k = std::stoi(req.get_header_value("Key"));
    int v = std::stoi(req.get_header_value("Value"));
    d.write(Key(k), Value(v));
  });

  svr.Get("/delete", [&](const Request& req, Response& res) {
    int k = std::stoi(req.get_header_value("Key"));
    d.delete_key(Key(k));
  });

  svr.Get("/get", [&](const Request& req, Response& res) {
    int k = std::stoi(req.get_header_value("Key"));
    Value v = d.read(Key(k));
    if (v.get_is_delete()) {
      res.set_content("delete", "text/plain");
    } else {
      res.set_content(std::to_string(v.get()), "text/plain");
    }
  });

  svr.Get("/range", [&](const Request& req, Response& res) {
    int k_1 = std::stoi(req.get_header_value("Key_1"));
    int k_2 = std::stoi(req.get_header_value("Key_2"));
    map<Key, Value> results = d.read_range(Key(k_1), Key(k_2));
    for (auto elem : results) {
      if (elem.second.get_is_delete()) {
        std::cout << elem.first.get() << ":" << "delete" << std::endl;
      } else {
        std::cout << elem.first.get() << ":" << elem.second.get() << std::endl;
      }
    }
  });

  svr.Get("/load", [&](const Request& req, Response& res) {
    std::string filename = req.get_header_value("Filename");
    int length = std::stoi(req.get_header_value("Length"));
    ifstream infile;
    infile.open(filename);
    int file_pairs [length * 2];
    infile.read(reinterpret_cast<char*>(&file_pairs), length*sizeof(int)*2);
    infile.close();
    for (int i=0; i<length; i++) {
      d.write(Key(file_pairs[2*i]), Value(file_pairs[2*i+1]));
    }
  });

  svr.listen("localhost", 1234);
}

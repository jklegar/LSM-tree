#include "../httplib.h"
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  httplib::Client cli("localhost", 1234);

  if (argc > 1) {
    std::string cmd(argv[1]);
    if (cmd.compare("s") == 0) {
      auto res = cli.Get("/print_stats");
    }
    else if (cmd.compare("p") == 0) {
      httplib::Headers headers = {
        {"Key", argv[2]},
        {"Value", argv[3]}
      };
      auto res = cli.Get("/put", headers);
    }
    else if (cmd.compare("d") == 0) {
      httplib::Headers headers = {
        {"Key", argv[2]}
      };
      auto res = cli.Get("/delete", headers);
    }
    else if (cmd.compare("g") == 0) {
      httplib::Headers headers = {
        {"Key", argv[2]}
      };
      auto res = cli.Get("/get", headers);
      std::cout << "Reading " << argv[2] << " " << res->body << std::endl;
    }
    else if (cmd.compare("r") == 0) {
      httplib::Headers headers = {
        {"Key_1", argv[2]},
        {"Key_2", argv[3]}
      };
      auto res = cli.Get("/range", headers);
    }
    else if (cmd.compare("l") == 0) {
      httplib::Headers headers = {
        {"Filename", argv[2]},
        {"Length", argv[3]}
      };
      auto res = cli.Get("/load", headers);
    }
    else if (cmd.compare("m") == 0) {
      std::string filename = argv[2];
      int n = std::stoi(argv[3]);
      int file_pairs [n*2];
      for (int i=0; i<n; i++) {
        file_pairs[2*i] = i;
        file_pairs[2*i+1] = i;
      }
      std::ofstream outfile;
      outfile.open(filename);
      outfile.write(reinterpret_cast<char*>(&file_pairs), n*sizeof(int)*2);
      outfile.close();
    }
  }
}

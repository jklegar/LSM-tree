#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
using namespace std;

constexpr int file_length { 32 };
constexpr int runs_per_level { 4 };
constexpr int bloom_filter_bits { 16 };
constexpr int max_levels { 16 };
constexpr char data_directory[] = "/tmp/data/";

#endif

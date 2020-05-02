#ifndef OPTIONS_H
#define OPTIONS_H

#include <string>
using namespace std;

constexpr int file_length { 2 };
constexpr int runs_per_level { 3 };
constexpr int bloom_filter_bits { 16 };
constexpr int max_levels { 16 };
constexpr int max_files_per_run { 256 };
constexpr int max_runs = { 3 };
constexpr char data_directory[] = "/";

#endif

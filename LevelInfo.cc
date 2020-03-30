#include "LevelInfo.h"

LevelInfo::LevelInfo() {
  idx = 0;
  runs = new RunInfo* [max_runs];
}

void LevelInfo::add_run(RunInfo* run) {
  runs[idx] = run;
  idx = idx + 1;
  return;
}

RunInfo* LevelInfo::get_run(int i) {
  return runs[i];
}

bool LevelInfo::is_full() {
  return (idx == runs_per_level);
}

RunInfo** LevelInfo::pop_runs(int n) {
  RunInfo** array_start = runs;
  runs = &runs[n];
  idx = idx - n;
  return array_start;
}

int LevelInfo::get_runs_number() {
  return idx;
}

LevelInfo::~LevelInfo() {
  for (int i=0; i<get_runs_number(); i++) {
    delete get_run(i);
  }
  delete runs;
}
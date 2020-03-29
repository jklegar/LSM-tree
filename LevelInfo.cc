#include "LevelInfo.h"

LevelInfo::LevelInfo() {
  idx = 0;
}

void LevelInfo::add_run(RunInfo *run) {
  runs[idx] = run;
  idx = idx + 1;
  return;
}

RunInfo* LevelInfo::get_run(i) {
  return runs[i];
}

bool LevelInfo::is_full() {
  return (idx == runs_per_level);
}

RunInfo* RunInfo::pop(n) {
  array_start = runs;
  runs = runs[n]
  idx = idx - n;
  return array_start;
}

int RunInfo::get_runs_number() {
  return idx;
}

#include "LevelInfo.h"

LevelInfo::LevelInfo() {
  idx = 0;
  runs = new RunInfo* [max_runs];
}

RunInfo* LevelInfo::add_run(int hashes) {
  RunInfo* run = new RunInfo(hashes);
  runs[idx] = run;
  idx = idx + 1;
  return run;
}

RunInfo* LevelInfo::get_run(int i) {
  return runs[i];
}

bool LevelInfo::is_full() {
  return (idx == runs_per_level);
}

void LevelInfo::pop_runs(int n) {
  RunInfo** old_runs = runs;
  for (int i=0; i<n; i++) {
    old_runs[i]->delete_all_files();
    delete old_runs[i];
  }
  runs = new RunInfo* [max_runs];
  for (int i=n; i<get_runs_number(); i++) {
    runs[i-n] = old_runs[i];
  }
  delete[] old_runs;
  idx = idx - n;
  return;
}

int LevelInfo::get_runs_number() {
  return idx;
}

LevelInfo::~LevelInfo() {
  for (int i=0; i<get_runs_number(); i++) {
    delete get_run(i);
  }
  delete[] runs;
}

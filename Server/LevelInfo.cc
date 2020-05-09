#include "LevelInfo.h"

LevelInfo::LevelInfo() {
  idx = 0;
}

RunInfo* LevelInfo::add_run(int hashes) {
  RunInfo* run = new RunInfo(hashes);
  runs.push_back(run);
  idx = idx + 1;
  return run;
}

RunInfo* LevelInfo::get_run(int i) {
  return runs.at(i);
}

bool LevelInfo::is_full() {
  return (idx % runs_per_level == 0 && idx != 0);
}

void LevelInfo::pop_runs(int n) {
  for (int i=0; i<n; i++) {
    runs.front()->delete_all_files();
    delete runs.front();
    runs.pop_front();
  }
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
}

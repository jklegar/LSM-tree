#ifndef LEVELINFO_H
#define LEVELINFO_H

#include "Options.h"
#include "RunInfo.h"

class LevelInfo {
  public:
    LevelInfo();
    void add_run(RunInfo *run);
    RunInfo* get_run(int i);
    bool is_full();
    RunInfo* pop_runs(int n);
    int get_runs_number();

  private:
    RunInfo *runs;
    int idx;
};

#endif

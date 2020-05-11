#ifndef LEVELINFO_H
#define LEVELINFO_H

#include "Options.h"
#include "RunInfo.h"
#include <deque>

class LevelInfo {
  public:
    LevelInfo();
    RunInfo* add_run(int hashes);
    RunInfo* get_run(int i);
    bool is_full();
    void pop_runs(int n);
    int get_runs_number();
    ~LevelInfo();

  private:
    std::deque<RunInfo*> runs;
    int idx;
};

#endif

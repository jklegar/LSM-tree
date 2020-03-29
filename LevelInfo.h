class LevelInfo {
  public:
    LevelInfo();
    void add_run(RunInfo *run);
    RunInfo* get_run(i);
    bool is_full();
    RunInfo* pop(n);
    int get_runs_number();

  private:
    RunInfo *runs;
    int idx;
};

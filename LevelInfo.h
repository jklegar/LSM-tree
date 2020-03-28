class LevelInfo {
  public:
    LevelInfo();
    void add_run(RunInfo *run);
    RunInfo* get_run(idx);
    bool is_full();

  private:
    RunInfo *runs;
    int idx;
};

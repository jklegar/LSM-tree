class RunInfo {
  public:
    RunInfo(FileInfo *files, BloomFilter bf);
    bool possibly_contains(Key k);
    FileInfo* get_file(idx);

  private:
    FileInfo *files;
    BloomFilter bf;
};

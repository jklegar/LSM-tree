Class FileInfo {
  public:
    FileInfo(std::string filename, int level, FencePointer fp);
    std::string get_filename();
    int get_level();
    bool contains_in_range(Key k);

  private:
    std::string filename;
    int level;
    FencePointer fp;
};

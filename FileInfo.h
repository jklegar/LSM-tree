Class FileInfo {
  public:
    FileInfo(std::string f, int level_no, FencePointer fp);
    std::string get_filename();
    int get_level_number();
    bool contains_in_range(Key k);
    void delete();

  private:
    std::string filename;
    int level_number;
    FencePointer fence_pointer;
};

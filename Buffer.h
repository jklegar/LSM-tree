class Buffer {
  public:
    Buffer();
    void append(Key k, Value v);
    bool is_full();
    Pair read(idx);
    void save_to(std::string filename);
    void load_from(std::string filename);

  private:
    Pair b [file_length];
    int idx;
};

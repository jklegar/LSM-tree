class Buffer {
  public:
    Buffer(bool is_n = False);
    void append(Pair p);
    bool is_full();
    Pair read(i);
    Pair read_last();
    void save_to(std::string filename);
    void load_from(std::string filename);
    void sort();
    bool is_null_buffer();
    bool unordered_find(Key k, Value* v);
    bool ordered_find(Key k, Value* v);

  private:
    Pair b [file_length];
    int idx;
    bool is_null;
};

class Database {
  public:
    Database();
    void write(Key k, Value v);
    void merge_buffer(Buffer b);
    void merge(int level_number);
    Value read(Key k);

  private:
    Manifest m;
};

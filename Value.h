class Value {
  public:
    Value(int v, bool is_del = False);
    bool get_is_delete();
    int get();

  private:
    static const int value;
    static const bool is_delete;
};

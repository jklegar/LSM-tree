class Value {
  public:
    Value(int v, bool is_delete = False);
    bool get_is_delete();
    int get();

  private:
    static const int v;
    static const bool is_delete;
};

class Key {
  public:
    Key(int k);
    int get();
    bool equals(Key k);
    bool less_than(Key k);

  private:
    static const int key;
};

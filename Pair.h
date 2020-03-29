class Pair {
  public:
    Pair(Key k, Value v);
    Key get_key();
    Value get_value();
    bool less_than(Pair p);
    bool key_equals(Pair p);

  private:
    Key key;
    Value value;
};

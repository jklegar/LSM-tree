class Pair {
  public:
    Pair(Key k, Value v);
    Key get_key();
    Value get_value();

  private:
    Key k;
    Value v;
};

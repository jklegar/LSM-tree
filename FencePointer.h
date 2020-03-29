class FencePointer {
  public:
    FencePointer(Key min, Key max);
    bool contains_in_range(Key k);

  private:
    Key minimum;
    Key maximum;
};

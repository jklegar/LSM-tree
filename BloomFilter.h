class BloomFilter {
  public:
    BloomFilter(int b, int hashes_no);
    bool possibly_contains(Key k);
    void add(Key k);

  private:
    int hashes_number;
    int bits;
    std::vector<bool> bitvector;
};

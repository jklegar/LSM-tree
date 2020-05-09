#ifndef KEY_H
#define KEY_H

class Key {
  public:
    Key(int k);
    int get() const;
    bool equals(Key k);
    bool less_than(Key k);
    bool operator<(Key k) const;
    bool is_within(Key k_1, Key k_2);

  private:
    int key;
};

#endif

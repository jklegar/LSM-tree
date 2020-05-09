#ifndef KEY_H
#define KEY_H

class Key {
  public:
    Key(int k);
    int get();
    bool equals(Key k);
    bool less_than(Key k);
    bool operator<(Key k) const;

  private:
    int key;
};

#endif

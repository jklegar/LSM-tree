#ifndef KEY_H
#define KEY_H

class Key {
  public:
    Key(int k);
    int get();
    bool equals(Key k);
    bool less_than(Key k);

  private:
    int key;
};

#endif

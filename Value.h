#ifndef VALUE_H
#define VALUE_H

class Value {
  public:
    Value(int v, bool is_del = false);
    bool get_is_delete();
    int get();

  private:
    int value;
    bool is_delete;
};

#endif

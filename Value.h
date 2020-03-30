#ifndef VALUE_H
#define VALUE_H

class Value {
  public:
    Value(int v, bool is_del = false);
    bool get_is_delete();
    int get();

  private:
    int value; // will be 0 if is_delete=true
    bool is_delete;
};

#endif

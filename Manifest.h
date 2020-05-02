#ifndef MANIFEST_H
#define MANIFEST_H

#include "Buffer.h"
#include "LevelInfo.h"

class Manifest {
  public:
    Manifest();
    void set_buffer(Buffer *b);
    void set_buffer_backup(Buffer *b);
    Buffer* get_buffer();
    Buffer* get_buffer_backup();
    LevelInfo* get_level(int level_number);
    int get_levels_number();
    void roll_buffer();
    void remove_backup();
    void add_level();
    ~Manifest();

  private:
    LevelInfo** levels; // pointer to array of pointers
    Buffer *buffer;
    Buffer *buffer_backup;
    int levels_number;
};

#endif

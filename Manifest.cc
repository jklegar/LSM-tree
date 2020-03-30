#include "Manifest.h"

Manifest::Manifest() {
  levels_number = 0;
  buffer = new Buffer();
  buffer_backup = new Buffer(true);
}

void Manifest::set_buffer(Buffer *b) {
  buffer = b;
  return;
}

void Manifest::set_buffer_backup(Buffer *b) {
  buffer_backup = b;
  return;
}

Buffer* Manifest::get_buffer() {
  return buffer;
}

Buffer* Manifest::get_buffer_backup() {
  return buffer_backup;
}

LevelInfo* Manifest::get_level(int level_number) {
  return levels[level_number];
}

void Manifest::set_level(LevelInfo *level, int level_number) {
  levels[level_number] = level;
  return;
}

int Manifest::get_levels_number() {
  return levels_number;
}

void Manifest::increment_levels_number() {
  levels_number = levels_number + 1;
  return;
}

Manifest::~Manifest() {
  delete buffer;
  delete buffer_backup;
  for (int i=0; i<get_levels_number(); i++) {
    delete get_level(i);
  }
}

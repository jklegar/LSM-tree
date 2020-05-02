#include "Manifest.h"

Manifest::Manifest() {
  levels_number = 0;
  buffer = new Buffer();
  buffer_backup = new Buffer(true);
  levels = new LevelInfo* [max_levels];
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

int Manifest::get_levels_number() {
  return levels_number;
}

void Manifest::roll_buffer() {
  delete buffer_backup;
  buffer_backup = buffer;
  buffer = new Buffer();
  return;
}

void Manifest::remove_backup() {
  delete buffer_backup;
  buffer_backup = new Buffer(true);
  return;
}

void Manifest::add_level() {
  LevelInfo* level_info = new LevelInfo();
  levels[levels_number] = level_info;
  levels_number = levels_number+1;
  return;
}

Manifest::~Manifest() {
  delete buffer;
  delete buffer_backup;
  for (int i=0; i<get_levels_number(); i++) {
    delete get_level(i);
  }
  delete[] levels;
}

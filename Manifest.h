class Manifest {
  public:
    Manifest();
    void set_buffer(Buffer b);
    void set_buffer_backup(Buffer b);
    Buffer get_buffer();
    Buffer get_buffer_backup();
    LevelInfo get_level(int level_number);
    void set_level(LevelInfo *level, int level_number);

  private:
    LevelInfo *levels;
    Buffer b;
    Buffer b_backup; 
};

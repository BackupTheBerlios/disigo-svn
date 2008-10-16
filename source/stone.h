#ifndef STONE_H
#define STONE_H



enum {
  STONE_NORMAL = 0,
  STONE_GRAYED = 1,
  STONE_SHADOW = 2
};

class Stone {
 
  int size;

 public:
  int type;

  //Fl_Shared_Image *img[DRAWSTONE_NUMWHITE+2];

  Stone(int g = 0) { size = 32; type = g; }
  ~Stone();

  void set_size(int s);
  inline int get_size() { return size; }
  void draw(int x, int y, char c);
  void calc_stone();
  void init_image();
};

#endif


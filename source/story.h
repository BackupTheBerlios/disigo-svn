#ifndef STORY_H
#define STORY_H

class Story {
 public:
  Story** variants;
  Story* brother;
  Story* past;
  int nbvariants, current;
  char* commentary;
  int x, y;
  char b[19][19];
  float score[2];
  char player;

  int indent;

  Story(int px, int py, char pl, Story* ppast = 0);
  ~Story();

  Story* step();
  Story* back();
  void add_move(Story*);

  void clear(bool constructor = 0);
};

#endif

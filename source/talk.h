#ifndef TALK_H
#define TALK_H

class Talk;
class Igs;



class Talk {
  Igs* igs;
 // FlTalk* UI;
 public:
  char* name;
  Talk* next;

  Talk(char* n, Igs* b);
  ~Talk();

  Talk* find(char* name, bool create = 1);
  void add_message(char* m, bool show = 1);
  void send(char* m);
  void stats();
  inline void show() { }
};

extern Talk* cur_talk;

/*inline void gtalk(Fl_Widget*w) {
  while (w->parent()) w = w->parent();
  cur_talk = ((FlTalk*)w->user_data())->t;
}*/

#endif

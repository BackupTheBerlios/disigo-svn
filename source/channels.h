#ifndef CHANNELS_H
#define CHANNELS_H

class Channel;



class Channel {
  Igs* igs;

 public:
 // FlChannel* UI;
  int num;
  char* title;

  Channel(Igs*i, int n, bool enter = 1);
  ~Channel();

  void channel_command(const char* s);
};

/** used by FLTK's callback, will contain the current channel */
extern Channel* cur_channel;

/** get which channel is associated to this widget, put it in cur_channel */
/*inline void gchannel(Fl_Widget* w)
{
  while (w->parent() && w->type() != FL_WINDOW) w = w->parent();
  cur_channel = ((FlChannel*)w->user_data())->ch;
}*/

#endif

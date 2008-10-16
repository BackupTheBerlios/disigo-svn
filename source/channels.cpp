#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "igs.h"
#include "channels.h"
#include "defs.h"

Channel* cur_channel;

Channel::Channel(Igs* i, int n, bool enter)
{
  igs = i;
  num = n;
//  UI = new FlChannel();
 // UI->ch = this;

  char s[128];
  if (enter) {
    if (igs->nngs)
      sprintf(s, "channel %d", num);
    else
      sprintf(s, "; \\%d", num);
    igs->send(s);
  }

  sprintf(s, "Channel %d", num);
 // UI->window->label(title = strdup(s));
 // UI->window->show();

  igs->channels[num] = this;
}

Channel::~Channel()
{
//  delete UI;
  free(title);

  if (igs->nngs) {
    char s[128];
    sprintf(s, "channel %d", num);
    igs->send(s);
  }

  igs->channels[num] = 0;
}


void Channel::channel_command(const char* s)
{
  ALLOCA(t, strlen(s) + strlen(igs->myname) + 10);
  if (igs->nngs)
    sprintf(t, "tell %d %s", num, s);
  else
    sprintf(t, "; %s", s);
  igs->send(t);

  sprintf(t, "%s: %s\n", igs->myname, s);
//  UI->output->add(t);
}




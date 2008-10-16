#ifndef WIN32
#include <unistd.h>
#endif
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include "talk.h"
#include "igs.h"
#include "defs.h"

Talk* cur_talk;

Talk::Talk(char* n, Igs* b)
{
  name = strdup(n);
 // UI = new FlTalk();
 // UI->t = this;
 // UI->window->label(name);
  igs = b;
  next = 0;
}

Talk::~Talk()
{
 // delete UI;
  free(name);
}

void Talk::stats()
{
/*  IgsPlayer* p = igs->find_player(name);
  if (p)
    igs->UI->players->select_player(p);*/
  char s[128];
  sprintf(s, "stats %s", name);
  igs->send(s);
}

void Talk::send(char* m)
{
  ALLOCA(s, strlen(m)+strlen(igs->myname)+20);
  sprintf(s, "tell %s %s", name, m);
  igs->send(s);
  sprintf(s, "%s: %s", igs->myname, m);
  add_message(s);
 // UI->input->value("");
  igs->last_talk = this;
}

void Talk::add_message(char* m, bool show)
{
//  UI->output->add(m);
 // UI->output->add("\12");
  //  UI->output->topline(UI->output->size());
 // if (show)
   // UI->window->show();
}

Talk* Talk::find(char* n, bool create)
{
  if (!strcmp(n, name)) 
    return this;
  if (next) 
    return next->find(n, create);
  if (create)
    return next = new Talk(n, igs);
  else
    return 0;
}



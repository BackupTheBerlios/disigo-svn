#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proposegame.h"
#include "igs.h"


ProposeGame* cur_prop;

ProposeGame::ProposeGame(Igs* i, char* wn, char* bn)
{
 // UI = new FlProposeGame(this);
//  UI->whitename->copy_label(wn);
 // if (small_stone->img[0]) UI->whitename->image(small_stone->img[1]);
 // UI->blackname->copy_label(bn);
 // if (small_stone->img[0]) UI->blackname->image(small_stone->img[0]);
 // UI->status->value("");
 // UI->window->show();
  igs = i;
}

ProposeGame::~ProposeGame()
{
 // delete UI;
}

void ProposeGame::swap()
{
//  const char* tmp = UI->whitename->label();
 // UI->whitename->label(UI->blackname->label());
 // UI->blackname->label(tmp);
 // UI->whitename->redraw();
 // UI->blackname->redraw();
}

void ProposeGame::accept()
{
  //bool white = !strcmp(igs->myname, UI->whitename->label());
  char s[128];
/*  sprintf(s, "komi %f", UI->komi->value());
  igs->send(s);*/
 // sprintf(s, "match %s %s %d %d %d", 
	//  white? UI->blackname->label():UI->whitename->label(),
	//  white? "W":"B", 
	//  int(UI->size->value()),
	//  int(UI->main_time->value()),
	//  int(UI->BY_time->value()));
  igs->send(s);
 // UI->status->value("Game proposition sent ...");
}

void ProposeGame::decline()
{
  //bool white = !strcmp(igs->myname, UI->whitename->label());
  char s[128];
 // sprintf(s, "decline %s", 
	//  white? UI->blackname->label():UI->whitename->label());
  igs->send(s);
//  UI->window->hide();
}

void ProposeGame::stats(const char* name)
{
/*  IgsPlayer* p = igs->find_player((char*)name);
  if (p)
    igs->UI->players->select_player(p);*/
  char s[128];
  sprintf(s, "stats %s", name);
  igs->send(s);
}



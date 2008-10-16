#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "igsgame.h"
#include "defs.h"


IgsGame::IgsGame(Igs* i, int n) 
{
  igs = i;
  board = 0;
  pos = 0;
  observed = 0;
  whitename=whiterank=blackname=blackrank = 0;
  flags = 0;
  result = 0;
  wanted_komi = komi = 0.5;
  curpos = 0;
  size = 19;
  handi = 0;
  BY = 0;
  num = n;
  times[0] = times[1] = 0;
  nbmove[0] = nbmove[1] = -1;
 // time_observers = cur_time;
  nbobservers = 0;
}


IgsGame::~IgsGame()
{
  if (board) {
    board->owner = 0;
    board->mode &= ~BOARD_PLAYING;
    //    delete board;
  }
  set_whitename(0);
  if (whiterank) free(whiterank);
  set_blackname(0);
  if (blackrank) free(blackrank);
 // igs->UI->players->redraw();
 // DBGprintf("igsgame destructor\n");
}

void IgsGame::observe()
{
  if (observed) return;
  char s[128];
  sprintf(s, "observe %d", num);
  igs->send(s);
  sprintf(s, "games %d", num);
  igs->send(s);
  sprintf(s, "moves %d", num);
  igs->send(s);
  if (board) {
    board->owner = 0;
    board = 0;
  }
  pos = 0;
  observed = 1;
}

void IgsGame::unobserve()
{
  if (board) {
    board->owner = 0;
    board = 0;
  }
  if (!observed) return;
  if (num>0) {
    char s[128];
    sprintf(s, "unobserve %d", num);
    igs->send(s);
  } else {
   // DBGprintf("Trying to unobserve an orphan game !!\n");
  }
  observed = 0;
}


void IgsGame::add_move(int pp, char p, int i, int j, int m)
{
  if (/*!observed || */pp != pos) return;
  observed = 1;
  init_board(m);
  bool onlast = pos==board->pos;
  pos++;
  board->cur_player = p;
  int oldp = board->pos;
  if (!onlast) board->go(pos-1, onlast);
  board->add_move(i, j);
  if (!onlast) board->go(oldp, onlast);
  else board->go(32000);
  const char* ps = p == WHITE? "White":"Black";
  char s[128];
  if (i >= 0) {
    char c = 'a'+i;
    if (c>='i') c++;
    sprintf(s, "%s %c%d.", ps, c, board->size-j);
  } else {
    if (j == -1)
      sprintf(s, "%s passed.", ps);
    else
      sprintf(s, "%s set handicap to %d stones.", ps, -j);
  }
  board->status(s);
}

void IgsGame::undo()
{
  char s[128];
  sprintf(s, "%s undid last move.", board->cur_player == WHITE? "White":"Black");
  board->status(s);
  if (board->mode&BOARD_SCORING)
    board->clear_mode(BOARD_SCORING);
  else {
    if (pos == 1) {
      while (board->s && board->s->past) board->s = board->s->past;
      if (board->s) delete board->s;
      board->s = 0;
      pos = 0;
      board->go(0);
      board->update_moves_browser();
    } else {
      pos--;
      board->go(pos);
    }
  }
}

void IgsGame::init_board(int mode)
{
  bool ask_obs = 0;
  if (!board) {
    board = new Board(mode);
    if (size != 19)
      board->set_size(size);
    pos = 0;
    board->score[1] = komi;
    board->owner = this;
#define COPY_NAME(a) board->a[sizeof(board->a)-1] = 0; if (*a) strncpy(board->a, a, sizeof(board->a)-1)
    COPY_NAME(whitename);
    COPY_NAME(whiterank);
    COPY_NAME(blackname);
    COPY_NAME(blackrank);
    ask_obs = 1;
  } else if (cur_time-time_observers > 30*TIME_FACTOR)
    ask_obs = 1;

  if (ask_obs) {
    char s[128];
    sprintf(s, "all %d", num);
    igs->send(s);
    time_observers = cur_time;
  }
}


void IgsGame::set_name(char color, char* s)
{ 
  char** name = color == WHITE? &whitename : &blackname;
  char* boardname = board? (color == WHITE? board->whitename : board->blackname) : 0;
  if (*name) {
    if (s && board) strncpy(boardname, s, sizeof(board->whitename)-1);
    if (s && !strcmp(*name, s)) return;
    IgsPlayer* p = igs->find_player(*name);
    if (p) {
      p->playing = 0;
      p->nbobserved = 0;
      igs->update_player();
    }
    free(*name); 
  }
  if (!s) {
    *name = 0;
    return;
  }
  *name = strdup(s); 
  IgsPlayer* p = igs->find_player(*name);
  if (p) p->playing = 1;
 // igs->UI->players->redraw();
}
void IgsGame::set_whiterank(char* s) 
{ 
  if (whiterank) free(whiterank); 
  whiterank = s? strdup(s):strdup(""); 
  if (board && s) strncpy(board->whiterank, s, sizeof(board->whiterank)-1);
}
void IgsGame::set_whitename(char* s) 
{ 
  set_name(WHITE, s);
}
void IgsGame::set_blackname(char* s) 
{ 
  set_name(BLACK, s);
}
void IgsGame::set_blackrank(char* s) 
{ 
  if (blackrank) free(blackrank); 
  blackrank = s? strdup(s) : strdup("");  
  if (board && s) strncpy(board->blackrank, s, sizeof(board->blackrank)-1);
}

void IgsGame::set_komi(float nk)
{
  komi = nk;
  wanted_komi = nk;
  if (!board) return;
/*  if (!board->s)
    board->root_story.score[1] = board->score[1] = komi;
  else*/ {
    if (board->pos == 0)
      board->score[1] = komi;
    board->root_story.score[1] = komi;
    Story* s = &board->root_story;
    float ok = s->score[1];
    while (s) {
      s->score[1] += nk-ok;
      s = s->step();
    }
    board->go(board->pos);
  }
}

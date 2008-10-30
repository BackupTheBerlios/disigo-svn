//
//                   **** ViZiGO (C) 2000 Vincent Penne ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include "defs.h"
#include "igs.h"
#include "igsgame.h"
#include "main_window.h"
#include "binsert.h"
#include "proposegame.h"
#include "player_browser.h"
//#include "vizigo.h"
#include "channels.h"




#define DIRECT_SEND 1
#define DEBUG 1
#define WIFI 1 //to test on NO$GBA

Igs* Igs::first;
Igs* cur_igs;
int cur_time;
char* message= "";

static void build_channel_list();


// is it a symbol ?
static bool is(char c)
{
  switch(c) {
  case '(': case ')': case '[': case ']': case ';': case 10 : case 13:
  case ':': case '{': case '}':
    //  case ')': case ')': case ')': case ')': case ')': case ')': case ')':
    return 1;

  default:
    return 0;
  };
}

static char *p, *end;

static char* getword()
{
  static char s[10*1024];
  while (p<end && isspace(*p)) p++;
  if (is(*p)) {
    *s = *p;
    s[1] = 0;
    p++;
    return s;
  }
  char *ps = s;
  while (p<end && !is(*p) && !isspace(*p)) *ps++ = *p++;
  *ps++ = 0;
  return s;
}

void Igs::insert_game(IgsGame* g)
{
 // UI->games->sort();
  //UI->games->redraw();
 // UI->pobserved->redraw();
}

void Igs::talk_cb()
{
 /*if (UI->players->cur_player) {
    Talk* t;
    if (!talk) talk = t = new Talk(UI->players->cur_player->name, this);
    else t = talk->find(UI->players->cur_player->name);
    t->show();
  }*/
}

void Igs::match_cb()
{
	/*
  if (UI->players->cur_player && strcmp(UI->players->cur_player->name, myname)) {
    ProposeGame* p = find_proposition(UI->players->cur_player->name);
    if (p)
      p->UI->window->show();
    else {
      p = new ProposeGame(this, myname, UI->players->cur_player->name);
      p->UI->main_time->value(1);
      p->UI->BY_time->value(10);
      p->next = propositions;
      propositions = p;
    }
    p->UI->accept_button->label("Send");
    p->UI->accept_button->redraw();
  }*/
}

ProposeGame* Igs::find_proposition(char* o)
{
 /* ProposeGame* p = propositions;
  while(p) {
    if (!strcmp(o, p->UI->whitename->label()) || 
	!strcmp(o, p->UI->blackname->label()))
      return p;
    p = p->next;
  }
  return 0;*/
}

void Igs::update_stats_cb()
{
 /* if (UI->players->cur_player) {
    char s[128];
    sprintf(s, "players %d", UI->players->cur_player - *players);
    printf("%s\n", s);
    send(s);
    sprintf(s, "stats %s", UI->players->cur_player->name);
    send(s);

#ifdef CKM_STORED
   //UI->pstored->init_player(UI->players->cur_player);
    sprintf(s, "stored %s", UI->players->cur_player->name);
    send(s);
#endif
  }*/
}

void Igs::refresh_games()
{
  if (first_players_clicked) {
    send("players");
    first_players_clicked = 0;
  }
  send("games");
  first_games_clicked = 0;
}

void Igs::refresh_players()
{
/*  for (int i=0; i<nbplayers; i++) delete players[i];
  nbplayers = 0;*/
  send("players");
  first_players_clicked = 0;
}

void Igs::observe(int n)
{
  if (n >= MAX_IGSGAMES) return;
 // DBGprintf("ASK OBSERVE %d\n", n);
  IgsGame* g = games[n];
  if (n == played_game) {
   // DBGprintf("TRYIED TO OBSERVE/UNOBSERVE A PLAYED GAME !!\n");
    return;
  }
  if (!g) g = games[n] = new IgsGame(this, n);
  if (g->observed)
    g->unobserve();
  else
    g->observe();
}

#ifdef CKM_STORED
void Igs::Look( char * t ) // at a stored game
{
	
  char m[ 100 ];
  strcpy( m, "look " );
  strcat( m, t );
  send( m );
	
  lookb = new Board(BOARD_LOOKING);
  lookb->Look();
  lookb->set_title( t );
  lookb->pIgs = this;
}

#endif


int cmp_player(const void* pa, const void* pb)
{
  IgsPlayer *a = *(IgsPlayer**)pa, *b = *(IgsPlayer**)pb;
  return strcmp(a->name, b->name);
}


int rank_compare(const char* sa, const char* sb)
{
  char p[10];
  int ra = 0, rb = 0;
  int i, l;
  l = strlen(sa);
  for (i=0; i<l; i++) if (isalpha(sa[i])) break;
  strcpy(p, sa);
  p[i] = 0;
  if (sa[i] == 'd' || sa[i] == 'p') ra = -atoi(p);
  else if (sa[i] == 'k') ra = atoi(p);
  else ra = 1000000;
  l = strlen(sb);
  for (i=0; i<l; i++) if (isalpha(sb[i])) break;
  strcpy(p, sb);
  p[i] = 0;
  if (sb[i] == 'd' || sb[i] == 'p') rb = -atoi(p);
  else if (sb[i] == 'k') rb = atoi(p);
  else rb = 100000;
  return ra-rb;
}

/*void Igs::set_player_flags(char* s, IPFlags f)
{
  IgsPlayer* p = find_player(s);
  if (p) {
    p->set_flags(f);
  }
}*/

IgsPlayer* Igs::find_player(char* s)
{
  int buf[sizeof(IgsPlayer)/sizeof(int)+1];
  IgsPlayer* k = (IgsPlayer*) buf;
  k->name = s;
  IgsPlayer** p = (IgsPlayer**) bsearch(&k, players, nbplayers, 
					      sizeof(IgsPlayer*), cmp_player);
  if (p) return *p;
  else return 0;
}

void Igs::update_player()
{/*
 // IgsPlayer* cur_player = UI->players->cur_player;
  if (!cur_player) cur_player = tmp_player;
  if (cur_player == 0) {
   // UI->pname->value("");
   // UI->prank->value("");
   // UI->talk_button->deactivate();
  //  UI->match_button->deactivate();
  //  UI->update_stats_button->deactivate();
 //   UI->planguage->hide();
  //  UI->pdefaults->hide();
//    UI->pidle->hide();
//    UI->pratedgames->hide();
//    UI->pwins->hide();
 //   UI->plosses->hide();
  //  UI->pinfo->hide();
  //  UI->prank2->hide();
  //  UI->pobserved->hide();
  //  UI->pregistration->hide();
 //   UI->pmail->hide();
#ifdef CKM_STORED
   // UI->pstored->hide();
#endif
  //  UI->presults->hide();
  //  UI->plogged->hide();
  } else {
  //  UI->pname->value(cur_player->name);
    char s[30];
    if (cur_player->ranki <= 0) {
      if (cur_player->rank2i > 0)
	sprintf(s, "%s %d", cur_player->rank, cur_player->rank2i);
      else
	sprintf(s, "%s", cur_player->rank);
    } else
      sprintf(s, "%s %.2f", cur_player->rank, cur_player->ranki+cur_player->rankadjust);
  //  UI->prank->value(s);
  //  UI->talk_button->activate();
    if (played_game == 0 && strcmp(myname, cur_player->name) && 
	cur_player->flags != IPF_CLOSED && 
	!cur_player->playing)
    //  UI->match_button->activate();
    else
   //   UI->match_button->deactivate();
 //   UI->update_stats_button->activate();
    if (cur_player->language) {
   //   UI->planguage->show();
   //   UI->planguage->value(cur_player->language);
    } else
   //   UI->planguage->hide();
    if (cur_player->rank2) {
   //   UI->prank2->show();
      sprintf(s, "%s %d", cur_player->rank2, cur_player->rank2i);
   //   UI->prank2->value(s);
    } else
   //   UI->prank2->hide();
    if (cur_player->defaults) {
   //   UI->pdefaults->show();
   //   UI->pdefaults->clear();
   //   UI->pdefaults->add(cur_player->defaults);
    } else
   //   UI->pdefaults->hide();
    if (cur_player->info) {
   //   UI->pinfo->show();
   //   UI->pinfo->clear();
   //   UI->pinfo->add(cur_player->info);
    } else
   //   UI->pinfo->hide();
    if (cur_player->ratedgames) {
    //  UI->pratedgames->show();
   //   UI->pratedgames->value(cur_player->ratedgames);
    } else
   //   UI->pratedgames->hide();
    if (cur_player->ratedgames) {
  //    UI->pwins->show();
  //    UI->pwins->value(cur_player->wins);
    } else
  //    UI->pwins->hide();
    if (cur_player->ratedgames) {
  //    UI->plosses->show();
  //    UI->plosses->value(cur_player->losses);
    } else
   //   UI->plosses->hide();
    if (cur_player->idle) {
   //   UI->pidle->show();
  //    UI->pidle->value(cur_player->idle);
    } else {
    //  UI->pidle->hide();
    }
    if (cur_player->logged) {
    //  UI->plogged->hide();
    //  UI->pidle->label("Idle:");
    } else {
    //  UI->plogged->show();
   //   UI->pidle->label("Last Access:");
    }
    if (cur_player->mail) {
    //  UI->pmail->show();
    //  UI->pmail->value(cur_player->mail);
    } else
    //  UI->pmail->hide();
    if (cur_player->reg_date) {
    //  UI->pregistration->show();
    //  UI->pregistration->value(cur_player->reg_date);
    } else
   //   UI->pregistration->hide();
    if (cur_player->nbobserved) {
   //   UI->pobserved->nbgames = cur_player->nbobserved+1;
      if ( cur_player->playing ) {
	//UI->pobserved->label("Playing in game:");
      } else if (cur_player->nbobserved>1)
//	UI->pobserved->label("Observing games:");
//      else
	UI->pobserved->label("Observing game:");
   //   memcpy(UI->pobserved->games+1, cur_player->observed, sizeof(int)*
	     cur_player->nbobserved);
     // UI->pobserved->layout();
    //  UI->pobserved->parent()->redraw();
    //  UI->pobserved->show();
    } else
   //   UI->pobserved->hide();
#ifdef CKM_STORED
    if ( cur_player->nbstored ) {
      static char t[ 40 ];
   //   UI->pstored->init_player( cur_player );
      sprintf( t, "%d Stored game(s)", cur_player->nbstored );
  //    UI->pstored->label( t );
  //    UI->pstored->show();
   //   UI->pstored->redraw();
    } else
  //    UI->pstored->hide();
#endif	
    if (cur_player->nbresults) {
    //  UI->presults->player = cur_player;
    //  UI->presults->show();
    //  UI->presults->layout();
    //  UI->presults->redraw();
    } else
   //   UI->presults->hide();
  }*/
}

void Igs::sort_players()
{
 // UI->players->sort();
 // UI->players->redraw();
}

// Inspired by the algorithm in xgospel, using the "proba" command
void Igs::exact_rating(IgsPlayer* p)
{
  if (!received_my_stats) return;
  IgsPlayer* me = find_player(myname);
  if (!me || p->rank[strlen(p->rank)-1] != '*') return;
  int mr = (me->ranki>0)? me->ranki:me->rank2i;
  int tr = (p->ranki>0)? p->ranki:p->rank2i;

  char s[128];
  if (p != me) {
    if (mr == tr)
      sprintf(s, "proba %s 0 5.5", p->name);
    else
      sprintf(s, "proba %s %d -5.5", p->name, ABS(mr-tr));
  } else {
    tr = 26;
    if (mr == tr)
      sprintf(s, "proba ViZiGO 0 5.5");
    else
      sprintf(s, "proba ViZiGO %d -5.5", ABS(mr-tr));
  }
  
  send(s);
}

void Igs::parse(char* s)
{
	int size;
	int mt;
	int byt;
  bool obs = 0;
#ifdef DEBUG
  bool pr = 0;
#endif
//  DBGprintf( "%s\n", s); fflush(stdout);
  char* w = getword();
  if (file_mode) {
    char b[10];
    sprintf(b, "%d", file_mode);
    if (!strcmp(b, w) && !strcmp(getword(), "File")) {
      file_mode = 0; // End of the file
      return;
    }
    switch(file_mode) {
    case 25: // results
      {
	add_message(s+3);
	IgsPlayerResult r;
	p = s;
#define FIXEND(a) r.a[sizeof(r.a)-1] = 0
	FIXEND(winner);
	FIXEND(winner_rank);
	FIXEND(loser);
	FIXEND(loser_rank);
	FIXEND(result);
	FIXEND(date);
	strncpy(r.winner, getword(), sizeof(r.winner)-1);
	getword();
	strncpy(r.winner_rank, getword(), sizeof(r.winner_rank)-1);
	getword(); getword();	getword();	getword();	getword();
	strncpy(r.loser, getword(), sizeof(r.loser)-1);
	getword();
	strncpy(r.loser_rank, getword(), sizeof(r.loser_rank)-1);
	getword();	getword();	getword();	getword();
	getword(); // H
	r.handi = atoi(getword());
	getword(); // K
	r.komi = atof(getword());
	w = getword();
	char* s = w;	while (*s && isdigit(*s)) s++;	if (*s) *s = 0;
	r.size = atoi(w);
	strncpy(r.result, getword(), sizeof(r.result)-1);
	strncpy(r.date, getword(), sizeof(r.date)-1);
	r.rated = (*getword() == 'R');

	// insert the result for all concerned players
	for (int i=0; i<nbplayers; i++)
	  if (!strcmp(r.winner, players[i]->name) ||
	      !strcmp(r.loser, players[i]->name))
	    players[i]->insert_result(r);
	if (tmp_player && (!strcmp(r.winner, tmp_player->name) ||
			   !strcmp(r.loser, tmp_player->name)))
	  tmp_player->insert_result(r);
	update_player();
      }
      break;
    }
    return;
  }
  if (*s == '#') // we are not in client mode !
    send("toggle client true");
  if (!strcmp(w, "Login")) {
    if (!login_sent) {
   		//   connection_mutex.lock();
     	 if (cnx) {
			login_sent = 1;
			//send((char *) myname);
			cnx->write(password, strlen(password));
			cnx->write("\n", 1);
			char * com = "toggle client true\ntoggle quiet false\n";
			cnx->write(com, strlen(com));
     	 }
  //    connection_mutex.unlock();
    }
/*    char* m = "toggle client true\ntoggle verbose false\ntoggle quiet false\n";
    cnx->write(m, strlen(m));*/
  } else if (!strcmp(w, "NNGS")) {
   // DBGprintf("Detected an NNGS server !\n");
    nngs = 1;
/*  } else if (!strcmp(w, "NNGS")) {
    char* m = "toggle client true\n";
    cnx->write(m, strlen(m));*/
  } else if (!isdigit(*w)) {
    if (*s) add_message(s);
  } else if (!strcmp(w, "11")) {
    char* op = p;
    w = getword();
    if (!strcmp(w, "Kibitz")) {
 
      if (last_kibitz_player) free(last_kibitz_player);
      last_kibitz_player = strdup(getword());
      getword(); // [
      if (last_kibitz_rank) free(last_kibitz_rank);
      last_kibitz_rank = strdup(getword());
      getword(); // ]
      getword(); // :
      getword(); // Game
      getword(); // white name
      getword(); // vs
      getword(); // black name
      getword(); // [
      last_kibitz_num = atoi(getword());
    } else {
 
      int n = last_kibitz_num;
      if (last_kibitz_player && n>=1 && n<MAX_IGSGAMES && games[n]) {
	IgsGame* g = games[n];
	if (g->board) {
	  ALLOCA(s, strlen(last_kibitz_player) + 
				   strlen(last_kibitz_rank) + strlen(op)+20);
	  sprintf(s, "%s [%s] : %s", last_kibitz_player, last_kibitz_rank, op);
	  g->board->add_message(s);
	}
      }
    }
  } else if (!strcmp(w, "1")) {
    will_be_ready = 1;
  } else if (!strcmp(w, "5")) {
  //  DBGprintf("COMMAND OF TYPE 5 !!\n");
    add_message(p);
    w = getword();
    if (!strcmp(w, "Cannot")) {
      if (last_talk) {
	char s[128];
	sprintf(s, "It seems %s is not logged on ...", last_talk->name);
	last_talk->add_message(s);
      }
    }
  } else if (!strcmp(w, "40")) {
    last_talk = 0; // OK last talk succeded
  } else if (!strcmp(w, "9") || !strcmp(w, "21")) {
    if (!strcmp(w, "9") && observers_list>0) {
      w = getword();
      if (!strcmp(w, "Found"))
	observers_list = 0;
      else if (observers_list > 0 && observers_list < MAX_IGSGAMES) {
	IgsGame* g = games[observers_list];
	if (g && g->board) {
	  while (p<end && *w) {
	    char s[128];
	    char t[128];
	    strcpy(t, w);
	    sprintf(s, "%11s [%4s ]", t, getword());
	    g->board->add_observer(s);
	    w = getword();
	  }
	}
      }
    } else {
      if (!asked_my_stats) {
	asked_my_stats = 1;
	command_ready = 0;
	send("stats");
	send("players");
	send("games");
	first_players_clicked = 0;
	first_games_clicked = 0;
      }
      char* op = p;
      w = getword();
      if (*w != '{' && strcmp(w, "Game")) 
	add_message(op);
      if (!strcmp(w, "Observing")) {
	char* op = p;
	if (!strcmp(getword(), "game") && (w = getword(), *w != ':')) {
	  observers_list = atoi(w);
	  if (observers_list > 0 && observers_list < MAX_IGSGAMES) {
	    IgsGame* g = games[observers_list];
	    if (g && g->board) {
	      g->board->clear_observer();
	    }
	  }
	}
	p = op;
	w = "Observing";
      }
      if ( nngs && !strcmp(w,"Found") )	{
	getword();
	if ( !strcmp( getword(), "stored" ) )
	    update_player();
      } else if (!strcmp(w, "Channel")) {
	if (!nngs) {

	  w = getword();
	  int n = atoi(w);
	  w = getword();
	  if (n>0 && n<MAX_CHANNELS && !channels[n] && strcmp(w, "turned")) {
	    channels[n] = new Channel(this, n, 0);
	    //	    UI->tabs->add(channels[n]->UI->window);
	  }
	
      }
    } else if (*w == '!') { // shout
      w++;
      if (*w) w[strlen(w)-1] = 0;
      ALLOCA(msg, strlen(w)+strlen(p)+10);
      sprintf(msg, "%s : %s\n", w, p+2);
    //  UI->shout_output->add(msg);
    } else if (!strcmp(w, "Setting")) { // your '.' to someone
 
      last_talk = 0; // OK last talked succeded
    } else if (!strcmp(w, "Game")) {
      if (!strcmp(getword(), "is") && !strcmp(getword(), "titled")) {
	getword(); // :
	IgsGame* g = games[cur_game];
	if (g && g->board) {
	  char s[128];
	  while (p<end && isspace(*p)) p++;
	  if (g->whiterank)
	    snprintf(s, sizeof(s), "Game %d : %s [%s] vs. %s [%s] (%s)", 
		     g->num, g->whitename, g->whiterank, 
		     g->blackname, g->blackrank,
		     p);
	  else
	    snprintf(s, sizeof(s), "Game %d : %s vs. %s (%s)", g->num,
		    g->whitename, g->blackname, p);
	  g->board->set_title(s);
	}
      }
    } else if (!strcmp(w, "Use")) {
      w = getword();
//      printf("'Use' with %s\n", w);
      if (!strcmp(w, "<match")) {

	//  DBGprintf("YES !!!!\n");
	if (cur_opponent) free(cur_opponent);
	cur_opponent = strdup(getword());
	bool white = !strcmp("W", getword());
	
	size = atoi(getword());
	mt = atoi(getword());
	char s[128];
	strcpy(s, getword());
	if (*s) s[strlen(s)-1] = 0;
	byt = atoi(s);
	ProposeGame* p = find_proposition(cur_opponent);
	bool newone = 0;
	if (!p) {
	  p = new ProposeGame(this, cur_opponent, myname);
	  if (white) p->swap();
	 // p->next = propositions;
	  propositions = p;
	  newone = 1;
	} else {
	//  if (strcmp(myname, white? p->UI->whitename->label():
	//  p->UI->blackname->label()))
	    p->swap();
	  //	    p->UI->window->show();
	}
//	p->UI->main_time->value(mt);
//	p->UI->BY_time->value(byt);
//	p->UI->size->value(size);
//	p->UI->size->redraw();
//	p->UI->accept_button->label("Accept");
//	p->UI->accept_button->redraw();
//	p->UI->status->value(newone? 
//			     "Game proposition received ...":
//			     "Game proposition updated ...");
//	p->UI->window->show();
      } else if (!strcmp(w, "\"komi")) {
      }
    } else if (*w == '{') {
      w = getword();
      bool match = 0;
      if (!strcmp(w, "Game") || (match = !strcmp(w, "Match"))) {

	w = getword();
	int n = atoi(w);
	if (n >= MAX_IGSGAMES)
	  return;
	//	DBGprintf("GAME %d\n", n);
	char* ps;
	for (ps = p; ps < end; ps++) if (*ps == '@') break;
	if (ps < end) {
	  p = ps;
	  IgsGame* g = games[n];
	  if (g) g->curpos = atoi(getword());
	} else {
	  IgsGame* g = games[n];
	  if (g && !match) {
	    char result[256];
	    result[0] = 0;
	    char* op1;
	    char* op2;
	    bool ok = 1;
	    getword(); // :
	    if (g->whitename == 0 || strcmp(g->whitename, w = getword())) {
	      ok = 0;
	      g->set_whitename(w);
	    }
	    IgsPlayer* p = find_player(g->whitename);
	    if (p) {
	      p->playing = 0;
	      p->nbobserved = 0;
	      update_player();
	    }
	    getword(); // vs
	    if (g->blackname == 0 || strcmp(g->blackname, w = getword())) 
	      ok = 0;
	    if (!ok) {
	      g->set_blackname(w);
	      g->set_whiterank(0);
	      g->set_blackrank(0);
	    }
	    p = find_player(g->blackname);
	    if (p) {
	      p->playing = 0;
	      p->nbobserved = 0;
	      update_player();
	    }
	    getword(); // :
	    w = getword();
	    char c = 0;
	    if (!strcmp(w, "Black")) {
	      c = BLACK;
	      op1 = g->blackname;
	      op2 = g->whitename;
	    } else if (!strcmp(w, "White")) {
	      c = WHITE;
	      op1 = g->whitename;
	      op2 = g->blackname;
	    } else if (!strcmp(w, "has")) w = getword(); // for NNGS
	    if (!strcmp(w, "adjourned") || !strcmp(w, "adjourned.")) {
	      g->result = IGSGAME_ADJ;
	      g->time_finished = cur_time;
	      sprintf(result, "Game adjourned.");
	  //    DBGprintf("Adjourned !!n");
	    }
	    if (c) {
	      w = getword();
	  //    DBGprintf("%s\n", w);
	      if (!strcmp(w, "resigns") || !strcmp(w, "resigns.")) {
		g->result = 
		   (c==BLACK? IGSGAME_WWINS:IGSGAME_BWINS) | IGSGAME_RESIGN;
		g->time_finished = cur_time;
		sprintf(result, "%s has resigned.", op1);
	      } else if (!strcmp(w, "forfeits")) {
		g->result = 
		 (c==BLACK? IGSGAME_WWINS:IGSGAME_BWINS) | IGSGAME_TIME;
	        g->time_finished = cur_time;
		sprintf(result, "%s ran out of time and lost.", op1);
	      }
	    } else {
	      if (!strcmp(w, "W")) {
		float ws = atof(getword());
		getword();
		float bs = atof(getword());
		g->result = (ws>=bs)? IGSGAME_WWINS:IGSGAME_BWINS;
		g->result_score = ABS(ws-bs);
		sprintf(result, "Score : White %f, Black %f. %s wins.", 
			ws, bs, ws>=bs? "White":"Black");
		g->time_finished = cur_time;
	      }
	    }
	    insert_game(g);
	    if (n == played_game) {
	      played_game = 0;
	      update_player();
	    }
	    if (g->board) {
	//      DBGprintf("detaching board %d from igs\n", n);
	      g->board->clear_mode(BOARD_PLAYING);
	      if (*result) g->board->talk(result);
	      g->board->owner = 0;
	      g->board = 0;
	    }
	  } else if (match) {
	 //   DBGprintf("MATCH %d !!\n", n);

	    bool mygame = 0;
	    char s[64];
	    sprintf(s, "games %d", n);
	    send(s);
	    if (n != played_game && g) {
	      delete g;
	      g = 0;
	    }
	    if (g == 0)
	      g = games[n] = new IgsGame(this, n);
	    getword(); // :
	    g->set_blackname(w = getword());
	    if (!strcmp(myname, w)) mygame = 1;
	    getword(); // [
	    g->set_blackrank(getword());
	    getword(); // ]
	    getword(); // vs.
	    g->set_whitename(w = getword());
	    if (!strcmp(myname, w)) mygame = 1;
	    getword(); // [
	    g->set_whiterank(getword());
	    getword(); // ]
	    g->result = IGSGAME_PLAYING;
	    g->flags = 0;
	    if (mygame && !g->board) {
	      ProposeGame* p = find_proposition(g->blackname);
	      if (!p) p = find_proposition(g->whitename);
	      if (p) {
		//g->size = int(p->UI->size->value());
		//DBGprintf("SET SIZE TO %d\n", g->size);
	      }
	      while (propositions) {
		ProposeGame* next = propositions->next;
		delete propositions;
		propositions = next;
	      }
	      g->init_board(BOARD_OBSERVE|BOARD_PLAYING);
	      g->observed = 1;
	    //  DBGprintf("MY GAME !!\n");
	      played_game = n;
	      update_player();
	    }
	    insert_game(g);
	  }
	}
      } else {
	char* n = strdup(w);
	w = getword();
	if (*w == '[') {   // Somebody connected

	  IgsPlayer* p = new IgsPlayer(n, getword(), this);
	  
	  IgsPlayer** pp = (IgsPlayer**) binsert(&p, (const void**)&players, sizeof(IgsPlayer*), &nbplayers, &maxplayers,
		  cmp_player);
	  if (*pp != p)
	    delete p;
	  else {
	    sort_players();
	  }
	  if (talk) {
	    Talk* t = talk->find(n, 0);
	    if (t) {
	      char s[128];
	      sprintf(s, "%s has reconnected.", n);
	      t->add_message(s, 0);
	    }
	  }
	} else if (!strcmp(w, "has")) { // somebody has disconnected
  
	  IgsPlayer* k = (IgsPlayer*) malloc (sizeof(IgsPlayer));
	  k->name = n;
	  IgsPlayer** p = (IgsPlayer**) bsearch(&k, players, nbplayers, 
					      sizeof(IgsPlayer*), cmp_player);
	  free(k);
	  if (p) {
	    if (cur_player == *p) cur_player = 0;
	/*    if (*p == UI->players->cur_player) {
	      UI->players->cur_player = 0;
	      update_player();
	      UI->players->deselect();
	    }*/
	    delete *p;
	    *p = 0;
	    int n = p-players;
	    memmove(p, p+1, (nbplayers-n-1)*sizeof(IgsPlayer*));
	    nbplayers--;
	    sort_players();
	  }
	  if (talk) {
	    Talk* t = talk->find(n, 0);
	    if (t) {
	      char s[128];
	      sprintf(s, "%s has disconnected.", n);
	      t->add_message(s, 0);
	    }
	  }
	}
	free(n);
      }
    } else if (!strcmp(w, "You")) {
      if (!strcmp(getword(), "can") && !strcmp(getword(), "check")) {
	if (played_game) {

	  IgsGame* g = games[played_game];
	  g->board->set_mode(BOARD_SCORING);
	}
      }
    } else if (!strcmp(w, "Removing")) {
      getword(); // @
      w = getword();
      int i, j;
      i = *w-'A'; if (i>8) i--;
      j = 18-(atoi(w+1)-1);
      if (played_game) {

 	IgsGame* g = games[played_game];
	g->board->remove(i, j, g->board->b[i][j], !g->board->dead[i][j]);
	g->board->update_territory();
      }
    } else if (!strcmp(w, "Creating")) {

/*      while (propositions) {
	ProposeGame* next = propositions->next;
	delete propositions;
	propositions = next;
      }*/
    } else if (!strcmp(w, "Komi")) {
	if (played_game) {
	getword(); // is
	getword(); // now
	getword(); // set
	getword(); // to
	w = getword();
	games[played_game]->set_komi(atof(w));
      }
    } else if (!strcmp(w, "Set")) {
      if (played_game) {
	getword(); // the
	getword(); // komi
	getword(); // to
	w = getword();
	games[played_game]->set_komi(atof(w));
      }
    } else if (!strcmp(w, "Player")) {

      getword();
      w = getword();
      if (!strcmp(w, myname))
	received_my_stats = 1;
      if (!received_my_stats)
	send("stats");
      char s[128];
      sprintf(s, "user %s", w);
      send(s);
      sprintf(s, "results %s", w);
      send(s);
      cur_player = find_player(w);
#ifdef CKM_STORED
      if ( cur_player ) {
	//UI->pstored->init_player(cur_player);
	sprintf(s, "stored %s", w);
	send( s );
      }
#endif
      if (!cur_player && !strcmp(w, myname)) {
	cur_player = new IgsPlayer(myname, "NR", this, IPF_DONTKNOW);
	binsert(&cur_player, (const void**)&players, 
		sizeof(IgsPlayer*), &nbplayers, &maxplayers, cmp_player);
      } else if (!cur_player) {
	if (tmp_player) delete tmp_player;
	cur_player = tmp_player = new IgsPlayer(w, "NR", this, IPF_CLOSED);
//	UI->players->select_player(0);
	update_player();
      }
      if (cur_player)
	cur_player->logged = 1;
    } else {
      ALLOCA(name, strlen(w)+1);
      strcpy(name, w);
      char* op = p;
      w = getword();
      if (!strcmp(w, "offers") || !strcmp(w, "wants")) {

	if (!strcmp(name, myname) || (strcmp(w=getword(), "a") && strcmp(w, "the"))) {
	    if (cur_opponent) free(cur_opponent);
	    cur_opponent = strdup(name);
	  } else {
	    getword(); // new
	    getword(); // komi
	    getword(); // of
	    w = strdup(getword());
	    if (nngs) w[strlen(w)-1] = 0;
	    float k = atof(w);
	    if (played_game && games[played_game]->wanted_komi != k) {
	    if (/*fl_ask("%s offers a new komi of %s, accept ?", name, w)*/1) {
		char s[20];
	//	DBGprintf("KOMI %s", w);
		sprintf(s, "komi %s", w);
		send(s);
		games[played_game]->set_komi(atof(w));
	      } else {
		sprintf(s, "decline %s", name);
		send(s);
	      }
	    }
	    free(w);
	  }
      } else if (!strcmp(w, "declines")) {

	if (played_game) {
	//  fl_message("Your komi proposition was rejected ...");
	} else {
	  ProposeGame* p = find_proposition(name);
	  if (p) {
	  //  p->UI->status->value("Challenge rejected ...");
	  //  p->UI->window->show();
	  }
	}
      }
      if (cur_player) {
	w = name;
	p = op;
	if (!strcmp(w, "Language")) {

	    if (cur_player->language) free(cur_player->language);
	    getword();
	    cur_player->language = strdup(getword());
	    update_player();
	} else if (!strcmp(w, "Defaults")) {

	    if (cur_player->defaults) free(cur_player->defaults);
	    getword();
	    getword();
	    getword();
	    getword();
	    getword();
	    w = getword();
	    cur_player->defaults = strdup(p-strlen(w)-1);
	  //  UI->players->select_player(cur_player);
	} else if (!strcmp(w, "Info")) {
	
	    if (cur_player->info) free(cur_player->info);
	    getword();
	    w = getword();
	    cur_player->info = strdup(p-strlen(w)-1);
	    update_player();
	} else if (!strcmp(w, "Idle")) {

	  getword();
	  getword();
	  getword();
	  getword();
	  getword();
	  getword();
          cur_player->playing = 0;
	  if (cur_player->idle) free(cur_player->idle);
	  cur_player->idle = strdup(getword());
	  if (cur_player->observed) {
	    free(cur_player->observed);
	    cur_player->observed = 0;
	    cur_player->nbobserved = 0;
	  }
	  cur_player->logged = 1;
	  update_player();
	  observers_list = -1;
	} else if (!strcmp(w, "Last")) {
	  if (!strcmp(getword(), "Access")) {
	    for (int i=0; i<8; i++) getword();
	    while(isspace(*p)) p++;
	    if (cur_player->idle) free(cur_player->idle);
	    char s[128];
	    sprintf(s, " %s (local)", p);
	    cur_player->idle = strdup(s);
	    cur_player->logged = 0;
	    update_player();
	  }
	} else if (!strcmp(w, "Reg")) {
	    if (cur_player->reg_date) free(cur_player->reg_date);
	    getword();
	    getword();
	    w = getword();
	    cur_player->reg_date = strdup(p-strlen(w)-1);
	    update_player();
	} else if (!strcmp(w, "Address")) {

	    if (cur_player->mail) free(cur_player->mail);
	    getword();
	    w = getword();
	    cur_player->mail = strdup(p-strlen(w)-1);
	    observers_list = 0;
	    update_player();
	} else if (!strcmp(w, "Playing") || (obs = !strcmp(w, "Observing"))) {
	  //printf("PLAYING : %s\n", s);
	  //printf("PLAYING %d %d\n", int(obs), observers_list);

	  bool not_stats = 0;
	  if (!obs) {
	    if (!strcmp("as", getword())) {
	      bool white = !strcmp("WHITE", getword());
	      getword(); // win/lose
	      getword(); // :
	      float p = 1-atof(getword())/100;
	      if (white) {
		proba_as_white = p;
	      } else {
		if (ABS(p-0.5)>ABS(proba_as_white-0.5)) {
		  p = proba_as_white;
		  white = 1;
		}
		if (cur_player && ABS(p-0.5)<0.4) {
		  if (p<0.5)
		    p = (log(p) - log(0.5))/(2.*log(0.75));
		  else
		    p = (log(0.5) - log(1.0 - p))/(2.*log(0.75));
		  if (!white) proba_handi = -proba_handi;
		  IgsPlayer* pl = find_player(myname);
		  if (pl) {
		    if (!strcmp(cur_player->name, myname)) { // Set my own rating
		      if (pl->ranki <= 0)
			pl->ranki = your_rating;
		      pl->rankadjust = their_rating - pl->ranki + p + proba_handi;
		    } else {
		      cur_player->rankadjust = your_rating + pl->rankadjust - cur_player->ranki - p - proba_handi;
		    }
		  }
		}
	      }
	      not_stats = 1;
	      update_player();
	    } else
	      cur_player->playing = 1; // FIXME : what's that ??
	  }
	  if (!not_stats) {
	    getword();
	    if (observers_list < 0) {
	      getword();
	      for (;;) {
		int n = atoi(getword());
		if (*getword() != '(') break;
		if (n < MAX_IGSGAMES) {
		  if (cur_player->observed)
		    cur_player->observed = (int*) realloc(cur_player->observed, 
							  sizeof(int)*(cur_player->nbobserved+1));
		  else
		    cur_player->observed = (int*)
		      malloc(sizeof(int)*(cur_player->nbobserved+1));
		  cur_player->observed[cur_player->nbobserved] = n;
		  cur_player->nbobserved++;
		}
		getword();
		getword();
	      }
	      update_player();
	    }
	  }
	} else if (!strcmp(w, "Your")) {
	  getword(); // rating
	  getword(); // :
	  your_rating = atoi(getword());
	  getword(); // Their
	  getword(); // rating
	  getword(); // :
	  their_rating = atoi(getword());
	  getword(); // The
	  getword(); // handicap
	  getword(); // :
	  proba_handi = atof(getword());
	} else if (!strcmp(w, "Rank")) {

	  if (cur_player->rank2) free(cur_player->rank2);
	  getword();
	  cur_player->rank2 = strdup(getword());
	  cur_player->rank2i = atoi(getword());
	  update_player();
	} else if (!strcmp(w, "Rating")) {
	
	  free(cur_player->rank);
	  getword();
	  cur_player->rank = strdup(getword());
	  cur_player->ranki = atoi(getword());
	  exact_rating(cur_player);
	  update_player();
	} else if (!strcmp(w, "Wins")) {
	
	  getword();
	  cur_player->wins = atoi(getword());
	  update_player();
	} else if (!strcmp(w, "Losses")) {

	    getword();
	  cur_player->losses = atoi(getword());
	  update_player();
	} else if (!strcmp(w, "Rated")) {

	  getword();
	  getword();
	  cur_player->ratedgames = atoi(getword());
	  update_player();
	} else if (!strcmp(w, "Off") || !strcmp(w, "On")) {
	
	    int f = (!strcmp(w, "On")? IPF_VERBOSE:0); 
	  f |= (!strcmp(getword(), "On")? IPF_BELL:0);
	  f |= (!strcmp(getword(), "On")? IPF_QUIET:0);
	  f |= (!strcmp(getword(), "On")? IPF_SHOUT:0);
	  f |= (!strcmp(getword(), "On")? IPF_AUTOMAIL:0);
	  f |= (!strcmp(getword(), "On")? IPF_OPEN:0);
	  f |= (!strcmp(getword(), "On")? IPF_LOOKING:0);
	  f |= (!strcmp(getword(), "On")? IPF_CLIENT:0);
	  f |= (!strcmp(getword(), "On")? IPF_KIBITZ:0);
	  f |= (!strcmp(getword(), "On")? IPF_CHATTER:0);
	  cur_player->set_flags(f);
	//  UI->open->value(cur_player->flags & IPF_OPEN? 1:0);
	//  UI->bell->value(cur_player->flags & IPF_BELL? 1:0);
	//  UI->quiet->value(cur_player->flags & IPF_QUIET? 1:0);
	//  UI->shout->value(cur_player->flags & IPF_SHOUT? 1:0);
	//  UI->automail->value(cur_player->flags & IPF_AUTOMAIL? 1:0);
	//  UI->looking->value(cur_player->flags & 1? 1:0);
	//  UI->kibitz->value(cur_player->flags & IPF_KIBITZ? 1:0);
	//  UI->chatter->value(cur_player->flags & IPF_CHATTER? 1:0);
	}
      }
    }
    }
  } else if (!strcmp(w, "32")) { // channel
    int n = atoi(getword()); // channel #
    getword(); // :
    if (n>0 && n<MAX_CHANNELS && channels[n]) {
    //  channels[n]->UI->output->add(p);
    //  channels[n]->UI->output->add("\n");
    }
  } else if (!strcmp(w, "7")) {
    getword();
    w = getword();
    if (isdigit(*w)) {

      int n = atoi(w);
      if (n>0 && n<MAX_IGSGAMES) {
	IgsGame* g = games[n];
	if (!g) 
	  g = games[n] = new IgsGame(this, n);
	else 
	  g->result = IGSGAME_PLAYING;
	getword(); // ]
	g->set_whitename(getword());
	getword(); // [
	w = getword(); if (w[strlen(w)-1] != '*') 
	  { w[strlen(w)+1] = 0; w[strlen(w)] = ' '; }
	g->set_whiterank(w);
	getword(); // ]

	getword(); // vs

	g->set_blackname(getword());
	getword(); // [
	w = getword(); if (w[strlen(w)-1] != '*') 
	  { w[strlen(w)+1] = 0; w[strlen(w)] = ' '; }
	g->set_blackrank(w);
	getword(); // ]
	
	getword(); // (
	g->curpos = atoi(getword());
	int size = atoi(getword());
	if (size != g->size) {
	  g->size = size;
	  if (g->board)
	    g->board->set_size(size);
	}
	g->handi = atoi(getword());
	g->set_komi(atof(getword()));
	g->BY = atoi(getword());
	w = getword();
	g->flags = 0;
	while (*w) { 
	  switch(*w++) {
	  case 'I': g->flags |= IGSGAME_I; break;
	  case 'F': g->flags |= IGSGAME_F; break;
	  case 'T': g->flags |= IGSGAME_T; break;
	  }
	}
	getword(); // )
	getword(); // (
	g->nbobservers = atoi(getword());
	insert_game(g);
      }
    }
  } else if (!strcmp(w, "15")) {
    w = getword();
    if (isdigit(*w)) {

      int pos = atoi(w);
      getword();
      w = getword();
      char c = *w == 'B'? BLACK:WHITE;
      getword();
      getword();
      w = getword();
      int i, j;
      if (!strcmp(w, "Pass")) { 
	i=-1; j=-1; 
      } else if (!strcmp(w, "Handicap")) {
	i = -1;
	j = -atoi(getword());
      } else {
	i = *w-'A'; if (i>8) i--;
	j = 18-(atoi(w+1)-1);
      }
      if (cur_game && cur_game < MAX_IGSGAMES) {
	IgsGame* g = games[cur_game];
	if (!g) g = games[cur_game] = new IgsGame(this, cur_game);
	if (i>=0) j += g->size-19;
	if (cur_game != played_game) g->observe();
	g->add_move(pos, c, i, j, BOARD_OBSERVE);
	if (g->board && g->whitename) {
	  char s[128];
	  if (g->whiterank)
	    sprintf(s, "Game %d : %s [%s] vs. %s [%s]", g->num,
		    g->whitename, g->whiterank, g->blackname, g->blackrank);
	  else
	    sprintf(s, "Game %d : %s vs. %s", g->num,
		    g->whitename, g->blackname);
	  g->board->set_title(s);
	}
      }
    } else {
 
      w = getword();
      cur_game = atoi(w);
      if (cur_game >= MAX_IGSGAMES)
	return;
      IgsGame* g = games[cur_game];
      if (!g) g = games[cur_game] = new IgsGame(this, cur_game);
      w = getword();
      g->flags = 0;
      while (*w) { 
	switch(*w++) {
	case 'I': g->flags |= IGSGAME_I; break;
	case 'F': g->flags |= IGSGAME_F; break;
	case 'T': g->flags |= IGSGAME_T; break;
	}
      }
      getword();
      bool mygame = 0;
      bool ok = 1;
      w = getword();
      if (g->whitename == 0 || strcmp(w, g->whitename)) ok = 0;
      g->set_whitename(w);
      if (!strcmp(myname, w)) mygame = 1;
      getword(); // (
      getword();
      int t = atoi(getword());
      g->times[1] = t*TIME_FACTOR + TIME_FACTOR/2;
      g->nbmove[1] = atoi(getword());
      getword(); // )
      getword(); // vs
      w = getword();
      if (g->blackname == 0 || strcmp(w, g->blackname)) ok = 0;
      g->set_blackname(w);
      if (!strcmp(myname, w)) mygame = 1;
      getword(); // (
      getword();
      t = atoi(getword());
      g->nbmove[0] = atoi(getword());
      g->times[0] = t*TIME_FACTOR + TIME_FACTOR/2;
      if (!ok || g->result != IGSGAME_PLAYING) {
	char s[128];
	sprintf(s, "games %d", cur_game);
	send(s);
	g->set_whiterank(0);
	g->set_blackrank(0);
	g->result = IGSGAME_PLAYING;
      }
      if (mygame && !g->board) {
	ProposeGame* p = find_proposition(g->blackname);
	if (!p) p = find_proposition(g->whitename);
	if (p) {
	//  g->size = int(p->UI->size->value());
	//  DBGprintf("SET SIZE BIS TO %d\n", g->size);
	}
	while (propositions) {
	  ProposeGame* next = propositions->next;
	  delete propositions;
	  propositions = next;
	}
	g->observe();
	g->init_board(BOARD_OBSERVE|BOARD_PLAYING);
//	DBGprintf("MY GAME !!\n");
	played_game = cur_game;
	update_player();
      }
      insert_game(g);
    }
  } else if (!strcmp(w, "28")) {
    char* name = strdup(getword());
    if (!strcmp(name, "Undo") && !played_game) {
      getword(); // in
      getword(); // game
      int n = atoi(getword());
      if (n>=1 && n<MAX_IGSGAMES && games[n]) {
	IgsGame* g = games[n];
	g->undo();
	char s[128];
	sprintf(s, "Undo.\n");
	g->board->add_message(s);
      }
    } else {
      w = getword();
      if (!strcmp(w, "undid")) {
	if (played_game) {

	  IgsGame* g = games[played_game];
	  g->undo();
	  char s[128];
	  sprintf(s, "%s has typed undo.\n", name);
	  g->board->add_message(s);
	}
      }
    }
    free(name);
  } else if (!strcmp(w, "24") || !strcmp(w, "19")) {
    w = getword();
    w++; w[strlen(w)-1] = 0;
    IgsGame* g = games[played_game];
    if (played_game && g && 
	(!strcmp(g->whitename, w) || !strcmp(g->blackname, w))) {
 
      char* s = (char*) malloc(strlen(p)+30);
      sprintf(s, "*%s* : %s", w, p+1);
      g->board->add_message(s);
      free(s);
    } else {

      Talk* t;
      if (!talk) talk = t = new Talk(w, this);
      else t = talk->find(w);
      ALLOCA(s, strlen(w) + strlen(p) + 1);
      sprintf(s, "%s%s", w, p);
      t->add_message(s);
    }
  } else if (!strcmp(w, "27") && s[38] == '|') {
 
    int i;
    for (i=52,p = s+15; p<=s+52; p=s+i, i+=52-15) {
      //      p = s+15;
      IPFlags flags = p[-10]=='X'? IPF_CLOSED : 
	(p[-10]=='!'? IPF_LOOKING:IPF_OPEN);
      if (*p == ' ') continue;
      w = getword();
      if (!strcmp(w, "Name") || !strcmp(w, "********")) continue;
      char* n = strdup(w);
      getword();
      IgsPlayer* p = new IgsPlayer(n, getword(), this, flags);
      IgsPlayer** pp = (IgsPlayer**) binsert(&p, (const void**)&players, 
                sizeof(IgsPlayer*), &nbplayers, &maxplayers, cmp_player);
      if (*pp != p) {
	(*pp)->set_flags(flags);
/*	IgsPlayer* op = *pp;
	*pp = p;
	delete op;*/
	delete p;
	//	*pp
      }

      free(n);
    }
    sort_players();
  } else if (!strcmp(w, "25")) {
    w = getword();
    if (!strcmp(w, "File"))
      file_mode = 25;
  } else if (!strcmp(w, "42")) {
    char pn[11];
    char flags[10];
    sscanf(s+3, "%10s", pn);
    IgsPlayer* pl = find_player(pn);
    if (pl) {
      p = s+3+42;
      while(*p && *p++ != '/');
      if (*p) {
	while (*p && *p == ' ') p++;
	while (*p && isdigit(*p)) p++;
	if (*p) {
	  p += 17;
	  memcpy(flags, p, 2);
	  flags[2] = 0;
	  int nf = 0;
	  switch (flags[1]) {
	  case '!': nf = IPF_LOOKING; break;
	  case '-': nf = IPF_OPEN; break;
	  case '?': nf = IPF_DONTKNOW; break;
	  }
	  pl->flags = (pl->flags & ~3) | nf;
//	  UI->players->damage_player(pl);
	  update_player();
	}
      }
    }
#ifdef CKM_STORED
  } else if ((nngs && !strcmp(w, "23")) || (!nngs && !strcmp("18", w))) {
    // do domething with stored games
    w = getword();
    if (!strcmp(w,"found" ) ) {
      update_player();
      return;
    }
		
    for( ; ; ) {
      char buf[128];
      strcpy( buf, w );
			
      char *q = strchr( buf, '-' );
      if ( !q )
	break;
			
      *q = '\0';
      IgsPlayer *pl = find_player(buf);
      if ( pl )
	pl->add_stored(w);
			
      pl = find_player( q + 1 );
      if ( pl )
	pl->add_stored(w);
			
      w = getword();
    }
	
  } else if (!strcmp(w, "22")) {
    if ( lookb ) {
      int y = atol(getword());
      w = getword();
      if ( *w != ':' ) {
	lookb->out_msg_line(s+3);
	return;
      }

      w = getword();

      int x = strlen(w);

      if (y == 0 && x > 4 && x < 19)
	lookb->set_size(x);
      
      while( --x >= 0 ) {
	int c = w[x];
	if ( c == '0' || c == '1' )
	  // may have x and y reversed here
	  lookb->Look( c == '0' ? BLACK : WHITE, y, x );
      }
      
    }
    
  } else if (!strcmp(w, "13")) {
    if ( lookb )
      // Show as output
      lookb->out_msg_line(s+3);
#endif
  } else if (strcmp(w, "27") && strcmp(w, "2"))
    add_message(p);

#ifdef DEBUG
 // fprintf(upLog, "<%s> %s\n", pr? "*":" ", s);
#endif
}

void Igs::read_callback()
{
  /*if (connecting) return;
  if (!cnx) {
    connecting = 1;
    if (/*!fl_choice ("Connection lost for user %s !", "Cancel", "Retry", 0, myname0) {
      delete this;
      return;
    }
    if (!connect()) {
      delete this;
      return;
    }
    return;
  }*/
  will_be_ready = 0;
  if (read_commands) {
    IgsCommand* c;
   // read_command_mutex.lock();
    while (c = read_commands, c) {
      nb_read_commands--;
      read_commands = c->next;
   //   read_command_mutex.unlock();
      p = c->m;
      end = p+strlen(p);
      parse(p);
      free(c->m);
     // read_command_mutex.lock();
    }
  //  read_command_mutex.unlock();
  }
/*  if (will_be_ready) {
    if (commands) {
      IgsCommand* c = commands;
      char* s = c->m;
      int l = strlen(s);
      char* p = (char*) alloca(l+1);
      memcpy(p, s, l);
      memcpy(p+l, "\n", 1);
      int err = cnx->write(p, l+1);
      DBGprintf( "DELAYED SEND : %s (%d)\n", c->m, err);
      command_ready = 0;
      free(c->m);
      commands = c->next;
      delete c;
      command_ready = 0;
    } else
      command_ready = 1;
  }*/
}

void Igs::read_VBL_function()
{
 char message [50] = "";
 /*create_connection();
  while (!cnx->open(server, port, term)) {
    /*if (!fl_choice ("Connection failed !", "Cancel", "Retry", 0)) {
      destroy_connection();
   //   read_thread = 0;
      connecting = 0;
      return;
    }
  }*/
  
  
  login_sent = 0;
  connecting = 0;
  
term->add("Receive socket error");

 /* char* s = new char[1024];

    char* ps = s;
    int len = 0;
    do {
    	
      bool incomplete_command = 0;
      int p = cnx->read(ps, 1024-len);
      term->add(ps);
      if (p == SOCKET_ERROR) 
			term->add("Receive socket error");
      else term->add(ps);
      if (p+len == 1024) incomplete_command = 1;
      if (p <= 0 || thread_quit) {
        delete[] s;
		destroy_connection();
		//	read_thread = 0;
		thread_quit = 0;
		return; // There was a connection error !
      }
      len += p;
      while (ps<s+len) {
		while (*ps != '\n' && ps<s+len) {
	  		if (*ps == 13) {
	    	memmove(ps, ps+1, len-(ps-s)-1);
	   		 len--;
	 		} else
	    		ps++;
			}
			if (ps == s+1024) {
	 		 term->add("DEBUG : Reading buffer overflow !!");
			 break;
			}
			if (ps == s+len) {
	 		 //	  incomplete_command = 1;
	 		 break;
			}
			
			*ps = 0;
			ps++;
			//	read_command_mutex.lock();
			while (nb_read_commands > 200) {
				//  read_command_mutex.unlock();
				term->add("reading thread sleeping for 1 second because of command buffer full !\nis the GUI blocked ?");
	 		 	//sleep(1);
	 		 	PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
				//  read_command_mutex.lock();
			}
			IgsCommand* nc = new IgsCommand, *f = read_commands;
			nc->m = strdup(s);
			nc->next = 0;
			if (f) {
	  			while (f->next) 
	    			f = f->next;
	  				f->next = nc;
			} else
	  		read_commands = nc;
			nb_read_commands++;
			//      Fl::awake(this);
			//	read_command_mutex.unlock();
			len -= ps-s;
			memmove(s, ps, len);
			ps = s;
     	}

    } while (ps<s+1024);
  */

}



void Igs::read_thread_function()
{
 char message [50] = "";
 /*create_connection();
  while (!cnx->open(server, port, term)) {
    /*if (!fl_choice ("Connection failed !", "Cancel", "Retry", 0)) {
      destroy_connection();
   //   read_thread = 0;
      connecting = 0;
      return;
    }
  }*/
  
    create_connection();
cnx->open(server, port, term);

  send((char*)myname);    
 
  login_sent = 0;
  connecting = 0;
  


  char* s = new char[1024];
  for ( ;; ) {
    char* ps = s;
    int len = 0;
    do {
    	
      bool incomplete_command = 0;
      int p = cnx->read(ps, 1024-len);
      term->add(ps);
      if (p == SOCKET_ERROR) 
			term->add("Receive socket error");
      else term->add(ps);
      if (p+len == 1024) incomplete_command = 1;
      if (p <= 0 || thread_quit) {
        delete[] s;
		destroy_connection();
		//	read_thread = 0;
		thread_quit = 0;
		return; // There was a connection error !
      }
      len += p;
      while (ps<s+len) {
		while (*ps != '\n' && ps<s+len) {
	  		if (*ps == 13) {
	    	memmove(ps, ps+1, len-(ps-s)-1);
	   		 len--;
	 		} else
	    		ps++;
			}
			if (ps == s+1024) {
	 		 term->add("DEBUG : Reading buffer overflow !!");
			 break;
			}
			if (ps == s+len) {
	 		 //	  incomplete_command = 1;
	 		 break;
			}
			
			*ps = 0;
			ps++;
			//	read_command_mutex.lock();
			while (nb_read_commands > 200) {
				//  read_command_mutex.unlock();
				term->add("reading thread sleeping for 1 second because of command buffer full !\nis the GUI blocked ?");
	 		 	//sleep(1);
	 		 	PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
				//  read_command_mutex.lock();
			}
			IgsCommand* nc = new IgsCommand, *f = read_commands;
			nc->m = strdup(s);
			nc->next = 0;
			if (f) {
	  			while (f->next) 
	    			f = f->next;
	  				f->next = nc;
			} else
	  		read_commands = nc;
			nb_read_commands++;
			//      Fl::awake(this);
			//	read_command_mutex.unlock();
			len -= ps-s;
			memmove(s, ps, len);
			ps = s;
     	}

#if !DIRECT_SEND
    //  command_mutex.lock();
      if (!incomplete_command && commands) {
		while (commands){
	  		IgsCommand* c = commands;
	  		char* s = c->m;
	  		int l = strlen(s);
	  		ALLOCA(p, l+1);
	 		memcpy(p, s, l);
	  		p[l] = '\n';
	  		int err = cnx->write(p, l+1);
	  		sprintf(message,"DEBUG : DELAYED SEND : %s (%d)\n", c->m, err);
			term->add(message);
	  		free(c->m);
	  		commands = c->next;
	  		delete c;
		}
       command_ready = 0;
      } else
		command_ready = !incomplete_command;
    //  command_mutex.unlock();
#endif
    } while (ps<s+1024);
  }

}

void Igs::send(const char* s)
{
	char debug[100];
//  connection_mutex.lock();
  if (!cnx) return;
#if !DIRECT_SEND
//  command_mutex.lock();
  if (command_ready && !commands) {
    if (1 || *s) {
      int l = strlen(s);
      ALLOCA(p, l+1);
      memcpy(p, s, l);
      p[l] = '\n';
      cnx->write(p, l+1);
    //  DBGprintf( "DIRECT SEND : %s\n", s);
      command_ready = 0;
    }
  } else {
    IgsCommand* nc = new IgsCommand, *f = commands;
    nc->m = strdup(s);
    nc->next = 0;
    if (f) {
      while (f->next) f = f->next;
      f->next = nc;
    } else
      commands = nc;
  }
 // command_mutex.unlock();
#else
  if (*s) {
    int l = strlen(s);
    ALLOCA(p, l+1);
    memcpy(p, s, l);
    memcpy(p+l, "\n", 1);
    cnx->write(p, l+1);
    
#if DEBUG
    	sprintf(debug,"Send %s", p);
 		term->add(debug);
#endif	
  }
#endif
  //connection_mutex.unlock();
}

void Igs::add_message(const char* m)
{
	term->add((char *) m);
  //UI->output->add(m);
 // UI->output->add("\12");
}

void Igs::user_command(const char *s)
{
  add_message(s);
  add_message("");
  IgsGame* g;
  int n;
  if (!strncmp(s, "observe ", 8) && (n=atoi(s+8), n>0) && n<MAX_IGSGAMES && (g = games[n], g)) { 
  //  Hack to have the command "observe" typed by the user working correctly ..
    g->observe();
  } else
    send(s);
}

void read_callback(int s, void* p)
{
  ((Igs*)p)->read_callback();
}

void read_function()
{
  ((Igs*)p)->read_VBL_function();
}

void* read_thread_function(void* p)
{
  ((Igs*)p)->read_thread_function();
  return 0;
}

Igs::Igs(const char* user, const char* pass, const char* pserver, int pport, bool qnngs)
{
int i;
char message[100];
PlayerBrowser* pbrowser;
char letter = ' '; // New letter to write.
s32 nletter = 0; // Next letter to right. 0 since no letters are there yet
	
 // read_thread = 0;
  thread_quit = 0;
  command_ready = 0;
  commands = 0;
  read_commands = 0;
  nb_read_commands = 0;
 // UI = new FlIgs();
 // UI->i = this;

  login_sent = 0;

  last_talk = talk = 0;
  propositions = 0;
  for (int i=0; i<MAX_IGSGAMES; i++) games[i] = 0;
  server = strdup(pserver);
  port = pport;
  myname = strdup(user);
  password = strdup(pass);
  first_games_clicked = 1;
  first_players_clicked = 1;
  observers_list = 0;
  cur_game = 0;
  cur_player = 0;
  players = (IgsPlayer**) calloc(sizeof(IgsPlayer*), 16);
  maxplayers = 16;
  nbplayers = 0;
  nngs = qnngs;
  nngs = strcmp(pserver, "igs.joyjoy.net");
  tmp_player = 0;
 // UI->presults->type(Fl_Browser::VERTICAL | Fl_Browser::MULTI_BROWSER);
 // UI->games->type(Fl_Browser::BOTH | Fl_Browser::MULTI_BROWSER);
 // UI->pobserved->type(Fl_Browser::VERTICAL | Fl_Browser::MULTI_BROWSER);
 // UI->players->type(Fl_Browser::VERTICAL_ALWAYS);
#ifdef CKM_STORED
 // UI->pstored->type(Fl_Browser::VERTICAL | Fl_Browser::MULTI_BROWSER);
  lookb = NULL;
#endif
  cur_opponent = 0;
  played_game = 0;
  asked_my_stats = received_my_stats = 0;
  last_kibitz_rank = last_kibitz_player = 0;
  file_mode = 0;
 // UI->open->setup("open", this);
 // UI->looking->setup("looking", this);
 // UI->bell->setup("bell", this);
 // UI->quiet->setup("quiet", this);
 // UI->shout->setup("shout", this);
 // UI->automail->setup("automail", this);
 // UI->kibitz->setup("kibitz", this);
 // UI->chatter->setup("chatter", this);


  next = first;
  first = this;

 // UI->quiet->hide(); // VP 2005-08-02 disable the quiet setting

  memset(channels, 0, sizeof(void*)*MAX_CHANNELS);

	PA_ResetBgSys();
  PA_ResetSpriteSys();
  
	term = new Term();	
	term->draw();
	screen = TERM_SCREEN;
	pbrowser = new PlayerBrowser();
	
  	connecting = 0;
 	cnx = 0;
 	
 	#if WIFI
 	term->add("WFC initialization");

	PA_InitWifi(); //Initializes the wifi
    PA_ConnectWifiWFC();
 
 	if (!connect()) {
    	delete this;
   		term->add("Connection failed"); 
   
	}
	#endif
	
	
  
  
 	while(1){
 		
		
		if (screen == TERM_SCREEN){
			
		if ((PA_SpriteTouched(PLAYERS_SCREEN)) & !(screen == PLAYERS_SCREEN)){
 			pbrowser->draw();
 			screen = PLAYERS_SCREEN;
 			PA_WaitForVBL();
 		}
 		
 		
						
				letter = PA_CheckKeyboard();	
						
				if ((letter > 31) & (nletter < 100)) { // there is a new letter
					message[nletter] = letter;
					nletter++;
				}
			
				else if ((letter == PA_BACKSPACE)&&nletter) { // Backspace pressed
					nletter--;
					message[nletter] = ' '; // Erase the last letter
				}
				else if (letter == '\n'){ // Enter pressed
					nletter++;	
					#ifdef WIFI
						send(message);	
					#endif
					letter = ' ';
					nletter = 0;
					 memset(message, EMPTY, 100);
					PA_16cClearZone(0, 25, 55, 230, 75);	
				}
				
				PA_16cClearZone(0, 25, 55, 230, 75);
				PA_16cText(0, 28, 60, 230, 80, (char *) message, 1, 1, 100);	
	
				PA_WaitForVBL();
			
	
 		}
 		
 		if (screen == PLAYERS_SCREEN){
 			if (PA_SpriteTouched(TERM_SCREEN)){
 			term->draw();
 			screen = TERM_SCREEN;
 			PA_WaitForVBL();
		}
	}
 		
 	
  }



}

bool Igs::connect()
{
  connecting = 1;

//  Fl::add_fd(cnx->fdin, FL_READ, read_callback, this);
  command_ready = 1;
  while (commands) {
    IgsCommand* tmp = commands;
    free(commands->m);
    commands = commands->next;
    delete tmp;
  }
  asked_my_stats = 0;

  //  connecting = 0;

   create_connection();
	cnx->open(server, port, term);

  	send((char*)myname);    
 
	//PA_VBLFunctionInit(read_function);


 // fl_create_thread(read_thread, ::read_thread_function, this);

  return 1;
}

Igs::~Igs()
{
  send("exit");
 /* if (read_thread) {
    thread_quit = 1;
    connecting = 1;
   // UI->window->hide();
    //UI->window->deactivate();
    while (thread_quit) {
    //  DBGprintf("waiting thread_quit\n");
    //  Fl::wait(0.1);
    }
  }*/
  while (commands) {
    IgsCommand* tmp = commands;
    free(commands->m);
    commands = commands->next;
    delete tmp;
  }
  while (read_commands) {
    IgsCommand* tmp = read_commands;
    free(read_commands->m);
    read_commands = read_commands->next;
    delete tmp;
  }

  Igs* b = first;
  Igs** p = &first;
  while (b) {
    if (b == this) {
      *p = next;
      break;
    }
    p = &b->next;
    b = b->next;
  }

  while (talk) {
    Talk* next = talk->next;
    delete talk;
    talk = next;
  }
  while (propositions) {
    ProposeGame* next = propositions->next;
    delete propositions;
    propositions = next;
  }
  { for (int i=0; i<MAX_IGSGAMES; i++) if (games[i]) delete games[i]; }
  { for (int i=0; i<MAX_CHANNELS; i++) if (channels[i]) delete channels[i]; }
  { int i; for (i=0; i<nbplayers; i++) delete players[i]; }
  free(players);
 // DBGprintf("Igs destructor\n");
  destroy_connection();
  free(password);
  free(myname);
  free(server);
  if (last_kibitz_player) free(last_kibitz_player);
  if (last_kibitz_rank) free(last_kibitz_rank);
  if (cur_opponent) free(cur_opponent);
 // delete UI;
}

void Igs::go_channel(int n)
{
  if (channels[n] == 0) {
    channels[n] = new Channel(this, n);
/*    channels[n]->UI->window->resize(UI->games_group->x(), UI->games_group->y(), UI->games_group->w(), UI->games_group->h());
    channels[n]->UI->window->align(FL_ALIGN_INSIDE);
    channels[n]->UI->window->labeltype(UI->games_group->labeltype());
    channels[n]->UI->window->labelcolor(UI->games_group->labelcolor());
    channels[n]->UI->window->labelfont(UI->games_group->labelfont());
    UI->tabs->add(channels[n]->UI->window);
    UI->tabs->value(channels[n]->UI->window);*/
  } else
  //  channels[n]->UI->window->show();
  if (!nngs) 
    for (int i=1; i<MAX_CHANNELS; i++) {
      if (i != n) {
	delete channels[i];
	channels[i] = 0;
      }
    }
}

static void channel_menu_cb( void* data)
{
  int n = (int) data;
 // gigs(w);
  cur_igs->go_channel(n);
}

static void build_channel_list()
{
  for (int i=1; i<MAX_CHANNELS; i++) {
    char s[10];
    sprintf(s, "%d", i);
  //  Fl_Item* mi = new Fl_Item("");
   /* mi->copy_label(s);
    mi->user_data((void*)i);
    mi->callback(channel_menu_cb);*/
  }
}

void Igs::create_connection()
{
 // connection_mutex.lock();
  if (cnx) delete cnx;
  cnx = new Connection;
  //connection_mutex.unlock();
}

void Igs::destroy_connection()
{
//  connection_mutex.lock();
  if (cnx) {
    delete cnx;
    cnx = 0;
  }
 // connection_mutex.unlock();
}

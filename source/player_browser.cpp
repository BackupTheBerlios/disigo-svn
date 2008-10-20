class PlayerBrowser;
#include "player_browser.h"
#include "stone.h"
#include "igs.h"
#include <PA9.h>
#include "gfx/all_gfx.h"

PlayerBrowser::PlayerBrowser()  

{
  cur_player = 0;
  players = 0;
  nbplayers = 0;
  
  	
	PA_ResetSpriteSys();
	PA_Init16cBg(1, 0);
	PA_LoadTiledBg(1, 1, bgplayer1);  
	PA_LoadTiledBg(0, 1, bgplayer0);  
	PA_LoadSpritePal(0, 1, (void*)pbutton_Pal); 


	PA_CreateSprite(0, 10, (void*)term_Sprite, OBJ_SIZE_64X32, 1, 1, 0, 0);  
	PA_CreateSprite(0, 11, (void*)players_Sprite, OBJ_SIZE_64X32, 1, 1, 64, 0);  
	PA_CreateSprite(0, 12, (void*)games_Sprite, OBJ_SIZE_64X32, 1, 1, 128, 0);  
	PA_CreateSprite(0, 13, (void*)options_Sprite, OBJ_SIZE_64X32, 1, 1, 192, 0);  

	PA_SetSpriteAnim(0, 10, 1);
 
}

PlayerBrowser::~PlayerBrowser()
{
  nbplayers = 0;
}

#ifdef WIN32x
int strcasecmp(const char* a, const char* b)
{
  while(*a && *b) {
    int diff = tolower(*a++) - tolower(*b++);
    if (diff) return diff;
  }
  return *a - *b;
}
#endif

int cmpa_player(const void* pa, const void* pb)
{
  IgsPlayer *a = *(IgsPlayer**)pa, *b = *(IgsPlayer**)pb;
  int d = strcasecmp(a->name, b->name);
  if (d == 0) d = strcmp(a->name, b->name);
  return d;
}

int cmpr_player(const void* pa, const void* pb)
{
  IgsPlayer *a = *(IgsPlayer**)pa, *b = *(IgsPlayer**)pb;
  int d = rank_compare(a->rank, b->rank);
  if (d == 0) d = (b->playing? 0:int(b->flags)&3) - (a->playing? 0:int(a->flags)&3);
  if (d == 0) d = cmpa_player(pa, pb);
  return d;
}

int cmps_player(const void* pa, const void* pb)
{
  IgsPlayer *a = *(IgsPlayer**)pa, *b = *(IgsPlayer**)pb;
  int d = (b->playing? 0:int(b->flags)&3) - (a->playing? 0:int(a->flags)&3);
  if (d == 0) return rank_compare(a->rank, b->rank);
  else return d;
}

void PlayerBrowser::sort()
{
	/*
  gigs(this);
  if (topline() < nbplayers) {
    layout();
    top_player = players[topline()];
    //    DBFprintf("top player '%s'\n", top_player->name);
  } else
    top_player = 0;
  if (players) free(players);
  players = (IgsPlayer**) malloc(sizeof(IgsPlayer*)*cur_igs->nbplayers);
  nbplayers = cur_igs->nbplayers;
  memcpy(players, cur_igs->players, sizeof(IgsPlayer*)*nbplayers);

  int (*compar)(const void *, const void *) = cmpa_player;
  if (cur_igs->UI->pr->value()) compar = cmpr_player;
  else if (cur_igs->UI->ps->value()) compar = cmps_player;
  qsort(players, nbplayers, sizeof(IgsPlayer*), compar);
  if (cur_player) {
/*    IgsPlayer* k = (IgsPlayer*) malloc (sizeof(IgsPlayer));
    k->name = cur_player->name;
    k->rank = cur_player->rank;
    IgsPlayer** p = (IgsPlayer**) bsearch(&k, players, nbplayers, 
					  sizeof(IgsPlayer*), compar);
    if (p) select(p-players, 1);
    else   {
      cur_player = 0;
      cur_igs->update_player();
    }
    free(k);*/
  /*  int i;
    for (i=0; i<nbplayers; i++) if (cur_player == players[i]) break;
    if (i<nbplayers) {
      select(i, 1);
      //      center(i);
    } else {
      cur_player = 0;
      cur_igs->update_player();
    }
  }
  if (top_player) {
    int i;
    for (i=0; i<nbplayers; i++) if (top_player == players[i]) break;
    if (i<nbplayers)
      topline(i);
  }
  layout();
  */
}

void PlayerBrowser::click()
{/*
  int n = value();
  cur_player = players[n];
  gigs(this);
  cur_igs->update_player();
  if (!Fl::event_clicks() && !cur_player->defaults) {
    char s[128];
    sprintf(s, "stats %s", cur_player->name);
    cur_igs->send(s);
    DBGprintf("ASK STATS !!\n");
  }*/
}

void PlayerBrowser::select_player(IgsPlayer* p)
{
	/*
  gigs(this);
  cur_player = p;
  int i;
  for (i=0; i<nbplayers; i++) if (cur_player == players[i]) break;
  if (i<nbplayers) {
    select(i, 1);
    //    center(i);
  click();
  } else {
    cur_player = 0;
    deselect();
    cur_igs->update_player();
  }
  cur_igs->UI->tabs->value(cur_igs->UI->players_group);
  * */
}

void PlayerBrowser::damage_player(IgsPlayer* p)
{
/*  gigs(this);
  int i;
  for (i=0; i<nbplayers; i++) if (p == players[i]) break;
  if (i<nbplayers)
    damage_item(i);*/
 // redraw();
}



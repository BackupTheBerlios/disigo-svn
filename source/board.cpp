#include "defs.h"
//#include "FlCells.h"
#include "board.h"
#include "igsgame.h"
#include "stone.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <FL/fl_ask.H>
//#include <FL/fl_file_chooser.H>
#include <PA9.h>

#include "gfx/all_gfx.h"

#define COND_PLAY_MOVE (!(mode&BOARD_OBSERVE) || \
((mode&BOARD_PLAYING) && \
(pos == owner->pos)))

Board* cur_board;
Stone* stn;
Pointer* ptn;

char debug[20];
//static Fl_Color time_color, time_color2;

Board* Board::first;

/*void Board::observer_cb(Fl_Widget* w, void* p)
{
  Board* b = (Board*) p;
  ObserverBrowser* o = (ObserverBrowser*) w;
  if (b->owner) {
    char s[128];
    char n[128];
    strcpy(n, o->observers[o->value()]);
    char* p = n;
    while(*p && *p == ' ') p++;
    while(*p && *p != ' ') p++;
    *p = 0;
    sprintf(s, "stats %s", n);
    b->owner->igs->send(s);
  }
}*/

Board::Board(int m):root_story(-1, -1, 0)
{
#ifdef CKM_STORED
//  pIgs = NULL;
#endif

	
	
  size = 19;
 // UI = new FlBoard();
 // time_color = UI->black_time->text_background();
 //time_color2 = UI->black_time->text_color();
  //UI->b = this;
  set_size(size);
  cur_player = BLACK;
  for (int i=0; i<19; i++)
    for (int j=0; j<19; j++) {
      b[i][j] = EMPTY;
      root_story.b[i][j] = EMPTY;
    }
  s = &root_story;

  pos = 0;
  root_story.score[0] = score[0] = 0; 
  root_story.score[1] = score[1] = 0.5;
  mode = 0;
  set_mode(m);
//  owner = 0; //< pointer on the possibly associated IGS game
  memset(inib, EMPTY, 19*19);
  memset(dead, EMPTY, 19*19);
  memset(territory, EMPTY, 19*19);
    memset(b, EMPTY, 19*19);
 // memset(old_c, 0, sizeof(old_c)); //time color
  next = first;
  first = this;
  if (!stn) {
    stn = new Stone();
    stn->set_size(20);
  //  stn->init_image();
  }
 // UI->black_box->image(stn->img[0]);
 // UI->white_box->image(stn->img[1]);
 // UI->black_name->hide();
//  UI->white_name->hide();
//  UI->black_time->hide();
  //UI->white_time->hide();
 // UI->moves_browser->board = this;
 // UI->observer_browser->callback(observer_cb, this);
  //  UI->moves_browser->type(Fl_Browser::VERTICAL | Fl_Browser::MULTI_BROWSER);
  strcpy(blackname, "");
  strcpy(blackrank, "");
  strcpy(whitename, "");
  strcpy(whiterank, "");
  filename = 0;
  title = 0;
 
  
  PA_ResetBgSys();
  PA_ResetSpriteSys();
	PA_Init3D(); 
	PA_Reset3DSprites();
	
	PA_LoadTiledBg(0, 3, board19); 
	PA_LoadTiledBg(1, 1, bgscoreigs); 
	PA_Init16cBg(1, 0);
	PA_16cText(1, 120, 10, 255, 192, "chris28ttttt [17k]", 1, 2, 100);	
	//	PA_LoadSpritePal(0, 1, (void*)pass_Pal); 
		
	
 
 	
 	
		PA_3DProcess();  // Update sprites

		PA_WaitForVBL();
		
		if (!ptn) {
    ptn = new Pointer();
    
  }
  u16 gfx[6];
 gfx[PASSBUTTON] = PA_3DCreateTex((void*)pass_Texture, 64, 32, TEX_256COL);
    PA_3DCreateSpriteFromTex(PASSBUTTON, gfx[PASSBUTTON], 64, 32,  2, 33, 164);
    	PA_3DProcess();  // Update sprites
	PA_WaitForVBL();

      
//	while (COND_PLAY_MOVE) {
	while(!(mode&BOARD_OBSERVE)) {
		ptn->move();
		
		//add_move(ptn->xpointer, ptn->ypointer);
		
		 if (mode&BOARD_SCORING) {
  		  //  if (s->nbvariants) return 1;
    	  if (mode&BOARD_PLAYING) {
				if (!dead[ptn->xpointer][ptn->ypointer]) {
	 				char c = 'a'+ptn->xpointer;
	  				if (c>='i') c++;
	  				char s[10];
	 				// sprintf(s, "%c%d", c, this->size-my);
	 				//	 owner->igs->send(s);
				}
     	 }
     	 else {
			if (0)//(mk == 1) event button 
	 		 remove(ptn->xpointer,ptn->ypointer, cur_board->b[ptn->xpointer][ptn->ypointer], !dead[ptn->xpointer][ptn->ypointer]);
			else {
	 		 	if (b[ptn->xpointer][ptn->ypointer] == EMPTY)
	   		 	dead[ptn->xpointer][ptn->ypointer] = dead[ptn->xpointer][ptn->ypointer] == 2? 0:2;
			}
			//	cur_board->dead[mx][my] = !cur_board->dead[mx][my];
			//	refresh();
			//	redraw();
			update_territory();
     	 }
    	}
    
    else if (b[ptn->xpointer][ptn->ypointer] == EMPTY) {
      if (mode&BOARD_PLAYING) { //IGS
		char s[10];
		char c = 'a'+ptn->xpointer;
		if (c>='i') c++;
		//sprintf(s, "%c%d", c, this->size-my);
		//owner->igs->send(s);
      }
      else { // edit mode
      
		add_move(ptn->xpointer, ptn->ypointer);
	//	refresh();
		update_pos();
      }
    }
		// sprintf(debug,"player : %d",cur_player);
		// PA_16cText(1, 10, 30, 255, 192, debug, 1, 2, 100);	
	//	refresh();
	//	update_pos();
	}
		
 //	PA_CreateSprite(0, 1, (void*)pass_Sprite, OBJ_SIZE_64X32, 1, 1, 1, 60);  
}

Board::~Board()
{
  Board* b = first;
  Board** p = &first;
  while (b) {
    if (b == this) {
      *p = next;
      break;
    }
    p = &b->next;
    b = b->next;
  }
  
 // DBGprintf("Board destructor \n");
  //if (owner) { //IGS GAME
  //  DBGprintf("Had an owner !!\n");
  //  owner->unobserve();
    //    delete owner;
 // }
  //delete UI;
  if (filename) free(filename);
  if (title) free(title);
  root_story.clear();

#ifdef CKM_STORED
  //if ( pIgs )
 //   pIgs->lookb = NULL;
#endif	
}

void Board::set_mode(int m)
{
	u16 gfx[6];
	PA_Load3DSpritePal(2, (void*)buttons_Pal); // Palette	
  mode |= m;
  if (mode&BOARD_OBSERVE) {
   // UI->load->hide();
  //  UI->edit->show();
  //  UI->scoring->hide();
  } else {
  //  UI->load->show();
  //  UI->edit->hide();
   // UI->scoring->show();
    gfx[PASSBUTTON] = PA_3DCreateTex((void*)pass_Texture, 64, 32, TEX_256COL);
    PA_3DCreateSpriteFromTex(PASSBUTTON, gfx[PASSBUTTON], 64, 32,  2, 32, 164);
    	PA_3DProcess();  // Update sprites
	PA_WaitForVBL();
  }

  if (mode&BOARD_PLAYING) {
   // UI->komi->show();
   // UI->handicap->show();
   // UI->pass->show();
    gfx[PASSBUTTON] = PA_3DCreateTex((void*)pass_Texture, 64, 32, TEX_256COL);
    PA_3DCreateSpriteFromTex(PASSBUTTON, gfx[PASSBUTTON], 64, 32,  2, 32, 164);
    	PA_3DProcess();  // Update sprites
	PA_WaitForVBL();
   
   // UI->adjourn->show();
  //  UI->undo->show();
  //  UI->close->hide();
  //  UI->resign->show();
    if (mode&BOARD_SCORING) {
    //  UI->resign->label("Done");
      update_territory();
    } else {
    //  UI->resign->label("Resign");
  //  UI->resign->redraw();
}
  } else {
   // UI->komi->hide();
   // UI->handicap->hide();
   // UI->pass->hide();
  //  UI->adjourn->hide();
  //  UI->undo->hide();
  //  UI->close->show();
  //  UI->resign->hide();
  }

#ifdef CKM_STORED
  if (mode&BOARD_LOOKING) {
   // UI->scoring->hide();
   // UI->edit->hide();
   // UI->load->hide();
   // UI->save->hide();
  }
#endif

  if (mode&BOARD_SCORING) {
  //  UI->status->value(mode&BOARD_PLAYING? 
	   //    "Mark dead stones then click Done." :
	   //    "Mark dead stones with left click, drop frontiers with right click.");
  } else if (!(mode&BOARD_PLAYING)) {
   // UI->status->value("Edit mode.");
  }
}

#ifdef CKM_STORED
void Board::Look(int s, int x, int y)
{

  // b[x][y] 0,0 is upper left corner
  if ( s < 0 ) {
    go(0);
    return;
  }

  if ( (unsigned)x < size && (unsigned)y < size ) {
    b[x][y] = s;
  //  UI->cells->refresh();
  }
}
#endif

void Board::clear_mode(int m)
{
  if (m&BOARD_SCORING) {
    memset(dead, EMPTY, 19*19);
    memset(territory, EMPTY, 19*19);
   // UI->cells->refresh();
  }
  mode &= ~m;
  set_mode(0);
}

// Handicap stone generic positions (for 19x19 sized board),
// will be used to compute for arbitrary sized board
static char generic_handicap_stones[8][10][2] =
{
  { {3,15}, {15, 3}, {0} }, // Handicap 2
  { {3,15}, {15, 3}, {3, 3}, {0} }, // Handicap 3
  { {3,15}, {15, 3}, {3, 3}, {15, 15}, {0} }, // Handicap 4
  { {3,15}, {15, 3}, {3, 3}, {15, 15}, {9, 9}, {0} }, // Handicap 5
  { {3,15}, {15, 3}, {3, 3}, {15, 15}, {3, 9}, {15, 9}, {0} }, // Handicap 6
  { {3,15}, {15, 3}, {3, 3}, {15, 15}, {3, 9}, {15, 9}, 
    {9, 9}, {0} }, // Handicap 7
  { {3,15}, {15, 3}, {3, 3}, {15, 15}, {3, 9}, {15, 9}, 
    {9, 3}, {9, 15}, {0} }, // Handicap 8
  { {3,15}, {15, 3}, {3, 3}, {15, 15}, {3, 9}, {15, 9}, 
    {9, 3}, {9, 15}, {9, 9}, {0} } // Handicap 9
};

void Board::set_size(int s)
{
  size = s;
  int d = size > 12? 3 : 2;
  for (int i=0; i<8; i++) {
    int j;
    for (j=0; j<10; j++) {
      for (int k=0; k<2; k++)
	switch(generic_handicap_stones[i][j][k]) {
	case 3:
	  handicap_stones[i][j][k] = d;
	  break;
	case 15:
	  handicap_stones[i][j][k] = size-1-d;
	  break;
	case 9:
	  handicap_stones[i][j][k] = 8*size/19 + 1;
	  break;
	default:
	  handicap_stones[i][j][k] = generic_handicap_stones[i][j][k];
	}
    }
  }
 // UI->cells->set_size(size);
}

void Board::add_move(int x, int y, int make_graph)
{
  if (x < 0 && y <= -2 && y >= -9) {
    int h = -y-2;
    for (int i=0; i<9 && handicap_stones[h][i][0]; i++)
      b[handicap_stones[h][i][0]][handicap_stones[h][i][1]] = cur_player;
  } else if (!cur_player || (x >= 0 && b[x][y])) return;
  if (x >= 0) {
    b[x][y] = cur_player;
    if (capture(cur_player, x, y,0)) /*UI->cells->refresh()*/; // compute possible capture
    else {                     // no capture --> check for suicide
      char sb[19][19];
      float oscore[2]; 
      memcpy(oscore, score, sizeof(oscore));     
      memcpy(sb, b, 19*19);
      if (capture(cur_player == WHITE? BLACK:WHITE, x, y,1)) {  //third parameter testsuicide to not modify sprites
			memcpy(b, sb, 19*19);  // this if forbiden, cancel the move !
        	memcpy(score, oscore, sizeof(oscore));
			b[x][y] = EMPTY;
			PA_16cText(1, 10, 10, 255, 192, "capture", 1, 2, 100);	
			return;
      }
    }
  }
  Story* ns = new Story(x, y, cur_player);
  memcpy(ns->b, b, 19*19);
  memcpy(ns->score, score, sizeof(score));
//  ns->score = score;
  if (s)
    s->add_move(ns);
    stn->draw(ptn->xpointer,ptn->ypointer,cur_player);
  s = ns;
  cur_player = cur_player == WHITE? BLACK:WHITE;
  pos ++;
 // if (make_graph)
 //   UI->moves_browser->make_graph();
}

void Board::go(int n, bool refresh)
{
 /* if (n==0 && owner) {
    score[0] = 0;
    score[1] = owner? owner->komi : 0.5;
    memcpy(root_story.score, score, sizeof(score));
  }*/
#if 0
  if (!s) {
    pos = 0;
    cur_player = BLACK;
    score[0] = 0;
    score[1] = owner? owner->komi : 0.5;
    memcpy(b, inib, 19*19);
    if (refresh) {
     // UI->cells->refresh();
      update_pos();
    }
    return;
  }
#endif
  s = &root_story;
  cur_player = BLACK;
  pos = 0;
#if 0
  if (n <= 0) {
    memcpy(b, inib, 19*19);
    memcpy(score, s->score, sizeof(score));
//    score = s->score;
    if (refresh) {
    //  UI->cells->refresh();
      update_pos();
    }
    return;
  }
#else
  n = n<0? 0:n;
#endif
  //  if (s->x >= 0) b[s->x][s->y] = cur_player;
  cur_player = BLACK;
  //  pos ++;
  while (s->nbvariants && n--) {
    s = s->step();
    if (s->x >= 0) b[s->x][s->y] = cur_player;
    //    capture(cur_player);
    cur_player = cur_player == WHITE? BLACK:WHITE;
    pos ++;
  }
  memcpy(b, s->b, 19*19);
  memcpy(score, s->score, sizeof(score));
  //score = s->score;
  if (refresh) {
  //  if (!owner) {
   //   UI->output->clear();
  //    if (s && s->commentary)
//	UI->output->add(s->commentary);
 //   }
//    UI->cells->refresh();
    update_pos();
  }
}

static int nbcoords;
static char fcoords[500][2];

static void fill(int i, int j, char b[19][19], char cb[19][19], char other, int size)
{
  int ii=i;
  while (ii<size && b[ii][j]==other) {
    if (cb[ii][j] == 1) break;
    cb[ii][j] = 1;
    fcoords[nbcoords][0] = ii;
    fcoords[nbcoords++][1] = j;
    if (j+1<size && b[ii][j+1]==other) fill(ii,j+1,b,cb,other, size);
    if (j-1>=0 && b[ii][j-1]==other) fill(ii,j-1,b,cb,other, size);
    ii++;
  }
  ii = i;
  while (ii-1>=0 && b[ii-1][j]==other) {
    ii--;
    if (cb[ii][j] == 1) break;
    cb[ii][j] = 1;
    fcoords[nbcoords][0] = ii;
    fcoords[nbcoords++][1] = j;
    if (j+1<size && b[ii][j+1]==other) fill(ii,j+1,b,cb,other, size);
    if (j-1>=0 && b[ii][j-1]==other) fill(ii,j-1,b,cb,other, size);
  }
}

// This looks very slow, need to be optimized !
// 29 may 2000 --> OK : seems better now ...
// 18 oct 2000 --> New optimization : do not check capture everywhere, but
// just around the played stone ! This was obvious but ...
static char cb[19][19];
bool Board::capture(char c, int i, int j, int suicide)
{
  bool capture = 0;
  memset(cb, 0, 19*19);
  if (capture_internal(c, i, j, suicide)) capture = 1;
  if (i>0 && capture_internal(c, i-1, j, suicide)) capture = 1;
  if (j>0 && capture_internal(c, i, j-1, suicide)) capture = 1;
  if (i<(size-1) && capture_internal(c, i+1, j,suicide)) capture = 1;
  if (j<(size-1) && capture_internal(c, i, j+1,suicide)) capture = 1;
  return capture;
}

bool Board::capture_internal(char c, int i, int j, int suicide)
{
  char other = c == WHITE? BLACK:WHITE;
  bool capt = 1;

  if (b[i][j] == other && cb[i][j] != 2) {
    nbcoords = 0;
    fill(i, j, b, cb, other, size);
    int k;
    for (k=0; k<nbcoords; k++) {
      int i = fcoords[k][0];
      int j = fcoords[k][1];
      if ((i+1<size && b[i+1][j]==EMPTY) ||
	  (j+1<size && b[i][j+1]==EMPTY) ||
	  (i-1>=0 && b[i-1][j]==EMPTY) ||
	  (j-1>=0 && b[i][j-1]==EMPTY)) { 
	capt = 0; 
	break; 
      }
    }
    for (k=0; k<nbcoords; k++) {
      int i = fcoords[k][0];
      int j = fcoords[k][1];
      if (capt) {
		score[cur_player-1]++;
		b[i][j] = EMPTY;
		if (!suicide) stn->draw(i,j,EMPTY);
      }
      cb[i][j] = 2;
    }
    return capt;
  } else
    return 0;
}


void Board::update_pos()
{
 // if (pos != UI->moves_browser->value())
//    UI->moves_browser->value(pos);
  if (s) {
    int n=pos;
    Story* ps = s;
    while (ps->nbvariants) {
      n++;
      ps = ps->step();
    }
  //  UI->pos->range(0, n);
  } else
  //  UI->pos->range(0, 0);
//  UI->pos->value(pos);
  if (mode&BOARD_SCORING) {
  //  UI->black_score->value(tscore[0]);
  //  UI->white_score->value(tscore[1]);
  } else {
 //   UI->black_score->value(score[0]);
  //  UI->white_score->value(score[1]);
  }
  if (*whitename) {
   // UI->white_name->show();
    char s[128];
    if (*whiterank)
      sprintf(s, "%s [%s]", whitename, whiterank);
    else
      strcpy(s, whitename);
 //   UI->white_name->value(s);
  } else
  //  UI->white_name->hide();
  if (*blackname) {
   // UI->black_name->show();
    char s[128];
    if (*blackrank)
      sprintf(s, "%s [%s]", blackname, blackrank);
    else
      strcpy(s, blackname);
   // UI->black_name->value(s);
  } else
    //UI->black_name->hide();
  update_time(WHITE);
  update_time(BLACK);
}

//void static inline set(Fl_Color& c, Fl_Color p) { memcpy(&c, &p, sizeof(c)); }

void Board::update_time(char player)
{
 // Fl_Output* o = player == WHITE ? UI->white_time : UI->black_time;
 /* if (owner) {
    owner->init_board(mode); // only to update the observer list
    char cur_player = (owner->pos&1)? WHITE:BLACK;
    char s[128];
    char s2[128];
    
    if (owner->times[player-1] < 0) {
      if (owner->nbmove[player-1] < 0) {
	owner->nbmove[player-1] = 24;
	owner->times[player-1] += int(owner->BY*TIME_FACTOR*60);
      } else
	owner->times[player-1] = 0;
    }
    if (owner->nbmove[player-1]>=0)
      sprintf(s2, "%d", owner->nbmove[player-1]);
    else
      strcpy(s2, "-");
    sprintf(s, "%d:%c%c%s%c%c/%s", int(owner->times[player-1]/TIME_FACTOR/60/60),
	    int('0'+int(owner->times[player-1]/TIME_FACTOR/60/10)%6),
	    int('0'+int(owner->times[player-1]/TIME_FACTOR/60)%10),
	    (int(owner->times[player-1]/(TIME_FACTOR/2))&1)? ":":" ",
	    int('0'+int(owner->times[player-1]/TIME_FACTOR/10)%6),
	    int('0'+int(owner->times[player-1]/TIME_FACTOR)%10),
	    s2);
  // Fl_Color c = time_color;
  //  Fl_Color c2 = time_color2;
    if (owner->times[player-1]/TIME_FACTOR < 60 && cur_player == player && (int(owner->times[player-1]/TIME_FACTOR) & 1)) {
      set(c, fl_rgb(200, 200, 200));
      set(c2, fl_rgb(128, 0, 0));
    }
    if (strcmp(s, o->value()) || c != old_c[player-1]) {
      o->value(s);
      if (c != old_c[player-1]) {
	old_c[player-1] = c;
	o->text_background(c);
	o->text_color(c2);
	o->redraw();
      }
    }
    o->show();
  } else {
    o->hide();
  }*/
}

void Board::out_msg_line(const char *m)
{
   // UI->output->add(m);
  //  UI->output->add("\n");
}

void Board::add_message(const char* m)
{
  int opos = pos;
  go(32000);
  if (s) {
    if (s->commentary) {
      s->commentary = (char*) realloc(s->commentary, strlen(s->commentary)+strlen(m)+2);
      strcat(s->commentary, m);
      strcat(s->commentary, "\n");
    } else {
      s->commentary = (char*) malloc(strlen(m)+2);
      strcpy(s->commentary, m);
      strcat(s->commentary, "\n");
    }
  }
  if (pos != opos) 
    go(opos);
 // if (owner) {
  //  UI->output->add(m);
  //  UI->output->add("\n");
 // }
}

void Board::talk(const char* m)
{
  add_message(m);
 /* if (owner) {
    char* command = 0;
    if (mode & BOARD_PLAYING) command = "say";
    else if (mode&BOARD_OBSERVE) {
      char s[20];
      sprintf(s, "kibitz %d", owner->num);
      command = s;
    }
    if (command) {
      char* s = (char*) malloc(100+strlen(m));
      sprintf(s, "%s %s", command, m);
      owner->igs->send(s);
      free(s);
    }
  }*/
}

void Board::pass_cb()
{/*
  if (mode&BOARD_PLAYING) {
    if (owner) owner->igs->send("pass");
  } else
    add_move(-1, -1);
    */ 
}

void Board::adjourn_cb()
{
 /* if (mode&BOARD_PLAYING) {
    if (owner) owner->igs->send("adjourn");
  }*/
}

void Board::undo_cb()
{
/*  if (mode&BOARD_PLAYING) {
    if (owner) owner->igs->send("undo");
  }*/
}

void Board::resign_cb()
{/*
  if (mode&BOARD_SCORING) {
    if (owner) owner->igs->send("done");
  } else
    if (owner && fl_ask("Are you sure ?")) owner->igs->send("resign");
    */
}

void Board::load_cb()
{
	/*
  char *f = fl_file_chooser("peek an SGF file", "*.sgf", filename);
  if (f) {
    if (filename) free(filename);
    filename = strdup(f);
    set_title(filename);
    load_sgf(f);
  }
  */
}

void Board::save_cb()
{/*
  if (!filename && owner) {
    char s[128];
    sprintf(s, "%s-%s.sgf", owner->whitename, owner->blackname);
    filename = strdup(s);
  } else if (!filename && *whitename && *blackname) {
    filename = (char*) malloc(strlen(whitename)+strlen(blackname)+10);
    sprintf(filename, "%s-%s.sgf", whitename, blackname);
  }
  char *f = fl_file_chooser("peek an SGF file", "*.sgf", filename);
  if (f) {
    if (filename) free(filename);
    filename = strdup(f);
    if (!owner) set_title(filename);
    save_sgf(f);
  }*/
}

void Board::edit_cb()
{
  int oldp = pos;
  Board* nb = new Board();
  #define COPY_NAME(a) strcpy(nb->a, a)
  COPY_NAME(whitename);
  COPY_NAME(whiterank);
  COPY_NAME(blackname);
  COPY_NAME(blackrank);
  nb->set_size(size);
  
  go(0);
  
  memcpy(nb->score, score, sizeof(score));
  memcpy(nb->root_story.score, score, sizeof(score));
  //nb->score = score;
/*  if (s) {
    while(s->step()) {
      nb->add_move(s->x, s->y);
      go(++pos);
    }
    nb->add_move(s->x, s->y);
    nb->go(32000);
  }*/
  Story* ts = s;
  if (ts) {
    while (ts->past) ts = ts->past;
    ts = ts->step();
    while (ts) {
      nb->add_move(ts->x, ts->y);
      ts = ts->step();
      nb->go(32000);
    }
  }

  go(oldp);
}

void Board::scoring_cb()
{/*
  if (UI->scoring->value()) {
    set_mode(BOARD_SCORING);
    update_territory();
  } else {
    clear_mode(BOARD_SCORING);
  //  UI->cells->refresh();
  }*/
}

static void fill2(int i, int j, char b[19][19], int size)
{
  if (b[i][j] >= 3) return;
  b[i][j] = 3;
  if (i+1<size && b[i+1][j]==EMPTY) fill2(i+1,j,b, size);
  if (j+1<size && b[i][j+1]==EMPTY) fill2(i,j+1,b, size);
  if (i-1>=0 && b[i-1][j]==EMPTY) fill2(i-1,j,b, size);
  if (j-1>=0 && b[i][j-1]==EMPTY) fill2(i,j-1,b, size);
}

void Board::update_territory()
{
  if (s)
    memcpy(tscore, s->score, sizeof(score));
    //tscore = s->score;
  else
    memcpy(tscore, score, sizeof(score));
    //tscore = score;

  int i, j;
  for (i=0; i<size; i++)
    for (j=0; j<size; j++) {
      switch (dead[i][j]) {
      case 0:
	territory[i][j] = b[i][j];
	break;
      case 1:
	territory[i][j] = EMPTY;
	break;
      case 2:
	territory[i][j] = 32;
	break;
      }
      if (dead[i][j] == 1) {
	tscore[b[i][j] == WHITE? 0:1]++;
      }
    }

  for (i=0; i<size; i++)
    for (j=0; j<size; j++) {
      if (territory[i][j] == EMPTY) {
	fill2(i, j, territory, size);
	bool w=0, b=0;
	for (int i=0; i<size; i++)
	  for (int j=0; j<size; j++) {
	    if (territory[i][j] == 3) {
	      if (i+1<size) { 
		char c = territory[i+1][j];
		if (c==WHITE) w=1;
		else if (c==BLACK) b=1;
	      }
	      if (j+1<size) { 
		char c = territory[i][j+1];
		if (c==WHITE) w=1;
		else if (c==BLACK) b=1;
	      }
	      if (i-1>=0) { 
		char c = territory[i-1][j];
		if (c==WHITE) w=1;
		else if (c==BLACK) b=1;
	      }
	      if (j-1>=0) { 
		char c = territory[i][j-1];
		if (c==WHITE) w=1;
		else if (c==BLACK) b=1;
	      }
	    }
	  }
	  {
	  for (int i=0; i<size; i++)
	  for (int j=0; j<size; j++) {
	    if (territory[i][j] == 3) {
	      if (w &&!b) {
		territory[i][j] = 4+WHITE;
		tscore[1]++;
	      } else if (!w &&b) {
		territory[i][j] = 4+BLACK;
		tscore[0]++;
	      } else
		territory[i][j] = 4+EMPTY;
	    }
	  }
	  }
      }
    } 

  update_pos();
 // UI->cells->refresh();
}


void Board::remove(int i, int j, char c, char d) 
{
  if (b[i][j] == EMPTY) return;
  dead[i][j] = d;
  if (i+1<size && b[i+1][j]==c && dead[i+1][j]!=d) remove(i+1,j,c,d);
  if (j+1<size && b[i][j+1]==c && dead[i][j+1]!=d) remove(i,j+1,c,d);
  if (i-1>=0 && b[i-1][j]==c && dead[i-1][j]!=d) remove(i-1,j,c,d);
  if (j-1>=0 && b[i][j-1]==c && dead[i][j-1]!=d) remove(i,j-1,c,d);
}

void Board::handicap_cb()
{
  /*const char* s = fl_input("Enter the handicap :", "9");
  if (!s) return;
  int h = atoi(s);
  if (h < 2 && h > 9) return;
  char p[128];
  sprintf(p, "handi %d", h);
  owner->igs->send(p);*/
}

void Board::komi_cb()
{/*
  if (!owner) return;
  char k[10];
  sprintf(k, "%2.1f", owner->komi);
  const char* s = fl_input("Enter the komi :", k);
  if (!s) return;
  owner->wanted_komi = atof(s);
  char p[128];
  sprintf(p, "komi %s", s);
  owner->igs->send(p);
  */
}

void Board::set_title(char* t)
{
  if (title) {
    if (!strcmp(t, title)) 
      return;
    else
      free(title);
  }
  title = strdup(t);
//  UI->window->label(title);
}


void Board::add_observer(char* n)
{// UI->observer_browser->add(n);
/*  Fl_Widget* w = */
/*  w->label_color(UI->observer_browser->text_color());
#ifdef WIN32
  w->label_size(UI->observer_browser->text_size()-2);
#endif
  w->label_font(UI->observer_browser->text_font());*/
}

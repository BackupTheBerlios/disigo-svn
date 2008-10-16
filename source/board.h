//
//                   **** ViZiGO (C) 2000 Vincent Penne ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//
/** 
 *  @file board.h
 *
 * Board class definition
 *
 */

#ifndef BOARD_H
#define BOARD_H

class IgsGame;
class Board;
#include "defs.h"
#include "story.h"
#include "pointer.h"
#include "igs.h"
//class FlCells;
//#include "flboard.h"
#include <time.h>
//#include <FL/Fl_Color.H>


enum {
  BOARD_OBSERVE = 0x0001,
  BOARD_PLAYING = 0x0002,
  BOARD_SCORING = 0x0004,
  BOARD_LOOKING = 0x0008,
};

/** Board class
 *
 * Each board (played game, observed game, edited game) is associated to a
 * board class
 *
 */
class Board {
  //FlBoard *UI;      ///< pointer on the FLTK user interface

 public:
  Board* next;
  static Board* first;
  
  int size;         ///< size of the board (default = 19)
  int pos;          ///< current position number
  float score[2];   ///< score (black/white)
  float tscore[2];  ///< score with territory
  char b[19][19];   ///< current board
  char inib[19][19]; ///< board at move 0 (handicap stones)
  char dead[19][19];///< dead stones board
  char territory[19][19];///< territory board
  char cur_player;  ///< current player
  Story* s;         ///< pointer on the variations tree at current position
  Story root_story; ///< root of the variation tree
  int mode;         ///< mode (playing, observing, editing, etc ...)
  IgsGame* owner;   ///< pointer on the possibly associated IGS game
  char* filename;   ///< name of the current sgf file
  char handicap_stones[8][10][2]; ///< handicap stones positions
//  Fl_Color old_c[2]; ///< previous colors used for time
  char* title;       ///< current title
  char whitename[20]; ///< white name
  char whiterank[6]; ///< white rank
  char blackname[20]; ///< black name
  char blackrank[6]; ///< black rank
  
 	

  /** constructor 
   * @param m mode
   */
  Board(int m = 0); 
  ~Board();

  /** add a stone with color cur_player
   * @param x x position (-1 to pass)
   * @param y y position 
   */
  void add_move(int x, int y, int refresh = 1);

  /** go to the position n in the current variant */
  void go(int n, bool refresh = 1);

  /** load an sgf file
   * @param name filename (zero to parse a variant in an already opened sgf 
   * file)
   */
  void load_sgf(char* name = 0);

  /** save an sgf file
   * @param name filename (zero to save a variant in an already opened sgf 
   * file)
   */
  void save_sgf(char* name = 0);

  /** handle captures
   * @param  c    player
   * @param suicide not update sprites
   * @return true if there was some capture 
   */
  bool capture(char c, int i, int j, int testsuicide);

  /** called by capture */
  bool capture_internal(char c, int i, int j, int suicide);

  /** update the UI elements for the current position (position, time, etc ...)
   */
  void update_pos();

  /** update the time on the board for player p */
  void update_time(char p);
  
  /** set some mode bits */
  void set_mode(int m);

  /** clear some mode bits */
  void clear_mode(int m);

  /** set the title of the board window */
  void set_title(char* t);

  /** add a message in the output window */
  void add_message(const char* m);
  void out_msg_line(const char *m);	

  /** talk to the opponent (when playing), or say in game */
  void talk(const char* m);

  /** pass button callback */
  void pass_cb();

  /** adjourn button callback */
  void adjourn_cb();

  /** edit button callback */
  void edit_cb();

  /** undo button callback */
  void undo_cb();

  /** scoring button callback */
  void scoring_cb();

  /** resign button callback */
  void resign_cb();

  /** load button callback */
  void load_cb();

  /** save button callback */
  void save_cb();

  /** handicap button callback */
  void handicap_cb();

  /** komi button callback */
  void komi_cb();

  /** update the territory */
  void update_territory();

#ifdef CKM_STORED
  void Look( int s = -1, int x = -1, int y = -1 );
  Igs *pIgs;
#endif

  /** mark/unmark a dead group 
   *
   * @param i horizontal coordinate
   * @param j vertical coordinate
   * @param c color of the stone
   * @param d state we want (1=dead, 0=alive)
   */
  void remove(int i, int j, char c, char dead);

  /** set the size of the board */
  void set_size(int size);

  /** observer browser callback */
 // static void observer_cb(Fl_Widget*, void*);

  inline void clear_observer() { /*UI->observer_browser->clear();*/ }
 void add_observer(char* s);
  inline void status(const char* s) {/* UI->status->value(s);*/ }
  inline void update_moves_browser() {/* UI->moves_browser->make_graph(); */}

  void clear() { go(0); s->clear(); } ///< clear all moves
};


/** used by FLTK's callback, will contain the current board */
//extern Board* cur_board;

/** get the board associated this widget belong to, put it in cur_board */
//inline void gboard(Fl_Widget* w)
//{
 // while(w->parent()) w = w->parent();
 // cur_board = ((FlBoard*) w->user_data())->b;
//}

#endif

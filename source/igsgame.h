//
//                   **** ViZiGO (C) 2000 Vincent Penne ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//
/** 
 *  @file igsgame.h
 *
 * Observed and played game on IGS
 *
 */

#ifndef IGSGAME_H
#define IGSGAME_H

#include "igs.h"
#include "board.h"
#include "igsplayer.h"

/** Type of game */
enum  {
  IGSGAME_F = 1, ///< Free game
  IGSGAME_I = 2, ///< ?
  IGSGAME_T = 4  ///< ?
};

/** Status of a game */
enum  {
 IGSGAME_PLAYING =  0,  ///< No result yet
 IGSGAME_BWINS =    1,  ///< Black wins
 IGSGAME_WWINS =    2,  ///< White wins
 IGSGAME_ADJ =      3,  ///< Game adjourned
 IGSGAME_TIME =     4,  ///< Game won on time (no score)
 IGSGAME_RESIGN =   8   ///< Game won on resign (no score)
};

/** Class IgsGame */
class IgsGame {
  friend class Board;
 // friend class FlCells;

  Igs* igs;

 public:
  int pos;
  Board* board;
  bool observed;
  bool played;
  int times[2]; ///< time remaining for black and white player
  int nbmove[2];    ///< # moves remaining in Byo-Yomi
 // clock_t time_finished; ///< time when the game as finished
 // clock_t time_observers; ///< last time when we asked the observer list
  int time_finished; ///< time when the game as finished - seconds number
  int time_observers; ///< last time when we asked the observer list - seconds number

  char* whitename, *whiterank;
  char* blackname, *blackrank;
  int curpos, size, handi, BY;
  float komi, wanted_komi;
  int flags;
  int result;
  float result_score;
  int num;
  int nbobservers;

  IgsGame(Igs*, int n);
  ~IgsGame();

  void add_move(int pos, char player, int x, int y, int m = BOARD_OBSERVE);
  void undo();
  void observe();
  void unobserve();
  void init_board(int m);
  void set_komi(float new_komi);

  void set_name(char color, char* s);
  void set_whitename(char* s);
  void set_whiterank(char* s);
  void set_blackname(char* s);
  void set_blackrank(char* s);
};

#endif

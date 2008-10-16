//
//                   **** ViZiGO (C) 2000 Vincent Penne ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//
/** 
 *  @file proposegame.h
 *
 * ProposeGame class definition
 *
 */

#ifndef PROPOSEGAME_H
#define PROPOSEGAME_H

class ProposeGame;
class Igs;


/** class ProposeGame
 *
 * dialog box for game propositions
 *
 */
class ProposeGame {
  Igs* igs;

 public:
  ProposeGame* next; ///< next proposition
//  FlProposeGame* UI; ///< FLTK UI elements

  ProposeGame(Igs* i, char* wn, char* bn);
  ~ProposeGame();

  void swap();       ///< swap button callback
  void accept();     ///< accept button callback
  void decline();    ///< decline button callback
  void stats(const char* s); ///< ask stats for player
};

extern ProposeGame* cur_prop; ///< see cur_igs in "igs.h"

/*inline void gprop(Fl_Widget* w) ///< see gigs in "igs.h"
{
  while (w->parent()) w = w->parent();
  cur_prop = ((FlProposeGame*)w->user_data())->p;
}*/

#endif

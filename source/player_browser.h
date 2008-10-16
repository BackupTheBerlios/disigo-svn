//
//                   **** ViZiGO (C) 2000 Vincent Penne ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//
/** 
 *  @file player_browser.h
 *
 * PlayerBrowser class definition
 *
 */

#ifndef PLAYERBROWSER_H
#define PLAYERBROWSER_H

#include "igsplayer.h"

/**
 * class PlayerBrowser
 */
class PlayerBrowser {


 public:

  IgsPlayer* cur_player; ///< selected player
  IgsPlayer* top_player; ///< player on top of browser player
  IgsPlayer** players;   ///< sorted list of players
  int nbplayers;         ///< number of players

  PlayerBrowser();
  ~PlayerBrowser();
  
  void sort();           ///< sort the player list
  void click();          ///< click on player callback
  void select_player(IgsPlayer* p); ///< select a player
  void damage_player(IgsPlayer* p); ///< redraw a player
  
};

#endif // ifdef PLAYERBROWSER_H

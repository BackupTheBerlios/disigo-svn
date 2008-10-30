//
//                   **** ViZiGO (C) 2000 Vincent Penne ****
// This software if distributed under the GPL license (See the file LICENSE
// that must be included with this source)
//
/** 
 *  @file igs.h
 *
 * Igs class definition
 *
 */

#ifndef IGS_H
#define IGS_H

#define MAX_IGSGAMES 1000
#define MAX_CHANNELS 100

// Screen visible and screen button number
#define PLAYERS_SCREEN 11
#define GAMES_SCREEN 12
#define TERM_SCREEN 10
#define OPTIONS_SCREEN 13

#include "defs.h"
#include "connection.h"
#include "talk.h"
#include "igsplayer.h"
#include "proposegame.h"
#include "term.h"
// #include "debug.h"

class IgsGame;
class Channel;
class Board;

class Igs;


/**  Commands queue structure
*
* A linked list of object of this type will be used to store all commands
* that are not yet sent
*/
struct IgsCommand {
  IgsCommand* next;    ///< next command
  char* m;             ///< command string
};


typedef IgsCommand* PIgsCommand;

/** Class Igs
*
* This is the main class for an IGS connection
*/
class Igs {
  int cur_game;
  bool login_sent;
  IgsPlayer* cur_player;         ///< current player that we get stats from
  IgsPlayer* tmp_player;         ///< this player is not logged on
  IgsCommand* commands;
  volatile PIgsCommand read_commands;
  volatile int nb_read_commands;
  bool command_ready, will_be_ready;
  bool asked_my_stats, received_my_stats;
 // Fl_Thread read_thread;
 // Fl_Mutex read_command_mutex;
 // Fl_Mutex command_mutex;
  //Fl_Mutex connection_mutex;     ///< mutex used while creating/destroying the connection object

 public:
  static Igs* first;
  Igs* next;

	int screen; ///< visible screen
	Term* term;
 // FlIgs* UI;                     ///< the FLTK user interface
  IgsGame* games[MAX_IGSGAMES];  ///< list of all known games
  Channel* channels[MAX_CHANNELS];///< list of channels
  int played_game;               ///< number of the played game, 0=not playing
  Connection* cnx;               ///< connection
  Talk* talk;                    ///< linked list of talk windows
  Talk* last_talk;               ///< last person we talked to
  ProposeGame* propositions;     ///< linked list of game propositions
  char* myname;                  ///< username
  char* password;                ///< password given by the user, 
                                 ///< will be send after the login
  char* server;
  int port;
  char* cur_opponent;            ///< current opponent in komi bargain
  bool first_games_clicked;      ///< will refresh the list of games 
                                 ///< automatically the first time
  bool first_players_clicked;    ///< idem for players
  IgsPlayer** players;           ///< array of players
  int nbplayers, maxplayers;     ///< actual number of players and size
                                 ///< of the array in byte
  char* last_kibitz_player;      ///< name of the last player who kibitzed
  char* last_kibitz_rank;        ///< and his rank
  int last_kibitz_num;           ///< number of the game where happened the last kibitz
  bool nngs;                     ///< are we on an nngs server, or on igs ?
  bool connecting;               ///< indicate we are currently trying to connect (reading timeout callback should do nothing meanwhile)
  volatile bool thread_quit;     ///< thread quit request (set it to 1, when it is back to 0, the thread has quit)
  int observers_list;            ///< started receiving observers list
  float proba_as_white;
  float proba_handi;
  int your_rating;
  int their_rating;
  int file_mode;                 ///< indicate that we are receiving a file

  /** constructor
   * @param user username
   * @param pass password
   */
  Igs(const char* user, const char* pass, const char* server, int port, bool qnngs = 0);
  ~Igs();
  void read_callback();         ///< function called when some characters
                                ///< are received from igs (main thread)
  void read_thread_function();  ///< basic reading thread
  void read_VBL_function();  ///< basic reading thread
  void send(const char* s);     ///< send a message to igs, or put it in the 
                                ///< queue if igs is not ready
  void add_message(const char* m);///< display a message in the main terminal
  void parse(char* s);          ///< parse an igs command 
                                ///< (called by read_callback)
  void insert_game(IgsGame* g);
  void refresh_games();         ///< ask igs for the game list
  void refresh_players();       ///< ask igs for the player list
  void observe(int n);          ///< start/stop oberving game n
#ifdef CKM_STORED
  void Look( char * t );        ///< edit a stored game
  Board *lookb;
#endif
  void sort_players();          ///< sort the list of player and redraw it
  void user_command(const char* s); ///< send the user command s and display it
  void set_player_flags(char* s, IPFlags f); ///< set the flags of player s
  IgsPlayer* find_player(char* s); ///< return a pointer on player s
  void talk_cb();               ///< talk button callback
  void match_cb();              ///< match button callback
  void update_stats_cb();       ///< update stats button callback
  void update_player();         ///< update current player UI elements
  void go_channel(int n);       ///< go to channel n
  bool connect();               ///< try to connect to igs
  void exact_rating(IgsPlayer* p); ///< calculate exact rating of a player
  void create_connection();     ///< create a Connection object (thread safe)
  void destroy_connection();    ///< destroy a connection object (thread safe)

  /** find a game proposition 
   *
   * @param o name of the opponent
   *
   */
 ProposeGame* find_proposition(char* o);   
};

/** used by FLTK's callback, will contain the current igs connection */
extern Igs* cur_igs;

/** get which igs connection is associated to this widget, put it in cur_igs */
/*inline void gigs(Fl_Widget* w)
{
  while (w->parent()) w = w->parent();
  cur_igs = ((FlIgs*)w->user_data())->i;
}*/

/** compare two rank strings ("1d", "4k*", "NR", etc ... in increasing order)
 * @param sa first string
 * @param sb second string 
 */
int rank_compare(const char* sa, const char* sb);

#endif

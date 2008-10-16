#ifndef IGSPLAYER_H
#define IGSPLAYER_H


#include "defs.h"

class Igs;

enum IPFlags {
  IPF_CLOSED=0,
  IPF_DONTKNOW=1,
  IPF_OPEN=2,
  IPF_LOOKING=3,
  IPF_VERBOSE=4,
  IPF_BELL=8,
  IPF_QUIET=16,
  IPF_SHOUT=32,
  IPF_AUTOMAIL=64,
  IPF_CLIENT=128,
  IPF_KIBITZ=256,
  IPF_CHATTER=512
};

struct IgsPlayerResult {
  char winner[16];
  char winner_rank[4];
  char loser[16];
  char loser_rank[4];
  char result[11];
  char date[11];
  int handi;
  float komi;
  int size;
  bool rated;
};

class IgsPlayer {
 public:
  Igs* igs;
  char* name;
  char* rank;
  char* language;
  char* rank2;
  int ranki, rank2i;
  float rankadjust;
  int wins, losses;
  int ratedgames;
  char* idle;
  int* observed;
  int nbobserved;
  char* mail;
  char* reg_date;
  char* info;
  char* defaults;
  bool playing;
  bool logged;
  int flags;

#ifdef CKM_STORED
  int nbstored;
  int stored_size;
  int stored_pos;
  char *stored_buf;
  void add_stored( char *t );
#endif

  IgsPlayerResult* results;
  int maxresults, nbresults;

  IgsPlayer(char* n, char* r, Igs* i, IPFlags f=IPF_DONTKNOW);
  ~IgsPlayer();
  void set_flags(int f);
  
  void insert_result(IgsPlayerResult& r);
};

#endif

#include <stdlib.h>
#include <string.h>
#include "igsplayer.h"
#include "igs.h"
#include "binsert.h"

IgsPlayer::IgsPlayer(char* n, char* r, Igs* i, IPFlags f)
{
  name = strdup(n);
  rank = strdup(r);
  igs = i;
  language = 0;
  rank2 = 0;
  rank2i = ranki = -1;
  rankadjust = 0;
  ratedgames = 0, wins = 0, losses = 0, idle = 0;
  observed = 0;
  nbobserved = 0;
  mail = 0;
  reg_date = 0;
  info = 0;
  defaults = 0;
  playing = 0;
  set_flags(f);
  results = 0;
  maxresults = nbresults = 0;
  logged = 1;

#ifdef CKM_STORED
  nbstored = 0;
  stored_buf = NULL;
  stored_pos = 0;
  stored_size = 0;
#endif

#ifdef SEBOGO
  if (!strcmp(name, "Sebogo"))
    system("xmms /mnt/big3/zig/mp3/chris_isaak_wicked_game-wicked_game.m3u");
#endif
}

IgsPlayer::~IgsPlayer()
{
  free(name);
  free(rank);
  if (language) free(language);
  if (rank2) free(rank2);
  if (observed) free(observed);
  if (mail) free(mail);
  if (reg_date) free(reg_date);
  if (info) free(info);
  if (defaults) free(defaults);
  if (idle) free(idle);
  if (results) free(results);

#ifdef CKM_STORED

  if ( stored_buf )
    free( stored_buf );
/*
  while( --nbstored  >= 0 )
    if ( stored[ nbstored ] )
      free( stored[ nbstored ] );
*/
  
#endif
}

void IgsPlayer::set_flags(int f)
{
  flags = f;
  //igs->UI->players->damage_player(this);
}

static int cmp_results(const void* pa, const void* pb)
{
  IgsPlayerResult* a = (IgsPlayerResult*) pa;
  IgsPlayerResult* b = (IgsPlayerResult*) pb;
  int d = strcmp(a->date, b->date);
  if (d == 0) d = strcmp(a->winner, b->winner);
  if (d == 0) d = strcmp(a->loser, b->loser);
  return d;
}

void IgsPlayer::insert_result(IgsPlayerResult& r)
{
  binsert(&r, (const void**)&results, sizeof(IgsPlayerResult), &nbresults, &maxresults, cmp_results);
}

#ifdef CKM_STORED
void IgsPlayer::add_stored( char *t )
{
	
  if ( stored_buf ) {
    char *p = stored_buf;
    while( *p ) {
      if ( !strcmp( p,t ) )
	return;
      p += strlen(p) + 1;
    }
  }

  // not already in list
  int i = strlen(t);

  if ( !stored_buf ) {
    stored_buf = (char *)malloc(0x40);
    stored_pos = 0;
    stored_size = 0x40;
  }
	
  if ( stored_pos + i + 1 >= stored_size ) {
    stored_size += 0x40 + i;
    stored_size -= (stored_size & 0x3f);
    stored_buf = (char *)realloc( stored_buf, stored_size );
  }
	
  strcpy( stored_buf + stored_pos, t );
  stored_pos += i+1 ;
  stored_buf[ stored_pos ] = '\0';
  nbstored++;

  /*
    if ( nbstored < MAX_STORED )
    {
    for ( i = 0; i < nbstored; i++ )
    if ( !strcmp( stored[i], t ) )
    return;
	
    stored[ nbstored++ ] = strdup( t );
    }
  */

}

#endif

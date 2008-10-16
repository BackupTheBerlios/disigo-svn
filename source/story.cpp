#include <stdlib.h>
#include "story.h"

Story::Story(int px, int py, char pl, Story* ppast)
{
  past = ppast;
  x = px; y = py;
  player = pl;
  clear(1);
}

Story::~Story()
{
  clear();
}

void Story::clear(bool constructor)
{
  if (!constructor) {
    for (int i=0; i<nbvariants; i++) delete variants[i];
    free(variants);
    if (commentary) free(commentary);
  }

  variants = 0;
  current = nbvariants = 0;
  commentary = 0;
  score[0] = score[1] = 0;
}

void Story::add_move(Story* s)
{
  if (nbvariants)
    variants = (Story**) realloc(variants, sizeof(Story*)*(nbvariants+1));
  else
    variants = (Story**) malloc(sizeof(Story*));
  current = nbvariants;
  variants[nbvariants++] = s;
  s->past = this;
}

Story* Story::step()
{
  if (variants == 0) return 0;
  return variants[current];
}

Story* Story::back()
{
  return past;
}

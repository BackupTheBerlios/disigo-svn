#include "stone.h"
#include <PA9.h>
#include "gfx/all_gfx.h"
#include "sound_stone.h"
#include "defs.h"


void Stone::draw(int x, int y, char c)
{
	u16 sgfx;
	PA_Load3DSpritePal(1, (void*)stone_Pal); // Palette	
	 
  switch (c) {
  case BLACK:
    sgfx = PA_3DCreateTex((void*)blackstone_Texture, 16, 16, TEX_256COL);
    PA_3DCreateSpriteFromTex(x+18*y, sgfx, 16, 16,  1, x*10+71, y*10+7);
    PA_PlaySimpleSound(sound_stone);	
    break;
  case WHITE:
    sgfx = PA_3DCreateTex((void*)whitestone_Texture, 16, 16, TEX_256COL);
    PA_3DCreateSpriteFromTex(x+18*y, sgfx, 16, 16,  1, x*10+71, y*10+7); 
    PA_PlaySimpleSound(sound_stone);	
    break;
  case FRONTIER:
  
   case EMPTY:
    PA_3DDeleteSprite(x+18*y);
    break;
  default:
    break;
  }	
	
			
	
	
//	 PA_3DSetSpriteWidth(x+18*y,8);
//	PA_3DSetSpriteHeight(x+18*y,8);
	PA_3DSetSpriteFrame(x+18*y, 0);
	   
	//PA_3DSetSpriteAlpha(x+18*y, 15);
	//PA_3DSetSpritePolyID(x+18*y, 0);
	PA_3DProcess();  // Update sprites
	PA_WaitForVBL();
}


void Stone::set_size(int s)
{
  if (size==s) return;
  size = s;

}




Stone::~Stone()
{
 
}

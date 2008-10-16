#include "term.h"
#include <PA9.h>
#include "gfx/all_gfx.h"


Term::Term() {
	
	PA_ResetSpriteSys();
	PA_Init16cBg(1, 0);
	PA_LoadTiledBg(1, 1, bgterm);  
	PA_LoadTiledBg(0, 1, bg01term);  
	PA_LoadSpritePal(0, 1, (void*)pbutton_Pal); 
	lastline = 0;

	PA_CreateSprite(0, 10, (void*)term_Sprite, OBJ_SIZE_64X32, 1, 1, 0, 0);  
	PA_CreateSprite(0, 11, (void*)players_Sprite, OBJ_SIZE_64X32, 1, 1, 64, 0);  
	PA_CreateSprite(0, 12, (void*)games_Sprite, OBJ_SIZE_64X32, 1, 1, 128, 0);  
	PA_CreateSprite(0, 13, (void*)options_Sprite, OBJ_SIZE_64X32, 1, 1, 192, 0);  

	PA_SetSpriteAnim(0, 11, 1);
	PA_SetSpriteAnim(0, 12, 1);
	PA_SetSpriteAnim(0, 13, 1);
	
	PA_InitCustomKeyboard(0, keyboardcustom2); // Load the keyboard on background 2...
	PA_KeyboardIn(25, 95); // This scrolls the keyboard from the bottom, until it's at the right position
			
}

  void Term::add(char* s){
  	if (lastline < 200){
  	strcpy(lines[lastline],s);  	
  		lastline ++;
	}
	draw();
}


void Term::draw(){
	u8 i = 0;
	for (i = 0; i < lastline; i++){
		 PA_16cText(1, 0, i*10, 255, i*10+10, lines[i], 1, 1, 100);	 
	 }
	
}



Term::~Term()
{
 
}


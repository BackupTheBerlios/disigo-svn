#include "term.h"
#include <PA9.h>
#include "gfx/all_gfx.h"


Term::Term() {
	
	PA_ResetSpriteSys();
	PA_Init16cBg(1, 0);
	PA_LoadTiledBg(1, 1, bgterm1);  
	PA_LoadTiledBg(0, 1, bgterm0);  
	PA_LoadSpritePal(0, 1, (void*)pbutton_Pal); 
	lastline = 0;

	PA_CreateSprite(0, 10, (void*)term_Sprite, OBJ_SIZE_64X32, 1, 1, 0, 0);  
	PA_CreateSprite(0, 11, (void*)players_Sprite, OBJ_SIZE_64X32, 1, 1, 64, 0);  
	PA_CreateSprite(0, 12, (void*)games_Sprite, OBJ_SIZE_64X32, 1, 1, 128, 0);  
	PA_CreateSprite(0, 13, (void*)options_Sprite, OBJ_SIZE_64X32, 1, 1, 192, 0);  

	PA_SetSpriteAnim(0, 10, 1);
	
	PA_InitCustomKeyboard(0, keyboardcustom2); // Load the keyboard on background 2...
	PA_KeyboardIn(25, 95); // This scrolls the keyboard from the bottom, until it's at the right position
			
}

  void Term::add(char* s){
  	if (lastline == LINES-1) lastline = 0;
  	
  	strcpy(lines[lastline],s);  	 		
  	lastline ++;
	
	
	draw();
}


void Term::draw(){
	u8 i = 0;
	int nb;
	PA_16cClearZone(1, 0, 0, 256, 192);

	for (i = 0 ; i < 19; i++){
		if (lastline-19+i < 0) nb = LINES - 1 + lastline-19+i;
		else nb = lastline-19+i;
		 PA_16cText(1, 1, i*10+2, 255, i*10+12, lines[nb], 1, 1, 100);	 
	 }
	
}



Term::~Term()
{
 
}



#include <PA9.h>
#include "gfx/all_gfx.c"
#include "gfx/all_gfx.h"
#include "main_window.h"
#include "clop.h"
#include "board.h"

static void cb_New() {
 // new Board();
}

static void cb_Connect() {
//  igs_login();
}


void make_window() {

	s32 nletter[4]; // Next letter to right. 0 since no letters are there yet
	char letter = ' '; // New letter to write.
	u8 input = 0;
	
	char igs_username[15] = "";
	nletter[0] = 0;
	char igs_password[15] = "";
	nletter[1] = 0;
	char igs_server[20] = "igs.joyjoy.net";
	nletter[2] = 14;
	char igs_server_port[5] = "6969";
	nletter[3] = 4;
	
	s8 i = 0;
	u8 exit = 0;
	u8 gobanselected = 0;
	u8 igsselected = 1;
	
	PA_SetBrightness(0, -31); // all black
	PA_SetBrightness(1, -31); // all black	
	
 	PA_LoadTiledBg(1, 1, bgtitre);  
 	PA_LoadTiledBg(0, 1, bg01);  
 	PA_LoadSpritePal(0, 1, (void*)icon_igs_Pal); 
 	PA_LoadSpritePal(0, 2, (void*)icon_goban_Pal); 
 	PA_LoadSpritePal(0, 3, (void*)pbutton_Pal); 
 	
 	PA_SetTextCol(1,0,0,0);
 	
 	PA_CreateSprite(0, 1, (void*)icon_igs_Sprite, OBJ_SIZE_64X64, 1, 1, 58, 60);  
	PA_CreateSprite(0, 2, (void*)icon_goban_Sprite, OBJ_SIZE_64X64, 1, 2, 142, 60);
	PA_SetSpriteAnim(0, 1, 1);
	
	for(i = -31; i < 0 ; i++){  //transition
		PA_SetBrightness(0, i); 
		PA_SetBrightness(1, i); 
		PA_WaitForVBL();		   
	}  	
	
	while (!exit) {
		
	if ((Stylus.Newpress) & (PA_SpriteTouched(1)) & !igsselected){
		igsselected = 1;
		gobanselected = 0;
			PA_WaitForVBL();
			PA_UpdateStylus();
		PA_SetSpriteAnim(0, 2,  0);
		PA_SetSpriteAnim(0, 1,  1);
	}
		
	if ((Stylus.Newpress) & (PA_SpriteTouched(1)) & igsselected){
			gobanselected = 0;
			PA_SetSpriteAnim(0, 1, 1);
			
			for (i=0; i <  3*30; i++){
				PA_WaitForVBL();
			}
			
			PA_ResetSpriteSys();
			PA_Init16cBg(1, 0);
			PA_LoadTiledBg(1, 1, bgigs);  
			PA_InitCustomKeyboard(0, keyboardcustom2); // Load the keyboard on background 2...
			PA_KeyboardIn(25, 95); // This scrolls the keyboard from the bottom, until it's at the right position
			PA_CreateSprite(0, 3, (void*)connect_Sprite, OBJ_SIZE_64X32, 1, 3, 92, 30);
		//	PA_16cText(1, 105, 98, 255, 108, igs_server, 10, 1, 100);	
			//PA_16cText(1, 105, 122, 255, 128, igs_server_port, 10, 1, 100);	
			
			while ((!Pad.Held.Start) & (!PA_SpriteTouched(3))) {
						
				letter = PA_CheckKeyboard();	
						
				if ((letter > 31) & (nletter[input] < 14)) { // there is a new letter
					PA_PlaySimpleSound(clop);
					if (input == 0)	igs_username[nletter[input]] = letter;
					else if (input == 1) igs_password[nletter[input]] = letter;
					else if (input == 2) igs_server[nletter[input]] = letter;
					else if (input == 3) igs_server_port[nletter[input]] = letter;
					nletter[input]++;
				}
			
				else if ((letter == PA_BACKSPACE)&&nletter) { // Backspace pressed
					PA_PlaySimpleSound(clop);
					nletter[input]--;
					if (input == 0)	igs_username[nletter[input]] =  ' '; // Erase the last letter
					else if (input == 1) igs_password[nletter[input]] = ' ';
					else if (input == 2) igs_server[nletter[input]] = ' ';
					else if (input == 3) igs_server_port[nletter[input]] = ' ';
				}
				else if (letter == '\n'){ // Enter pressed
					PA_PlaySimpleSound(clop);
					if (input < 3) input ++;	
					else input = 0;
				}
				
				PA_16cClearZone (1, 105, 48, 255, 192);
				PA_16cText(1, 105, 49, 255, 58, igs_username, 1, 1, 100);	
				PA_16cText(1, 105, 73, 255, 88, igs_password, 1, 1, 100);
				PA_16cText(1, 105, 97, 255, 108, igs_server, 1, 1, 100);	
				PA_16cText(1, 105, 122, 255, 132, igs_server_port, 1, 1, 100);	

				PA_WaitForVBL();
			}
			PA_SetSpriteAnim(0, 3,  1);
			PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
			new Igs(igs_username, igs_password, igs_server, (int) igs_server_port, 0);	
			
		}
		
		if ((Stylus.Newpress) & (PA_SpriteTouched(2)) & !gobanselected){
			gobanselected = 1;
			igsselected = 0;
			
			PA_WaitForVBL();
			PA_UpdateStylus();
			PA_SetSpriteAnim(0, 2,  1);
			PA_SetSpriteAnim(0, 1,  0);
			
		
			
		}
		if ((Stylus.Newpress) & (PA_SpriteTouched(2)) & gobanselected){
			new Board(0);
		}
			
	}
}

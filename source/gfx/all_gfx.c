//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .c, for easier inclusion in a project

#ifdef __cplusplus
extern "C" {
#endif

#include "all_gfx.h"


// Sprite files : 
#include "icon_goban.c"
#include "icon_igs.c"
#include "options.c"
#include "players.c"
#include "games.c"
#include "term.c"
#include "connect.c"

// Background files : 
#include "bgtitre.c"
#include "board19.c"
#include "bgtitre2.c"
#include "bgscoreigs.c"
#include "bg01.c"
#include "bg01term.c"
#include "bgigs.c"
#include "bgterm.c"
#include "keyboardcustom2.c"

// Sprite files : 
#include "pointer.c"
#include "blackstone.c"
#include "whitestone.c"
#include "pass.c"

// Palette files : 
#include "icon_goban.pal.c"
#include "icon_igs.pal.c"
#include "pbutton.pal.c"
#include "bgtitre.pal.c"
#include "board19.pal.c"
#include "bgtitre2.pal.c"
#include "bgscoreigs.pal.c"
#include "bg01.pal.c"
#include "bg01term.pal.c"
#include "bgigs.pal.c"
#include "bgterm.pal.c"
#include "keyboardcustom2.pal.c"
#include "pointer.pal.c"
#include "stone.pal.c"
#include "buttons.pal.c"

// Background Pointers :
PAGfx_struct bgtitre = {(void*)bgtitre_Map, 768, (void*)bgtitre_Tiles, 41856, (void*)bgtitre_Pal, (int*)bgtitre_Info };
PAGfx_struct board19 = {(void*)board19_Map, 768, (void*)board19_Tiles, 49152, (void*)board19_Pal, (int*)board19_Info };
PAGfx_struct bgtitre2 = {(void*)bgtitre2_Map, 768, (void*)bgtitre2_Tiles, 49152, (void*)bgtitre2_Pal, (int*)bgtitre2_Info };
PAGfx_struct bgscoreigs = {(void*)bgscoreigs_Map, 768, (void*)bgscoreigs_Tiles, 26880, (void*)bgscoreigs_Pal, (int*)bgscoreigs_Info };
PAGfx_struct bg01 = {(void*)bg01_Map, 768, (void*)bg01_Tiles, 49152, (void*)bg01_Pal, (int*)bg01_Info };
PAGfx_struct bg01term = {(void*)bg01term_Map, 768, (void*)bg01term_Tiles, 46144, (void*)bg01term_Pal, (int*)bg01term_Info };
PAGfx_struct bgigs = {(void*)bgigs_Map, 768, (void*)bgigs_Tiles, 46144, (void*)bgigs_Pal, (int*)bgigs_Info };
PAGfx_struct bgterm = {(void*)bgterm_Map, 768, (void*)bgterm_Tiles, 320, (void*)bgterm_Pal, (int*)bgterm_Info };
PAGfx_struct keyboardcustom2 = {(void*)keyboardcustom2_Map, 4096, (void*)keyboardcustom2_Tiles, 39488, (void*)keyboardcustom2_Pal, (int*)keyboardcustom2_Info };


#ifdef __cplusplus
}
#endif


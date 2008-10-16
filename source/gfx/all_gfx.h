//Gfx converted using Mollusk's PAGfx Converter

//This file contains all the .h, for easier inclusion in a project

#ifndef ALL_GFX_H
#define ALL_GFX_H

#ifndef PAGfx_struct
    typedef struct{
    void *Map;
    int MapSize;
    void *Tiles;
    int TileSize;
    void *Palette;
    int *Info;
} PAGfx_struct;
#endif


// Sprite files : 
extern const unsigned char icon_goban_Sprite[8192] __attribute__ ((aligned (4))) ;  // Pal : icon_goban_Pal
extern const unsigned char icon_igs_Sprite[8192] __attribute__ ((aligned (4))) ;  // Pal : icon_igs_Pal
extern const unsigned char options_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : pbutton_Pal
extern const unsigned char players_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : pbutton_Pal
extern const unsigned char games_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : pbutton_Pal
extern const unsigned char term_Sprite[4096] __attribute__ ((aligned (4))) ;  // Pal : pbutton_Pal

// Background files : 
extern const int bgtitre_Info[3]; // BgMode, Width, Height
extern const unsigned short bgtitre_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bgtitre_Pal
extern const unsigned char bgtitre_Tiles[41856] __attribute__ ((aligned (4))) ;  // Pal : bgtitre_Pal
extern PAGfx_struct bgtitre; // background pointer

extern const int board19_Info[3]; // BgMode, Width, Height
extern const unsigned short board19_Map[768] __attribute__ ((aligned (4))) ;  // Pal : board19_Pal
extern const unsigned char board19_Tiles[49152] __attribute__ ((aligned (4))) ;  // Pal : board19_Pal
extern PAGfx_struct board19; // background pointer

extern const int bgtitre2_Info[3]; // BgMode, Width, Height
extern const unsigned short bgtitre2_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bgtitre2_Pal
extern const unsigned char bgtitre2_Tiles[49152] __attribute__ ((aligned (4))) ;  // Pal : bgtitre2_Pal
extern PAGfx_struct bgtitre2; // background pointer

extern const int bgscoreigs_Info[3]; // BgMode, Width, Height
extern const unsigned short bgscoreigs_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bgscoreigs_Pal
extern const unsigned char bgscoreigs_Tiles[26880] __attribute__ ((aligned (4))) ;  // Pal : bgscoreigs_Pal
extern PAGfx_struct bgscoreigs; // background pointer

extern const int bg01_Info[3]; // BgMode, Width, Height
extern const unsigned short bg01_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bg01_Pal
extern const unsigned char bg01_Tiles[49152] __attribute__ ((aligned (4))) ;  // Pal : bg01_Pal
extern PAGfx_struct bg01; // background pointer

extern const int bg01term_Info[3]; // BgMode, Width, Height
extern const unsigned short bg01term_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bg01term_Pal
extern const unsigned char bg01term_Tiles[49088] __attribute__ ((aligned (4))) ;  // Pal : bg01term_Pal
extern PAGfx_struct bg01term; // background pointer

extern const int bgigs_Info[3]; // BgMode, Width, Height
extern const unsigned short bgigs_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bgigs_Pal
extern const unsigned char bgigs_Tiles[46144] __attribute__ ((aligned (4))) ;  // Pal : bgigs_Pal
extern PAGfx_struct bgigs; // background pointer

extern const int bgterm_Info[3]; // BgMode, Width, Height
extern const unsigned short bgterm_Map[768] __attribute__ ((aligned (4))) ;  // Pal : bgterm_Pal
extern const unsigned char bgterm_Tiles[448] __attribute__ ((aligned (4))) ;  // Pal : bgterm_Pal
extern PAGfx_struct bgterm; // background pointer

extern const int keyboardcustom2_Info[3]; // BgMode, Width, Height
extern const unsigned short keyboardcustom2_Map[4096] __attribute__ ((aligned (4))) ;  // Pal : keyboardcustom2_Pal
extern const unsigned char keyboardcustom2_Tiles[39488] __attribute__ ((aligned (4))) ;  // Pal : keyboardcustom2_Pal
extern PAGfx_struct keyboardcustom2; // background pointer


// Sprite files : 
extern const unsigned char pointer_Texture[256] __attribute__ ((aligned (4))) ;  // Pal : pointer_Pal
extern const unsigned char blackstone_Texture[768] __attribute__ ((aligned (4))) ;  // Pal : stone_Pal
extern const unsigned char whitestone_Texture[768] __attribute__ ((aligned (4))) ;  // Pal : stone_Pal
extern const unsigned char pass_Texture[4096] __attribute__ ((aligned (4))) ;  // Pal : buttons_Pal

// Palette files : 
extern const unsigned short icon_goban_Pal[224] __attribute__ ((aligned (4))) ;
extern const unsigned short icon_igs_Pal[196] __attribute__ ((aligned (4))) ;
extern const unsigned short pbutton_Pal[178] __attribute__ ((aligned (4))) ;
extern const unsigned short bgtitre_Pal[205] __attribute__ ((aligned (4))) ;
extern const unsigned short board19_Pal[65] __attribute__ ((aligned (4))) ;
extern const unsigned short bgtitre2_Pal[186] __attribute__ ((aligned (4))) ;
extern const unsigned short bgscoreigs_Pal[172] __attribute__ ((aligned (4))) ;
extern const unsigned short bg01_Pal[28] __attribute__ ((aligned (4))) ;
extern const unsigned short bg01term_Pal[30] __attribute__ ((aligned (4))) ;
extern const unsigned short bgigs_Pal[134] __attribute__ ((aligned (4))) ;
extern const unsigned short bgterm_Pal[5] __attribute__ ((aligned (4))) ;
extern const unsigned short keyboardcustom2_Pal[79] __attribute__ ((aligned (4))) ;
extern const unsigned short pointer_Pal[2] __attribute__ ((aligned (4))) ;
extern const unsigned short stone_Pal[123] __attribute__ ((aligned (4))) ;
extern const unsigned short buttons_Pal[88] __attribute__ ((aligned (4))) ;


#endif


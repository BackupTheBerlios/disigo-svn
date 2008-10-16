#include "pointer.h"
#include <PA9.h>
#include "gfx/all_gfx.h"


Pointer::Pointer() {
	

	
	size = 16; 
	xpointer = 9; // goban center
	ypointer = 9;
	button = 0;
	
		u16 pointergfx;
		
	PA_Load3DSpritePal(0, (void*)pointer_Pal); // Palette	
	pointergfx = PA_3DCreateTex((void*)pointer_Texture, 16, 16, TEX_256COL);		
	PA_3DCreateSpriteFromTex(370, pointergfx, 16, 16,  0, xpointer*10+71, ypointer*10+6);
	
	PA_3DProcess();  // Update sprites
	PA_WaitForVBL();

}

void Pointer::draw(){
	
}


void Pointer::set_size(int s)
{
  if (size==s) return;
  size = s;

}

void Pointer::move()
{
	int selected = 0;
	 while (!selected){
			if (Pad.Held.Right & xpointer < 18) {
				xpointer = xpointer + 1;
				PA_3DSetSpriteXY(370, xpointer*10+71, ypointer*10+6);
				PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
				PA_3DProcess(); 
				PA_WaitForVBL();
			}
			if (Pad.Held.Left & xpointer > 0){
				xpointer = xpointer - 1;
				PA_3DSetSpriteXY(370, xpointer*10+71, ypointer*10+6);
				PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
				PA_3DProcess(); 
				PA_WaitForVBL();
			}
				if (Pad.Held.Down & ypointer < 18){
				ypointer = ypointer + 1;
				PA_3DSetSpriteXY(370, xpointer*10+71, ypointer*10+6);
				PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
				PA_3DProcess(); 
				PA_WaitForVBL();
			}
				if (Pad.Held.Up & ypointer > 0){
				ypointer = ypointer - 1;
				PA_3DSetSpriteXY(370, xpointer*10+71, ypointer*10+6);
				PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();PA_WaitForVBL();
				PA_3DProcess(); 
				PA_WaitForVBL();
			}
			if (Pad.Newpress.A ){
				selected = 1;
				
			}
			
			if ((Stylus.Newpress) & (Stylus.X > 64)) {
					PA_UpdateStylus();
			if ((xpointer == (Stylus.X-66)/10) & (ypointer == Stylus.Y/10)) { //double clic
					selected = 1;
			}
			else  {
				ypointer = Stylus.Y/10;
				xpointer = (Stylus.X-66)/10;
				PA_3DSetSpriteXY(370, xpointer*10+71, ypointer*10+6);
				PA_WaitForVBL();
				PA_3DProcess(); 
				PA_WaitForVBL();
			}
		}
			
			
}
}


Pointer::~Pointer()
{
 
}

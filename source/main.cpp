
//***********
// INCLUDES *
//***********

#include <PA9.h>


#include "clop.h"
#include "connection.h"
#include "main_window.h"
#include "term.h"

//*************
// STRUCTURES *
//*************



//*********************
// VARIABLES GLOBALES *
//*********************


Connection* cnx; 


//****************************
// DECLARATIONS DE FONCTIONS *
//****************************


//****************
// MAIN FONCTION *
//****************

int main(void)	{

	
	PA_Init();
	PA_InitVBL();	
	AS_Init(8); // Initialisation du systeme du son
	PA_InitRand();
	

	u8 i = 0;
   


	//PA_Init16cBg(1, 0);

	make_window();
	
	//	cnx = new Connection;
//	PA_16cText(1, 0, 0, 255, 192, "--", 1, 1, 100);

//	PA_LoadPal16(PAL_BG1, 0, chancery_Pal);
//	int connectedclient = 0 ;

/*	connectedclient = cnx->open("igs.joyjoy.net", 6969, term);
if (connectedclient) term->add("Connect");
else term->add("Not connect");
	*/
	

  

	
	// Boucle infinie pour mas32enir le programme en fonction
	while(1) {	
		
 
	 
		for (i=0; i <  2*30; i++){
			PA_WaitForVBL();
		}
     
    
//PA_16cText(1, 10, 0, 255, 192, ps, 1, 1, 100);	
	   

		
		
	   
			//	PA_16cText(0, 100, 138, 255, 192, debugtext, 1, 6, 100);	
	
	} //Fin boucle infinie
	
	return 0;

}






//************
// FONCTIONS *
//************


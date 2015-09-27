#include "tm4c123gh6pm.h"
#include "Screen.h"
#include "Bitmaps.h"
#include "Character.h"
#include "AI.h"
#include "Random.h"

#define ai1 Characters[p2]
#define ai2 Characters[p3]
#define ai3 Characters[p4]

// ******************** Functions *********************
//------------AI_Init------------
// Initialize AI by setting up character struct
// Input: uint8_t containing bits of AI to be initialized (i.e. 0x01 for ai1, 0x02 for ai2, 0x04 for ai3, etc.)
// Output: none
void AI_Init(uint8_t aiSelect){
	if(aiSelect&0x01){
		ai1.bmp=&Sprite[2];								// Define sprite for ai1
		ai1.x=139;												// Define starting position
		ai1.y=130;
		ai1.height=(*ai1.bmp).height;
		ai1.width=(*ai1.bmp).width;
		ai1.spd=2;												// Define movement speed
		ai1.mass=9001;
	}
	if(aiSelect&0x02){
		ai2.bmp=&Sprite[6];								// Define sprite for ai2
		ai2.x=200;												// Define starting position
		ai2.y=200;
		ai2.height=(*ai2.bmp).height;
		ai2.width=(*ai2.bmp).width;
		ai2.spd=3;												// Define movement speed
		ai2.mass=5;
	}
	if(aiSelect&0x04){
		ai3.bmp=&Sprite[4];								// Define sprite for ai3
		ai3.x=7;													// Define starting position
		ai3.y=180;
		ai3.height=(*ai3.bmp).height;
		ai3.width=(*ai3.bmp).width;
		ai3.spd=1;												// Define movement speed
		ai3.mass=4;
	}
}
//------------AI_Update------------
// Decide which which AI each computer character will follow
// Input: none
// Output: none
void AI_Update(){
	AI_AI1(&ai1), AI_AI1(&ai2), AI_AI1(&ai3);
}
//------------AI_AI1------------
// Decide Moves AI randomly, accounting for collisions
// Input: AI character pointer
// Output: none
void AI_AI1(Character *data){
	switch (Random()%4){
		case 0: 
			if(!((*data).cState==1 || (*data).cState==2 || (*data).cState==3))
				
				(*data).x -= (*data).spd;
			break;
		case 1: 
			if(!((*data).cState==5 || (*data).cState==6 || (*data).cState==7))
				(*data).x += (*data).spd;
			break;
		case 2: 
			if(!((*data).cState==3 || (*data).cState==4 || (*data).cState==5))
				(*data).y -= (*data).spd;
			break;
		case 3: 
			if(!((*data).cState==1 || (*data).cState==7 || (*data).cState==8))
				(*data).y += (*data).spd;
			break;
	}
	if((*data).potato > 1)
		(*data).potato--;
}

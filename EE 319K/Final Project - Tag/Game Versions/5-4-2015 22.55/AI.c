#include "tm4c123gh6pm.h"
#include "Screen.h"
#include "Bitmaps.h"
#include "Character.h"
#include "AI.h"
#include "Random.h"
#include "HotPotato.h"
#include <stdlib.h>

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
		ai1.active=1;
		ai1.hp=200;
	}
	if(aiSelect&0x02){
		ai2.bmp=&Sprite[6];								// Define sprite for ai2
		ai2.x=200;												// Define starting position
		ai2.y=200;
		ai2.height=(*ai2.bmp).height;
		ai2.width=(*ai2.bmp).width;
		ai2.spd=3;												// Define movement speed
		ai2.mass=5;
		ai2.active=1;
		ai2.hp=200;
	}
	if(aiSelect&0x04){
		ai3.bmp=&Sprite[4];								// Define sprite for ai3
		ai3.x=7;													// Define starting position
		ai3.y=180;
		ai3.height=(*ai3.bmp).height;
		ai3.width=(*ai3.bmp).width;
		ai3.spd=1;												// Define movement speed
		ai3.mass=4;
		ai3.active=1;
		ai3.hp=200;
	}
}
//------------AI_Update------------
// Decide which which AI each computer character will follow
// Input: none
// Output: none
void AI_Update(){
	AI_AI2(&ai1), AI_AI2(&ai2), AI_AI2(&ai3);
}
//------------AI_AI1------------
// Decide Moves AI randomly, accounting for collisions
// Input: AI character pointer
// Output: none
void AI_AI1(Character *data){
	switch (Random()%4){
		case 0: 
			if(!((*data).cStateP==1 || (*data).cStateP==2 || (*data).cStateP==3))
				if(!((*data).cStateS==1 || (*data).cStateS==2 || (*data).cStateS==3))		
					(*data).x -= (*data).spd;
			break;
		case 1: 
			if(!((*data).cStateP==5 || (*data).cStateP==6 || (*data).cStateP==7))
				if(!((*data).cStateS==5 || (*data).cStateS==6 || (*data).cStateS==7))
					(*data).x += (*data).spd;
			break;
		case 2: 
			if(!((*data).cStateP==3 || (*data).cStateP==4 || (*data).cStateP==5))
				if(!((*data).cStateS==3 || (*data).cStateS==4 || (*data).cStateS==5))
					(*data).y -= (*data).spd;
			break;
		case 3: 
			if(!((*data).cStateP==1 || (*data).cStateP==7 || (*data).cStateP==8))
				if(!((*data).cStateS==1 || (*data).cStateS==7 || (*data).cStateS==8))
					(*data).y += (*data).spd;
			break;
	}
}
//------------AI_AI2------------
// Runs away from "it"; if "it, chases nearest character
// Input: AI character pointer
// Output: none
void AI_AI2(Character *data){
		int it = HotPotato_WhosIt();
		if(data == &Characters[it]){						// This AI is it
			int min;
			int minIndex = -1;
			for(int i = 0; i<characterCount; i++)	// Get's the index of the closest target
				if(abs((*data).x-Characters[i].x)+abs((*data).y-Characters[i].y) < min && &Characters[i] != data){
						min = abs((*data).x-Characters[i].x) + abs((*data).y-Characters[i].y);
						minIndex=i;
					}
			if(Characters[minIndex].x < (*data).x)
				if(!((*data).cStateP==1 || (*data).cStateP==2 || (*data).cStateP==3))
					if(!((*data).cStateS==1 || (*data).cStateS==2 || (*data).cStateS==3))
						(*data).x -= (*data).spd;
			if(Characters[minIndex].x > (*data).x)
				if(!((*data).cStateP==5 || (*data).cStateP==6 || (*data).cStateP==7))
					if(!((*data).cStateS==5 || (*data).cStateS==6 || (*data).cStateS==7))
						(*data).x += (*data).spd;
			if(Characters[minIndex].y < (*data).y)
				if(!((*data).cStateP==3 || (*data).cStateP==4 || (*data).cStateP==5))
					if(!((*data).cStateS==3 || (*data).cStateS==4 || (*data).cStateS==5))
						(*data).y -= (*data).spd;
			if(Characters[minIndex].y > (*data).y)
				if(!((*data).cStateP==1 || (*data).cStateP==7 || (*data).cStateP==8))
					if(!((*data).cStateS==1 || (*data).cStateS==7 || (*data).cStateS==8))
						(*data).y += (*data).spd;
	}
		else{																// This AI is not it
			if(Characters[it].x > (*data).x)
				if(!((*data).cStateP==1 || (*data).cStateP==2 || (*data).cStateP==3))
					if(!((*data).cStateS==1 || (*data).cStateS==2 || (*data).cStateS==3))
						(*data).x -= (*data).spd;
			if(Characters[it].x < (*data).x)
				if(!((*data).cStateP==5 || (*data).cStateP==6 || (*data).cStateP==7))
					if(!((*data).cStateS==5 || (*data).cStateS==6 || (*data).cStateS==7))
						(*data).x += (*data).spd;
			if(Characters[it].y > (*data).y)
				if(!((*data).cStateP==3 || (*data).cStateP==4 || (*data).cStateP==5))
					if(!((*data).cStateS==3 || (*data).cStateS==4 || (*data).cStateS==5))
						(*data).y -= (*data).spd;
			if(Characters[it].y < (*data).y)
				if(!((*data).cStateP==1 || (*data).cStateP==7 || (*data).cStateP==8))
					if(!((*data).cStateS==1 || (*data).cStateS==7 || (*data).cStateS==8))
						(*data).y += (*data).spd;
	}
}

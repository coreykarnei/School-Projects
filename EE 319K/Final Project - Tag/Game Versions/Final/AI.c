#include "tm4c123gh6pm.h"
#include "Screen.h"
#include "Bitmaps.h"
#include "Character.h"
#include "AI.h"
#include "Random.h"
#include "HotPotato.h"
#include <stdlib.h>
#include "TwoPlayer.h"

#define ai1 Characters[p2]
#define ai2 Characters[p3]
#define ai3 Characters[p4]

short aiCount = 1;
short aiDifficulty=2;
// ******************** Functions *********************
//------------AI_Init------------
// Initialize AI by setting up character structs
// Input: number of ai to initialize and their difficulty level
// Output: none
void AI_Init(void){
	for(int i=1; i <= characterCount; i++){
		Characters[i].bmp= &Sprite[Random()%(spriteCount-1)];						// Define sprite for Characters[i]
		Characters[i].x= Random32()%(Map[currentMap].width*mapScale-(*Characters[i].bmp).width);	// Define starting position
		Characters[i].y= Random32()%(Map[currentMap].height*mapScale-(*Characters[i].bmp).height)+(*Characters[i].bmp).height;
		Characters[i].height= (*Characters[i].bmp).height;
		Characters[i].width= (*Characters[i].bmp).width;
	  Characters[i].spd=Random()%3+2;															// Define movement speed
		Characters[i].mass=5;
		Characters[i].active=1;
		if(i > aiCount) Characters[i].active=0;
		Characters[i].hp=80;
		Characters[i].difficulty=aiDifficulty;
	}
}
//------------AI_Update------------
// Decide which which AI each computer character will follow
// Input: none
// Output: none
void AI_AI3(Character *data);
void AI_Update(){
	for(int i=1; i <= characterCount; i++)
		if(Characters[i].active & !(multiplayer > 0 && i==1)){
			/*if(Characters[i].difficulty == 1)
				AI_AI1(&Characters[i]);
			else if(Characters[i].difficulty == 2)
				AI_AI3(&Characters[i]);
			else if(Characters[i].difficulty == 3)
				AI_AI4(&Characters[i]);*/
			if(aiDifficulty == 1)
				AI_AI1(&Characters[i]);
			else if(aiDifficulty == 2)
				AI_AI3(&Characters[i]);
			else if(aiDifficulty == 3)
				AI_AI4(&Characters[i]);
		}
}
//------------AI_AI1------------
// Decide Moves AI randomly, accounting for collisions
// Input: AI character pointer
// Output: none
void AI_AI1(Character *data){
	switch (rand()%4){
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
// Runs away from "it" - If "it", chases nearest character
// Input: AI character pointer
// Output: none
void AI_AI2(Character *data){
		int it = HotPotato_WhosIt();
		if(data == &Characters[it]){						// This AI is it
			int min;
			int minIndex = -1;
			for(int i = 0; i<characterCount; i++)	// Get's the index of the closest target
				if(abs((*data).x-Characters[i].x)+abs((*data).y-Characters[i].y) < min && &Characters[i] != data && Characters[i].active){
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
//------------AI_AI3------------
// Runs away from "it" - If "it", chases nearest character (velocity based)
// Input: AI character pointer
// Output: none
void AI_AI3(Character *data){
		int it = HotPotato_WhosIt();
		if(data == &Characters[it]){						// This AI is it
			int min;
			int minIndex = -1;
			for(int i = 0; i<characterCount; i++)	// Get's the index of the closest target
				if(abs((*data).x-Characters[i].x)+abs((*data).y-Characters[i].y) < min && &Characters[i] != data && Characters[i].active){
						min = abs((*data).x-Characters[i].x) + abs((*data).y-Characters[i].y);
						minIndex=i;
					}
			if(Characters[minIndex].x < (*data).x){										// escape to left
				if((*data).cStateP==1 || (*data).cStateP==2 || (*data).cStateP==3 || 	// is a something is left, stop leftward momentum
					 ((*data).cStateS==1 || (*data).cStateS==2 || (*data).cStateS==3))		
					(*data).velocityX = 0;
				else if((*data).velocityX > -(*data).spd)															// otherwise move leftward velocity towards max speed
						(*data).velocityX--;
			}
			else if(Characters[minIndex].x > (*data).x){ 							// escape to right
				if(((*data).cStateP==5 || (*data).cStateP==6 || (*data).cStateP==7) || // is a something is right, stop rightward momentum
					 ((*data).cStateS==5 || (*data).cStateS==6 || (*data).cStateS==7))		
					(*data).velocityX = 0;
				else if((*data).velocityX < (*data).spd)																// otherwise move rightward velocity towards max speed
							(*data).velocityX ++;
			}
			// Move velocity towards 0 when no input
			else if((*data).velocityX > 0)
				(*data).velocityX--;
			else if((*data).velocityX < 0)
				(*data).velocityX++;
			
			if(Characters[minIndex].y < (*data).y){ 									// escape to up
				if((*data).cStateP==3 || (*data).cStateP==4 || (*data).cStateP==5 || 	// is a something is up, stop upward momentum
					 ((*data).cStateS==3 || (*data).cStateS==4 || (*data).cStateS==5))		
					(*data).velocityY = 0;
				else if((*data).velocityY > -(*data).spd)															// otherwise move upward velocity towards max speed
						(*data).velocityY--;
			}
			else if(Characters[minIndex].y > (*data).y){ 							// escape to down			
				if((*data).cStateP==1 || (*data).cStateP==7 || (*data).cStateP==8 || 	// is a something is down, stop downward momentum
					 ((*data).cStateS==1 || (*data).cStateS==7 || (*data).cStateS==8))	
					(*data).velocityY = 0;
				else if((*data).velocityY < (*data).spd)															// otherwise move downward velocity towards max speed
						(*data).velocityY++;
			}
			// Move velocity towards 0 when no input
			else if((*data).velocityY > 0)
				(*data).velocityY--;
			else if((*data).velocityY < 0)
				(*data).velocityY++;
	}
		else{																// This AI is not it
			// X-direction escaping
			if(Characters[it].x > (*data).x){										// escape to left
				if((*data).cStateP==1 || (*data).cStateP==2 || (*data).cStateP==3 || 	// is a something is left, stop leftward momentum
					 ((*data).cStateS==1 || (*data).cStateS==2 || (*data).cStateS==3))		
					(*data).velocityX = 0;
				else if((*data).velocityX > -(*data).spd)															// otherwise move leftward velocity towards max speed
						(*data).velocityX--;
			}
			else if(Characters[it].x < (*data).x){ 							// escape to right
				if(((*data).cStateP==5 || (*data).cStateP==6 || (*data).cStateP==7) || // is a something is right, stop rightward momentum
					 ((*data).cStateS==5 || (*data).cStateS==6 || (*data).cStateS==7))		
					(*data).velocityX = 0;
				else if((*data).velocityX < (*data).spd)																// otherwise move rightward velocity towards max speed
							(*data).velocityX ++;
			}
			// Move velocity towards 0 when no input
			else if((*data).velocityX > 0)
				(*data).velocityX--;
			else if((*data).velocityX < 0)
				(*data).velocityX++;
			// Y-direction escaping
			if(Characters[it].y > (*data).y){ 									// escape to up
				if((*data).cStateP==3 || (*data).cStateP==4 || (*data).cStateP==5 || 	// is a something is up, stop upward momentum
					 ((*data).cStateS==3 || (*data).cStateS==4 || (*data).cStateS==5))		
					(*data).velocityY = 0;
				else if((*data).velocityY > -(*data).spd)															// otherwise move upward velocity towards max speed
						(*data).velocityY--;
			}
			else if(Characters[it].y < (*data).y){ 							// escape to down			
				if((*data).cStateP==1 || (*data).cStateP==7 || (*data).cStateP==8 || 	// is a something is down, stop downward momentum
					 ((*data).cStateS==1 || (*data).cStateS==7 || (*data).cStateS==8))	
					(*data).velocityY = 0;
				else if((*data).velocityY < (*data).spd)															// otherwise move downward velocity towards max speed
						(*data).velocityY++;
			}
			// Move velocity towards 0 when no input
			else if((*data).velocityY > 0)
				(*data).velocityY--;
			else if((*data).velocityY < 0)
				(*data).velocityY++;
	}
	(*data).x+=(*data).velocityX;
	(*data).y+=(*data).velocityY;
}
//------------AI_AI4------------
// Runs away from "it" - If "it", chases player (velocity based)
// Input: AI character pointer
// Output: none
void AI_AI4(Character *data){
		int it = HotPotato_WhosIt();
		if(data == &Characters[it]){						// This AI is it
			if(Characters[0].x < (*data).x){										// chase left
				if((*data).cStateP==1 || (*data).cStateP==2 || (*data).cStateP==3 || 	// is a something is left, stop leftward momentum
					 ((*data).cStateS==1 || (*data).cStateS==2 || (*data).cStateS==3))		
					(*data).velocityX = 0;
				else if((*data).velocityX > -(*data).spd)															// otherwise move leftward velocity towards max speed
						(*data).velocityX--;
			}
			else if(Characters[0].x > (*data).x){ 							// chase right
				if(((*data).cStateP==5 || (*data).cStateP==6 || (*data).cStateP==7) || // is a something is right, stop rightward momentum
					 ((*data).cStateS==5 || (*data).cStateS==6 || (*data).cStateS==7))		
					(*data).velocityX = 0;
				else if((*data).velocityX < (*data).spd)																// otherwise move rightward velocity towards max speed
							(*data).velocityX ++;
			}
			// Move velocity towards 0 when no input
			else if((*data).velocityX > 0)
				(*data).velocityX--;
			else if((*data).velocityX < 0)
				(*data).velocityX++;
			
			if(Characters[0].y < (*data).y){ 									// chase up
				if((*data).cStateP==3 || (*data).cStateP==4 || (*data).cStateP==5 || 	// is a something is up, stop upward momentum
					 ((*data).cStateS==3 || (*data).cStateS==4 || (*data).cStateS==5))		
					(*data).velocityY = 0;
				else if((*data).velocityY > -(*data).spd)															// otherwise move upward velocity towards max speed
						(*data).velocityY--;
			}
			else if(Characters[0].y > (*data).y){ 							// chase down		
				if((*data).cStateP==1 || (*data).cStateP==7 || (*data).cStateP==8 || 	// is a something is down, stop downward momentum
					 ((*data).cStateS==1 || (*data).cStateS==7 || (*data).cStateS==8))	
					(*data).velocityY = 0;
				else if((*data).velocityY < (*data).spd)															// otherwise move downward velocity towards max speed
						(*data).velocityY++;
			}
			// Move velocity towards 0 when no input
			else if((*data).velocityY > 0)
				(*data).velocityY--;
			else if((*data).velocityY < 0)
				(*data).velocityY++;
	}
		else{																// This AI is not it
			// X-direction escaping
			if(Characters[it].x > (*data).x){										// escape to left
				if((*data).cStateP==1 || (*data).cStateP==2 || (*data).cStateP==3 || 	// is a something is left, stop leftward momentum
					 ((*data).cStateS==1 || (*data).cStateS==2 || (*data).cStateS==3))		
					(*data).velocityX = 0;
				else if((*data).velocityX > -(*data).spd)															// otherwise move leftward velocity towards max speed
						(*data).velocityX--;
			}
			else if(Characters[it].x < (*data).x){ 							// escape to right
				if(((*data).cStateP==5 || (*data).cStateP==6 || (*data).cStateP==7) || // is a something is right, stop rightward momentum
					 ((*data).cStateS==5 || (*data).cStateS==6 || (*data).cStateS==7))		
					(*data).velocityX = 0;
				else if((*data).velocityX < (*data).spd)																// otherwise move rightward velocity towards max speed
							(*data).velocityX ++;
			}
			// Move velocity towards 0 when no input
			else if((*data).velocityX > 0)
				(*data).velocityX--;
			else if((*data).velocityX < 0)
				(*data).velocityX++;
			// Y-direction escaping
			if(Characters[it].y > (*data).y){ 									// escape to up
				if((*data).cStateP==3 || (*data).cStateP==4 || (*data).cStateP==5 || 	// is a something is up, stop upward momentum
					 ((*data).cStateS==3 || (*data).cStateS==4 || (*data).cStateS==5))		
					(*data).velocityY = 0;
				else if((*data).velocityY > -(*data).spd)															// otherwise move upward velocity towards max speed
						(*data).velocityY--;
			}
			else if(Characters[it].y < (*data).y){ 							// escape to down			
				if((*data).cStateP==1 || (*data).cStateP==7 || (*data).cStateP==8 || 	// is a something is down, stop downward momentum
					 ((*data).cStateS==1 || (*data).cStateS==7 || (*data).cStateS==8))	
					(*data).velocityY = 0;
				else if((*data).velocityY < (*data).spd)															// otherwise move downward velocity towards max speed
						(*data).velocityY++;
			}
			// Move velocity towards 0 when no input
			else if((*data).velocityY > 0)
				(*data).velocityY--;
			else if((*data).velocityY < 0)
				(*data).velocityY++;
	}
	(*data).x+=(*data).velocityX;
	(*data).y+=(*data).velocityY;
}


#include "tm4c123gh6pm.h"
#include "Bitmaps.h"
#include "Powerup.h"
#include "Screen.h"
#include "ST7735.h"
#include "Character.h"
#include "Random.h"
#include <string.h>
Powerup Powerups[powerupCount];
void Powerup_Init(void){
	Powerup temp[]={
	{&Sprite[cursor],Sprite[cursor].width,Sprite[cursor].height,0,0,0,*PowerUp_SpeedUp}, //ignore this first element, it's just filler
	{&Sprite[cursor],Sprite[cursor].width,Sprite[cursor].height,0,0,0,*PowerUp_SpeedUp},
	{&Sprite[cursor],Sprite[cursor].width,Sprite[cursor].height,0,0,0,*PowerUp_SlowDown}
	};
	 memcpy(Powerups, temp, sizeof(temp));
}

//------------Powerup_Generate------------
// Activate a new power up
// Input: none
// Output: none
void Powerup_Generate(void){
	int i=0;
	for(int j=1; j < powerupCount; j++)		// count unactive powerups
		if(!Powerups[j].active)
			i++;
	if(i > 0){														// if atleast 1 powerup is innactive	
		i=0;
		do {i=(Random()%(powerupCount-1))+1;}		// find a powerup that is innactive
		while(Powerups[i].active);
		Powerups[i].x= Random()%(mapWidth*8-(*Powerups[i].bmp).width);	// Define random starting position
		Powerups[i].y= Random()%(mapHeight*8-(*Powerups[i].bmp).height)+(*Powerups[i].bmp).height;
		Powerups[i].active=1;								// activate powerup
	}
}
//------------Powerup_Draw------------
// Draw a powerup onto the screen
// Input: powerup struct to be drawn
// Output: none
void Powerup_Draw(Powerup data){
		ST7735_DrawBitmap(data.x-frameXCord, data.y-frameYCord, (*data.bmp).array, (*data.bmp).width, (*data.bmp).height);
}
//------------Powerup_DrawPowerups------------
// Draw active powerups to the screen
// Input: none
// Output: none
void Powerup_DrawPowerups(void){
		for(int i = 1; i<powerupCount; i++)		// Draw powerups
		if(Powerups[i].active){
			Powerup_Draw(Powerups[i]);
		}
}

//------------Powerup_SpeedUp------------
// increases characters speed temporarily
// Input: character to be altered
// Output: none
void PowerUp_SpeedUp(Character *data){
	if((*data).powerTick == 0){		// initial case
		(*data).powerTick=30;				// set power up duration
		(*data).spd+=4;							// increase player max speed
		return;		
	}
	(*data).powerTick--;					// decrement power up time
	if((*data).powerTick == 0)		// end case
	{
		(*data).spd-=4;							// reset player speed
		(*data).power=0;						// clear player power
	}
}
//------------Powerup_SlowDown------------
// lowers characters speed temporarily
// Input: character to be altered
// Output: none
void PowerUp_SlowDown(Character *data){
	if((*data).powerTick == 0){		// initial case
		(*data).powerTick=30;				// set power up duration
		(*data).spd-=4;							// decreases player max speed
		return;		
	}
	(*data).powerTick--;					// decrement power up time
	if((*data).powerTick == 0)		// end case
	{
		(*data).spd+=4;							// reset player speed
		(*data).power=0;						// clear player power
	}
}


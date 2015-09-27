#include "tm4c123gh6pm.h"
#include "Bitmaps.h"
#include "Powerup.h"
#include "Screen.h"
#include "ST7735.h"
#include "Character.h"
#include "Random.h"
#include "HotPotato.h"
#include <string.h>

Powerup Powerups[powerupCount];
void Powerup_Init(void){
	Powerup temp[]={
	{&PowerupTile[speedUpTile],PowerupTile[speedUpTile].width,PowerupTile[speedUpTile].height,0,0,0,*PowerUp_SpeedUp}, //ignore this first element, it's just filler
	{&PowerupTile[speedUpTile],PowerupTile[speedUpTile].width,PowerupTile[speedUpTile].height,0,0,0,*PowerUp_SpeedUp},
	{&PowerupTile[slowDownTile],PowerupTile[slowDownTile].width,PowerupTile[slowDownTile].height,0,0,0,*PowerUp_SlowDown},
	{&PowerupTile[shieldTile],PowerupTile[shieldTile].width,PowerupTile[shieldTile].height,0,0,0,*PowerUp_Shield},
	{&PowerupTile[hpUpTile],PowerupTile[hpUpTile].width,PowerupTile[hpUpTile].height,0,0,0,*PowerUp_HPUp}
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
		Powerups[i].x= Random32()%(Map[currentMap].width*mapScale-(*Powerups[i].bmp).width);	// Define random starting position
		Powerups[i].y= Random32()%(Map[currentMap].height*mapScale-(*Powerups[i].bmp).height)+(*Powerups[i].bmp).height;
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
// *************************** POWERUP FUNCTIONS ***************************
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
//------------Powerup_Shield------------
// Prevents player from becoming "it" and removes "it" from player
// Input: character to be altered
// Output: none
void PowerUp_Shield(Character *data){
	int j=0;
	while(j != characterCount){	// find a new person to be it
		j = Random()%characterCount;
		if(Characters[j].active && j != 0)	// newplayer must be active and not yourself
		{
			HotPotato_SetIt(j);			// set new person to it
			j=characterCount;
		}
	}
	(*data).potato -= 30;				// Add invunerability time (+1 sec)
	(*data).power=0;						// clear player power
	(*data).powerTick=0;
}
//------------Powerup_HPUp------------
// Increases the players HP by a set amount
// Input: character to be altered
// Output: none
void PowerUp_HPUp(Character *data){
	(*data).hp+=50;							// increase player health
	(*data).power=0;						// clear player power
	return;		
}

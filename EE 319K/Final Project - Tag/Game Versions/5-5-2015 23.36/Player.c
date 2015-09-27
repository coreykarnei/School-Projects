#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Player.h"
#include "ADC.h"					
#include "Screen.h"
#include "Bitmaps.h"
#include "Character.h"
#include "Button0.h"
#include "Powerup.h"
#define player Characters[p1]

// ******************** Functions *********************
//------------Player_Init------------
// Initialize player by setting up character struct
// Input: none
// Output: none
void Player_Init(void){
	player.bmp=&Sprite[2];							// Define sprite for player
	player.x=120;												// Define starting position
	player.y=120;
	player.velocityX=0;
	player.velocityY=0;
	player.height=(*player.bmp).height;
	player.width=(*player.bmp).width;
	player.spd=3;												// Define movement speed
	player.hp=400;
	player.active=1;
	player.power=1;
}
//------------Player_InitP------------
// Initialize player by setting up character struct with parameters
// Input: sprite pointer, x coordinate, y coordinate, speed, hp
// Output: none
void Player_InitP(Bitmap* bmp, short x, short y, short spd, short hp){
	player.bmp=bmp;										// Define sprite for player
	player.x=x;												// Define starting position
	player.y=y;
	player.velocityX=0;
	player.velocityY=0;
	player.height=(*player.bmp).height;
	player.width=(*player.bmp).width;
	player.spd=spd;											// Define movement speed
	player.hp=hp;
	player.active=1;
	player.power=1;
}

//------------Player_Update------------
// Update player by calling other functions accordingly
// Input: none
// Output: none
void Player_Update(void){
	// Re-locate player
	Player_Move();
	Player_UpdateVelocity();
	if(Button0_Pressed() && player.power > 0 && player.powerTick==0)
	{
		Powerups[player.power].function(&player);			// call powerup function which will intialize powerTick
	}
	else if(player.powerTick>0)
	{
		Powerups[player.power].function(&player);
	}
}

//------------Player_MoveOld------------
// Change coordinates based on analog input
// Input: none
// Output: none
void Player_MoveOld(void){
	uint32_t data[2];
	ADC0_In(data);														// get ADC input (data[0] => x, data[1] => y)
	
	// Slide pot magnitude direction
	if(data[0] < 1365) 												// move left
		if(!(player.cStateP==1 || player.cStateP==2 || player.cStateP==3))		// is a character in the way?
			if(!(player.cStateS==1 || player.cStateS==2 || player.cStateS==3))	// is a structure in the way?
				player.x -= player.spd;
	if(data[0] > 2730) 												// move right
		if(!(player.cStateP==5 || player.cStateP==6 || player.cStateP==7))		// is a character in the way?
			if(!(player.cStateS==5 || player.cStateS==6 || player.cStateS==7))	// is a structure in the way?
				player.x += player.spd;
		
	// Slide pot magnitude direction
	if(data[1] > 1365) 												// move down
		if(!(player.cStateP==3 || player.cStateP==4 || player.cStateP==5))		// is a character in the way?
			if(!(player.cStateS==3 || player.cStateS==4 || player.cStateS==5))	// is a structure in the way?
				player.y -= player.spd;
	if(data[1] < 2730) 												// move up
		if(!(player.cStateP==1 || player.cStateP==7 || player.cStateP==8)) 		// is a character in the way?
			if(!(player.cStateS==1 || player.cStateS==7 || player.cStateS==8))	// is a structure in the way?
				player.y += player.spd;
}
//------------Player_Move------------
// Change coordinates based on velocity
// Input: none
// Output: none
void Player_Move(void){
	player.x += player.velocityX;
	player.y += player.velocityY;
}
//------------Player_UpdateVelocity------------
// Change x and y velocity based on ADC input
// Input: none
// Output: none
void Player_UpdateVelocity(void){
	uint32_t data[2];
	ADC0_In(data);														// get ADC input (data[0] => x, data[1] => y)
		// Slide pot magnitude direction
	if(data[0] < 1365){												// move left
		if(player.cStateP==1 || player.cStateP==2 || player.cStateP==3 || 	// is a something is left, stop leftward momentum
			 (player.cStateS==1 || player.cStateS==2 || player.cStateS==3))		
			player.velocityX = 0;
		else if(player.velocityX > -player.spd)																		// otherwise move leftward velocity towards max speed
				player.velocityX--;
	}
	else if(data[0] > 2730){ 												// move right
		if((player.cStateP==5 || player.cStateP==6 || player.cStateP==7) || // is a something is right, stop rightward momentum
			 (player.cStateS==5 || player.cStateS==6 || player.cStateS==7))		
			player.velocityX = 0;
		else if(player.velocityX < player.spd)																			// otherwise move rightward velocity towards max speed
					player.velocityX ++;
	}
	// Move velocity towards 0 when no input
	else if(player.velocityX > 0)
		player.velocityX--;
	else if(player.velocityX < 0)
		player.velocityX++;
			
		
	// Slide pot magnitude direction
	if(data[1] > 2730){ 												// move up
		if(player.cStateP==3 || player.cStateP==4 || player.cStateP==5 || 	// is a something is up, stop upward momentum
			 (player.cStateS==3 || player.cStateS==4 || player.cStateS==5))		
			player.velocityY = 0;
		else if(player.velocityY > -player.spd)																		// otherwise move upward velocity towards max speed
				player.velocityY--;
	}
	else if(data[1] < 1365){ 												// move down			
		if(player.cStateP==1 || player.cStateP==7 || player.cStateP==8 || 	// is a something is down, stop downward momentum
			 (player.cStateS==1 || player.cStateS==7 || player.cStateS==8))	
			player.velocityY = 0;
		else if(player.velocityY < player.spd)																			// otherwise move downward velocity towards max speed
				player.velocityY++;
	}
	// Move velocity towards 0 when no input
	else if(player.velocityY > 0)
		player.velocityY--;
	else if(player.velocityY < 0)
		player.velocityY++;
	/*if(Button0_Pressed())		temp code for stopping velocity with button
	{
		player.velocityX=0;
		player.velocityY=0;
	}*/
}

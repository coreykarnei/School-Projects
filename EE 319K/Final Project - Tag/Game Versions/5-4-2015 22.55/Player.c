#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Player.h"
#include "ADC.h"					
#include "Screen.h"
#include "Bitmaps.h"
#include "Character.h"
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
	player.height=(*player.bmp).height;
	player.width=(*player.bmp).width;
	player.spd=3;												// Define movement speed
	player.hp=400;
	player.active=1;
}

//------------Player_Update------------
// Update player by calling other functions accordingly
// Input: none
// Output: none
void Player_Update(void){
	// Re-locate player
	Player_Move();
}

//------------Player_Move------------
// Change coordinates based on analog input
// Input: none
// Output: none
void Player_Move(void){
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
	if(data[1] > 1365) 												// move up
		if(!(player.cStateP==3 || player.cStateP==4 || player.cStateP==5))		// is a character in the way?
			if(!(player.cStateS==3 || player.cStateS==4 || player.cStateS==5))	// is a structure in the way?
				player.y -= player.spd;
	if(data[1] < 2730) 												// move down (turn right)
		if(!(player.cStateP==1 || player.cStateP==7 || player.cStateP==8)) 		// is a character in the way?
			if(!(player.cStateS==1 || player.cStateS==7 || player.cStateS==8))	// is a structure in the way?
				player.y += player.spd;
		
}

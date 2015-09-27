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
	player.bmp=&Sprite[2];		// Define sprite for player
	player.x=120;												// Define starting position
	player.y=120;
	player.height=(*player.bmp).height;
	player.width=(*player.bmp).width;
	player.spd=3;												// Define movement speed
	player.hp=500;
}

//------------Player_Update------------
// Update player by calling other functions accordingly
// Input: none
// Output: none
void Player_Update(void){
	// Re-locate player
	Player_Move();
	// decrement no-tag-back timer
	if(player.potato >= 1){
		player.hp--;
		if(player.potato > 1)
			player.potato--;
	}

		
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
		if(!(player.cState==1 || player.cState==2 || player.cState==3))	// is something in the way?
			player.x -= player.spd;
	if(data[0] > 2730) 												// move right
		if(!(player.cState==5 || player.cState==6 || player.cState==7))	// is something in the way?
			player.x += player.spd;
		
	// Slide pot magnitude direction
	if(data[1] < 1365) 												// move up
		if(!(player.cState==3 || player.cState==4 || player.cState==5))	// is something in the way?
			player.y -= player.spd;
	if(data[1] > 2730) 												// move down (turn right)
			if(!(player.cState==1 || player.cState==7 || player.cState==8)) // is something in the way
				player.y += player.spd;
		
}

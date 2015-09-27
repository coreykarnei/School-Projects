#include "tm4c123gh6pm.h"
#include "Character.h"
#include "Screen.h"
#include "ST7735.h"
#include "HotPotato.h"
#include "Powerup.h"

Character Characters[characterCount];

// *********************** Functions *************************
//------------Character_Draw------------
// Draw a character onto the screen
// Input: Character struct to be drawn
// Output: none
void Character_Draw(Character data){
	ST7735_DrawBitmap(data.x-frameXCord, data.y-frameYCord, (*data.bmp).array, (*data.bmp).width, (*data.bmp).height);
}
//------------Character_DrawCharacters------------
// Draw specified amount of Characters to the screen
// Input: Amount of characters to be drawn
// Output: none
void Character_DrawCharacters(void){
	short temp = HotPotato_WhosIt();
	for(int i = 0; i<characterCount; i++)													// Draw characters
		if(Characters[i].active){
			if(i==temp)	
				ST7735_FillRect(Characters[i].x-frameXCord-1, (Characters[i].y-frameYCord)-Characters[i].height, Characters[i].width+2, Characters[i].height+2, 0x001F);// Draw a red outline around the "it" person
			Character_Draw(Characters[i]);
		}
}
//------------Character_UpdateCollisions------------
// Call all other collision update functions
// Input: none
// Output: none
void Character_UpdateCollisions(void){
	Character_UpdatePowerups();
	Character_UpdateCollisionsP();
	Character_UpdateCollisionsS();
}
//------------Character_UpdatePowerups------------
// Update collision status for all powerups
// Input: none
// Output: none
void Character_UpdatePowerups(void){
	for(int i = 0; i < characterCount ; i++)						// update all powerup-collisions
		if(Characters[i].active)													// check if character is active
			for(int j = 1; j < powerupCount; j++)
				if(Powerups[j].active)												// check if powerup is active
					if (Characters[i].x < Powerups[j].x + Powerups[j].width && Characters[i].x + Characters[i].width > Powerups[j].x &&		// check for contact
						  Characters[i].y > Powerups[j].y - Powerups[j].height && Characters[i].y - Characters[i].height < Powerups[j].y) {
						Powerups[j].active=0;											// set powerup as inactive
						if(Characters[i].powerTick > 0){
							Characters[i].powerTick=1;							// finish out old powerup
							Powerups[Characters[i].power].function(&Characters[i]);
						}
						Characters[i].power=j;										// store new power up
					}		
}
//------------Character_Collision------------
// Compare two characters to tell where they collide
// Input: Two characters to be compared
// Output: (RELATIVE TO CHAR1) 0=no collision, 1=bottom-left, 2=mid-left, 3=top-left, 4=top-mid, 5=top-right, 6=mid-right, 7=bottom-right, 8=bottom-mid, 9=other collision
int Character_Collision(Character char1, Character char2){
	if (char1.x < char2.x + char2.width && char1.x + char1.width > char2.x &&													// STEP: any collision
			char1.y > char2.y - char2.height && char1.y - char1.height < char2.y){
		// Left side checks
		if(char2.x+char2.width >= char1.x && char2.x+char2.width <=char1.x + char1.width/2){						// STEP: left half
			if(char1.y-char1.height/2 >=char2.y - char2.height && char1.y-char1.height/2 <= char2.y)			// mid-left
				return 2;
			if(char2.y - char2.height >= char1.y - char1.height/2 && char2.y - char2.height <= char1.y)		// bottom-left
				return 1;
			if(char2.y >= char1.y - char1.height && char2.y <= char1.y - char1.height/2)									// top-left
				return 3;
		}
		// Right side checks
		if(char2.x <= char1.x + char1.width && char2.x >= char1.x + char1.width/2){											// STEP: right half
			if(char1.y-char1.height/2 >= char2.y - char2.height && char1.y-char1.height/2 <= char2.y)			// mid-right
				return 6;
			if(char2.y - char2.height >= char1.y - char1.height/2 && char2.y - char2.height <= char1.y)		// bottom-right
				return 7;
			if(char2.y >= char1.y - char1.height && char2.y <= char1.y - char1.height/2)									// top-right
				return 5;
		}
		if(char2.y-char2.height <= char1.y && char2.y-char2.height >=char1.y - char1.height/2)					// STEP: bottom half
			if(char1.x+char1.width/2 >= char2.x && char1.x + char1.width/2 <=char2.x + char2.width)				// bottom-mid
				return 8;
		if(char2.y >= char1.y - char1.height && char2.y <= char1.y - char1.height/2)										// STEP: top half
			if(char1.x+char1.width/2 >= char2.x && char1.x + char1.width/2 <=char2.x + char2.width)				// top-mid
				return 4;
		return 9;																																												// none-of-the-above collision
	}
	return 0;																																													// no collision
}
//------------Character_UpdateCollisionsP------------
// Update collision status for all characters
// Input: none
// Output: none
void Character_UpdateCollisionsP(void){
	for(int i = 0; i < characterCount ; i++){												// update all player-collisions
		Characters[i].cStateP=0;
		if(Characters[i].active)													// check if 1st character is active
			for(int j = 0; j < characterCount; j++){
				if(Characters[j].active)											// check if 2nd character is active
					if(i != j){																	// Can't collide with yourself
						int temp=Character_Collision(Characters[i], Characters[j]);
						if(temp){
							Characters[i].cStateP=temp;
							if(Characters[i].potato == 1){
								HotPotato_SetIt(j);
							}
						}		
					}	
		}
	}
}
//------------Character_UpdateCollisionsS------------
// Update structure collision status for all characters (Map borders, structures)
// Input: none
// Output: none
void Character_UpdateCollisionsS(void){
	for(int i = 0; i < characterCount ; i++){												// update all player-collisions
		Characters[i].cStateS=0;
		if(Characters[i].active){													// check if 1st character is active
			if(Characters[i].x <= 0){
				Characters[i].cStateS=2;													// map left border collision
				if(Characters[i].y-Characters[i].height <= 0)	
					Characters[i].cStateS=3;												// map top-left border collision
				if(Characters[i].y >= (mapHeight*8))
					Characters[i].cStateS=1;												// map bottom-left border collision
			}
			else if(Characters[i].x + Characters[i].width >= (mapWidth*8)){
				Characters[i].cStateS=6;													// map right border collision
				if(Characters[i].y-Characters[i].height <= 0)
					Characters[i].cStateS=5;												// map top-right border collision
				if(Characters[i].y >= (mapHeight*8))
					Characters[i].cStateS=7;												// map bottom-right border collision
			}
			else if(Characters[i].y >= (mapHeight*8))
				Characters[i].cStateS=8;													// map bottom border collision
			else if(Characters[i].y - Characters[i].height <= 0)
				Characters[i].cStateS=4;													// map top border collision
		}
	}
}
//------------Character_CollisionStatus------------
// Print the collision status for the selected character in the top left of the screen
// Input: Character status to be evaluated
// Output: none
void Character_CollisionStatus(short data){
		switch(data){												// print collision status
		case 0: 		ST7735_SetCursor(1, 1);
								ST7735_OutString("No ouch...");
								break;
		case 1: 		ST7735_SetCursor(1, 1);
								ST7735_OutString("Bottom Left");
								break;
		case 2: 		ST7735_SetCursor(1, 1);
								ST7735_OutString("mid LEft yo");
								break;
		case 3:			ST7735_SetCursor(1, 1);
								ST7735_OutString("top-left");
								break;
		case 4: 		ST7735_SetCursor(1, 1);
								ST7735_OutString("top-mid");
								break;
		case 5: 		ST7735_SetCursor(1, 1);
								ST7735_OutString("Top right yo");
								break;
		case 6:			ST7735_SetCursor(1, 1);
								ST7735_OutString("right Mido");
								break;
		case 7:			ST7735_SetCursor(1, 1);
								ST7735_OutString("Bottomz-right");
								break;
		case 8:			ST7735_SetCursor(1, 1);
								ST7735_OutString("bottom-Mido");
								break;
		case 9:			ST7735_SetCursor(1, 1);
								ST7735_OutString("!?WHY YOU INSIDE");
								ST7735_SetCursor(1, 2);								
								ST7735_OutString("ME BRAH?!");
								break;
	}
}

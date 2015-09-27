#include "tm4c123gh6pm.h"
#include "Character.h"
#include "Screen.h"
#include "ST7735.h"

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
void Character_DrawCharacters(int data){
	for(int i = 0; i<data; i++)													// Draw characters
		Character_Draw(Characters[i]);
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
//------------Character_UpdateCollisions------------
// Update collision status for all characters
// Input: none
// Output: none
void Character_UpdateCollisions(void){
	for(int i = 0; i < 4 ; i++){												// update all player-collisions
		Characters[i].cState=0;
		for(int j = 0; j < 4; j++){
			if(i != j){
				int temp=Character_Collision(Characters[i], Characters[j]);
				if(temp){
					Characters[i].cState=temp;
					if(Characters[i].potato == 1){
						Characters[j].potato= 31;				// pass the potato, set to 31 initially as a sort of no-tag back timer (30hz=1sec)
						Characters[i].potato=0;	
					}
				}
				
			}	
		}
		if(Characters[i].x <= 0){
			Characters[i].cState=2;													// map left border collision
			if(Characters[i].y-Characters[i].height <= 0)	
				Characters[i].cState=3;												// map top-left border collision
			if(Characters[i].y >= (mapHeight*8))
				Characters[i].cState=1;												// map bottom-left border collision
		}
		else if(Characters[i].x + Characters[i].width >= (mapWidth*8)){
			Characters[i].cState=6;													// map right border collision
			if(Characters[i].y-Characters[i].height <= 0)
				Characters[i].cState=5;												// map top-right border collision
			if(Characters[i].y >= (mapHeight*8))
				Characters[i].cState=7;												// map bottom-right border collision
		}
		else if(Characters[i].y >= (mapHeight*8))
			Characters[i].cState=8;													// map bottom border collision
		else if(Characters[i].y - Characters[i].height <= 0)
			Characters[i].cState=4;													// map top border collision
	}
}
//------------Character_Draw------------
// Print the collision status for the selected character in the top left of the screen
// Input: Character to be evaluated
// Output: none
void Character_CollisionStatus(Character data){
		switch(data.cState){												// print collision status
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

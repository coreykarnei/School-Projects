#include "tm4c123gh6pm.h"
#include "Character.h"
#include "HotPotato.h"
#include "ST7735.h"
#include "Random.h"

short it;

//------------HotPotato_Update------------
// Update hot-potato game mode elements
// Input: none
// Output: none
void HotPotato_Update(void){
	HotPotato_Cooldown();
	HotPotato_Burn();
	HotPotato_DrawStatus();
	HotPotato_Eliminate();
}
//------------HotPotato_WhosIt------------
// Find out which character is it (mainly for AI)
// Input: none
// Output: index of "it" character, -1 if none found
int HotPotato_WhosIt(void){
	return it;
}
//------------HotPotato_SetIt------------
// Set "it" index to input
// Input: index of new "it" character
// Output: none
void HotPotato_SetIt(int data){
	Characters[it].potato=0;				// relieve old player
	it = data;											// pass the potato, set to 31 initially as a sort of no-tag back timer (30hz=1sec)
	Characters[data].potato= 31;		
}
//------------HotPotato_Cooldown------------
// Step down the tag-back cooldown on "it" character
// Input: none
// Output: none
void HotPotato_Cooldown(void){
	if(Characters[it].potato > 1)
		Characters[it].potato--;
}
//------------HotPotato_Burn------------
// Burns the player holding the potato, lowering their hp every clock interupt
// Input: none
// Output: none
void HotPotato_Burn(void){
	if(Characters[it].hp > 0)
		Characters[it].hp--;
}
//------------HotPotato_DrawStatus------------
// Draws p1 status elements on the top of the screen
// Input: none
// Output: none
void HotPotato_DrawStatus(void){
	ST7735_SetCursor(16, 1);
	LCD_OutDec(Characters[0].hp/2);					// Write hp status
	if(Characters[0].hp == 0){							// Write death status
		ST7735_SetCursor(6, 5);
		ST7735_OutString("You Dead.");
	}
	if(Characters[0].potato){								// Write "it" status
		ST7735_SetCursor(16, 2);
		ST7735_OutString("It!");
		ST7735_DrawBitmap(110, 16, Sprite[smallPotato].array, 8,8);
	}
	else{																		// Write "not-it" status
		ST7735_SetCursor(14, 2);
		ST7735_OutString("Not it");
	}
}
//------------HotPotato_Eliminate------------
// Kills off characters with 0 hp
// Input: none
// Output: none
void HotPotato_Eliminate(void){
	for(int i =0; i < characterCount; i++)
		if(Characters[i].active)
			if(Characters[i].hp <=0)
			{
				Characters[i].active=0;
				int j;
				while(!Characters[it].active){
					j=Random()%4;
					if(Characters[j].active)
						HotPotato_SetIt(j);
				}
			}
	
}

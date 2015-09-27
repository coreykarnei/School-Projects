#include "tm4c123gh6pm.h"
#include "Character.h"
#include "HotPotato.h"
#include "ST7735.h"
#include "Random.h"
#include "Sound.h"
#include "Timer1.h"
#include "Button0.h"
#include "Menu.h"
#include "Powerup.h"
short it;
short hpDivisor;

//------------HotPotato_Init------------
// Setup hot-potato game mode elements
// Input: none
// Output: none
void HotPotato_Init(void){
	int temp=0;
		for(int i = 0; i<characterCount; i++)					// get count on active Players
			if(Characters[i].active)
				temp++;
	HotPotato_SetIt(Random()%temp);
	hpDivisor = (Characters[0].hp)/100;							// get value that will make health appear out of 100
}
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
	Characters[it].potato=-30;				// relieve old player (set to -30 as a no tag back timer)
	Characters[it].spd--;						// set speed back
	it = data;											// pass the potato, set to 31 initially as a sort of no-tag back timer (30hz=1sec) (outdated)
	Characters[data].potato= 1;
	Characters[data].spd++;					// increase it player's speed
	Sound_Shoot();
}
//------------HotPotato_Cooldown------------
// Step down the tag-back cooldown on "it" character
// Input: none
// Output: none
/*void HotPotato_Cooldown(void){
	if(Characters[it].potato > 1)
		Characters[it].potato--;
}*/
void HotPotato_Cooldown(void){
	for(int i = 0; i < characterCount; i++)
		if(Characters[i].potato < 0 && Characters[i].active)
			Characters[i].potato++;
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
	int count=0;
	for(int i = 0; i<characterCount; i++)						// get count on active Players
		if(Characters[i].active)
			count++;
	ST7735_SetCursor(0, 0);
	ST7735_OutString("Players: ");									// Write player count
	LCD_OutDec(count);		
	ST7735_SetCursor(0, 1);
	ST7735_OutString("Health: ");										// Write health remaining
	LCD_OutDec(Characters[0].hp/hpDivisor);		
	ST7735_DrawFastHLine(109, 2, 16, 0x001F);
	ST7735_DrawFastHLine(109, 19, 16, 0x001F);
	ST7735_DrawFastVLine(108, 2, 18, 0x001F);
	ST7735_DrawFastVLine(125, 2, 18, 0x001F);
	if(Characters[0].power)
		ST7735_DrawBitmap(109, 18, (*Powerups[Characters[0].power].bmp).array , 16, 16);
		
	if(Characters[0].hp == 0){											// Write death status
		ST7735_SetCursor(6, 5);
		ST7735_OutString("You Dead.");
		Music_Stop();
		while(!Button0_Pressed()){};		// wait for button press to go to menu
		Delay100ms(5);
		Button0_Pressed();							// attempt to clear button due to double press problem
		menu=1;
	}
	else if(count == 1 && Characters[0].hp > 0)			// write win status
	{
		ST7735_SetCursor(6, 5);
		ST7735_OutString("You Won!");									
		Music_Stop();
		Characters[0].hp++;
		while(!Button0_Pressed()){};		// wait for button press to go to menu
		Delay100ms(5);
		Button0_Pressed();							// attempt to clear button due to double press problem
		Music_Stop();
		menu=1;
	}
	else if(Characters[0].potato > 0){									// Write "it" status
		ST7735_SetCursor(0, 2);
		ST7735_OutString("You're It!");
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
					j=Random()%characterCount;
					if(Characters[j].active)
						HotPotato_SetIt(j);
				}
			}
	
}

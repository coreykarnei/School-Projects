#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "HotPotato.h"
#include "Character.h"
#include "AI.h"
#include "Player.h"
#include "Screen.h"
#include "Button0.h"
#include "ADC.h"
#include "Timer1.h"
#include "Menu.h"
#include <stdlib.h>
#include "Sound.h"
#include "Random.h"
#include "Powerup.h"

void Delay100ms(uint32_t count);
short menu;

void Menu_Init(){
	menu=0;
}
void Menu_Start(void){
	 /* ST7735_FillScreen(0x0000);            // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);	 */
	//Timer2_Init(Menu_Start,  0);
//	Timer1_Init(Menu_Init,  80000000/60);
	ST7735_FillScreen(0);
	ST7735_SetTextColor(0xD7E0);
	ST7735_SetCursor(9, 3);
  ST7735_OutString("TAG");
	while(!Button0_Pressed()){};		// Wait for button press
	Sound_Highpitch();
	Delay100ms(1);
	Button0_Pressed();							// attempt to clear button due to double press problem
	menu++;													// advance to next screen
}
void Menu_Pregame(void){
	ST7735_FillScreen(0);
	ST7735_SetCursor(4, 3);
	ST7735_OutString("Sprite:");
	ST7735_SetCursor(4, 5);
	ST7735_OutString("Game Mode:");
	ST7735_SetCursor(4, 7);
	ST7735_OutString("AI Count:");
	ST7735_SetCursor(4, 9);
	ST7735_OutString("Difficulty:");
	ST7735_SetCursor(4, 11);
	ST7735_OutString("UART:");
	ST7735_SetCursor(4, 13);
	ST7735_OutString("Launch!");
	volatile short cursorX = 12;
	volatile short cursorY = 136;
	volatile short sprite = 0;
	volatile short aiCount = 8;
	volatile short aiDiff = 2;
	ST7735_DrawBitmap(cursorX, cursorY, Sprite[cursor].array, Sprite[cursor].width,Sprite[cursor].height);
	ST7735_DrawBitmap(70, 36, Sprite[sprite].array, Sprite[sprite].width,Sprite[sprite].height);
	ST7735_SetCursor(13, 7);
	LCD_OutDec(aiCount);
	ST7735_SetCursor(15, 9);
	LCD_OutDec(aiDiff);
	
	uint32_t data[2];								// array to get ADC input									
	while(!(Button0_Pressed() && cursorY == 136)){	// WAIT FOR LAUNCH
		// Move Cursor
		ADC0_In(data);								// get ADC input (data[0] => x, data[1] => y)
		if(data[1]<1365) 	
			if(cursorY < 136){					// if input is down & not on edge element, erase old cursor and move cursor down
				ST7735_FillRect(cursorX, cursorY-Sprite[cursor].height+1, Sprite[cursor].width, Sprite[cursor].height, 0x0000);
				cursorY+=20;
				ST7735_DrawBitmap(cursorX, cursorY, Sprite[cursor].array, Sprite[cursor].width,Sprite[cursor].height);
				Sound_Menutick1();
				Delay100ms(2);
			}
		if(data[1]>2730)
			if(cursorY >36){						// if input is up & not on edge element, erase old cursor and move up
				ST7735_FillRect(cursorX, cursorY-Sprite[cursor].height+1, Sprite[cursor].width, Sprite[cursor].height, 0x0000);
				cursorY-=20;							// move cursor up
				ST7735_DrawBitmap(cursorX, cursorY, Sprite[cursor].array, Sprite[cursor].width,Sprite[cursor].height);
				Sound_Menutick1();
				Delay100ms(2);
			}
		switch(cursorY){	// Option select
			case 36:														// Sprite Select
				if(data[0]<1365) // left 	
					if(sprite > 0){
						ST7735_FillRect(70, 36-Sprite[sprite].height+1, Sprite[sprite].width, Sprite[sprite].height, 0x0000);
						sprite--;
						ST7735_DrawBitmap(70, 36, Sprite[sprite].array, Sprite[sprite].width,Sprite[sprite].height);
						Sound_Menutick2();
						Delay100ms(3);
					}
				if(data[0]>2730) // right
					if(sprite < (spriteCount-2)){
						ST7735_FillRect(70, 36-Sprite[sprite].height+1, Sprite[sprite].width, Sprite[sprite].height, 0x0000);
						sprite++;
						ST7735_DrawBitmap(70, 36, Sprite[sprite].array, Sprite[sprite].width,Sprite[sprite].height);
						Sound_Menutick2();
						Delay100ms(3);
					}
			break;
			case 76:												// AI Count
				if(data[0]<1365) // left 	
					if(aiCount > 1){
						aiCount--;
						ST7735_SetCursor(13, 7);
						ST7735_OutString("  ");
						ST7735_SetCursor(13, 7);
						LCD_OutDec(aiCount);
						Sound_Menutick2();
						Delay100ms(3);
					}
				if(data[0]>2730) // right
					if(aiCount < characterCount-1){
						aiCount++;
						ST7735_SetCursor(13, 7);
						LCD_OutDec(aiCount);
						Sound_Menutick2();
						Delay100ms(3);
					}
			break;
			case 96:												// AI Difficulty
				if(data[0]<1365) // left 	
					if(aiDiff > 1){
						aiDiff--;
						ST7735_SetCursor(15, 9);
						LCD_OutDec(aiDiff);
						Sound_Menutick2();
						Delay100ms(3);
					}
				if(data[0]>2730) // right
					if(aiDiff < 2){
						aiDiff++;
						ST7735_SetCursor(15, 9);
						LCD_OutDec(aiDiff);
						Sound_Menutick2();
						Delay100ms(3);
					}
			break;
		}
	}
	Sound_Highpitch();
	Delay100ms(1);
	Button0_Pressed();							// attempt to clear button due to double press problem
	Random_Init(NVIC_ST_CURRENT_R);
	Player_InitP(&Sprite[sprite], Random()%(mapWidth*8-Sprite[sprite].width), Random()%(mapHeight*8-Sprite[sprite].height)+Sprite[sprite].height, 5, 200);
	AI_Init(aiCount, aiDiff);
	Powerup_Init();
	menu++;													// advance to next screen
}	
void Menu_Game(void){
	HotPotato_Init();
	Powerup_Generate();
	Powerup_Generate();
	//Music_Tropical();
	while(1){
		Character_UpdateCollisions();
		Powerup_Generate();
		Screen_Update();
		if (player.active)
			Player_Update();
		AI_Update();
		HotPotato_Update();
		
		Character_DrawCharacters();					// Draw 4 characters
		Powerup_DrawPowerups();
		
		Character_CollisionStatus(player.cStateS);				// Write collision status for P1
	}
}

// Used for avoiding button double-press problems
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 727240 = 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

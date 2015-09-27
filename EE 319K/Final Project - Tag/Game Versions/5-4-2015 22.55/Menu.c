#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "HotPotato.h"
#include "Character.h"
#include "AI.h"
#include "Player.h"
#include "Screen.h"
#include "Button0.h"
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
	void ST7735_FillScreen(uint16_t color);
	ST7735_SetCursor(4, 3);
  ST7735_OutString("TAG");
	if(Button0_Pressed())
		menu=2;
}
void Menu_Pregame(void){
}
void Menu_Game(void){
	Character_UpdateCollisions();
	Character_UpdateCollisionsS();
	Screen_Update();
	if (player.active)
		Player_Update();
	AI_Update();
	HotPotato_Update();
	
	Character_DrawCharacters(characterCount);					// Draw 4 characters
	Character_CollisionStatus(player.cStateS);				// Write collision status for P1
}

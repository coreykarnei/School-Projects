// Tag.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the EE319K Lab 10

// Last Modified: 3/6/2015
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Backlight (pin 10) connected to +3.3 V
// MISO (pin 9) unconnected
// SCK (pin 8) connected to PA2 (SSI0Clk)
// MOSI (pin 7) connected to PA5 (SSI0Tx)
// TFT_CS (pin 6) connected to PA3 (SSI0Fss)
// CARD_CS (pin 5) unconnected
// Data/Command (pin 4) connected to PA6 (GPIO), high for data, low for command
// RESET (pin 3) connected to PA7 (GPIO)
// VCC (pin 2) connected to +3.3 V
// Gnd (pin 1) connected to ground

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "ADC.h"
#include "Timer0.h"
#include "Timer1.h"
#include "Timer2.h"
#include "Button0.h"
#include "Player.h"
#include "Screen.h"
#include "Bitmaps.h"
#include "Character.h"
#include "AI.h"
#include "HotPotato.h"
#include "DAC.h"
#include "Sound.h"
#include "Menu.h"



void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
void Master_Funk(void);
void SoundTest(void);


// *************************** Capture image dimensions out of BMP**********
int main(void){
  TExaS_Init(SSI0_Real_Nokia5110_Scope);  // set system clock to 80 MHz
  Random_Init(75);
  Output_Init();
	ADC0_Init();
	DAC_Init();
	Menu_Init();
	AI_Init(0x07);
	Player_Init();
	Sound_Init();
	HotPotato_Init();
	Music_Tropical();
	
	Button0_Init();
	Timer0_Init(Play, 80000000/11025);	// 11.025 kHz 80000000/11025
	//Timer1_Init(PlayM,80000000/11025);
	Timer2_Init(Master_Funk,  80000000/60);
//	SoundTest();

 /* ST7735_FillScreen(0x0000);            // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  LCD_OutDec(1234);	 */
  while(1){							
  } 
}
void Master_Funk(void){
	switch(menu){
		case 0: Menu_Start();
			break;
		case 1: Menu_Pregame();
			break;
		case 2: Menu_Game();
	}
}


void SoundTest(void){
	Delay100ms(10);
				Sound_Shoot();
	Delay100ms(20);
				Sound_Killed();
	Delay100ms(20);
				Sound_Explosion();
	Delay100ms(20);
				Sound_Fastinvader1();
	Delay100ms(20);
				Sound_Fastinvader2();
	Delay100ms(20);
				Sound_Fastinvader3();
	Delay100ms(20);
				Sound_Fastinvader4();
	Delay100ms(20);
				Sound_Highpitch();
	//Music_CrazyFrog34();

}
// You can use this timer only if you learn how it works
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 727240 = 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

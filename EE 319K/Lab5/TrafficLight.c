// TableTrafficLight.c
// Runs on LM4F120 or TM4C123
// Index implementation of a Moore finite state machine to operate
// a traffic light.
// Daniel Valvano, Jonathan Valvano
// July 20, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013
   Volume 1 Program 6.8, Example 6.4
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013
   Volume 2 Program 3.1, Example 3.1

 Copyright 2013 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// east facing red light connected to PE5
// east facing yellow light connected to PE4
// east facing green light connected to PE3
// north facing red light connected to PE2
// north facing yellow light connected to PE1
// north facing green light connected to PE0
// north facing car detector connected to PD1 (1=car present)
// east facing car detector connected to PD0 (1=car present)
// walk sensor PD2
// red walk light PF1
// green walk light PF3

#include "PLL.h"
#include "SysTick.h"
#include "tm4c123gh6pm.h"
#include "TExaS.h"

#define LIGHT                   (*((volatile unsigned long *)0x4002400C))
#define GPIO_PORTE_OUT          (*((volatile unsigned long *)0x4002400C)) // bits 5-0
#define GPIO_PORTB_IN           (*((volatile unsigned long *)0x400050FC)) // bits 1-0
#define SENSOR                  (*((volatile unsigned long *)0x400050FC))
#define SYSCTL_RCGC2_GPIOE      0x00000010		// port E Clock Gating Control
#define SYSCTL_RCGC2_GPIOB      0x00000002		// port B Clock Gating Control

// Linked data structure
struct State {
  unsigned long Out; 
  unsigned long Time;  
  unsigned long Next[8];}; 
typedef const struct State STyp;
#define goN   0
#define waitN 1
#define goE   2
#define waitE 3
#define walk	4
#define flash 5
STyp FSM[15]={
 {0x61, 100,{goN,		waitN,	goN,		waitN,	waitN,	waitN,	waitN,	waitN}}, 
 {0x62, 50, {goE,		goE,		goE,		goE,		walk,		goE,		walk,		goE}},
 {0x4C, 100,{goE,		goE,		waitE,	waitE,	waitE,	waitE,	waitE,	waitE}},
 {0x54, 50, {goN,		goN,		goN,		goN,		walk,		walk,		goN,		walk}},
 {0xA4, 300,{flash,	flash,	flash,	flash,	flash,	flash,	flash,	flash}},
 {0x24, 10, {6,			6,			6,			6,			6,			6,			6,			6}},
 {0x64, 10, {7,			7,			7,			7,			7,			7,			7,			7}},
 {0x24, 10, {8,			8,			8,			8,			8,			8,			8,			8}},
 {0x64, 10, {9,			9,			9,			9,			9,			9,			9,			9}},
 {0x24, 10, {10,		10,			10,			10,			10,			10,			10,			10}},
 {0x64, 10, {11,		11,			11,			11,			11,			11,			11,			11}},
 {0x24, 10, {12,		12,			12,			12,			12,			12,			12,			12}},
 {0x64, 10, {13,		13,			13,			13,			13,			13,			13,			13}},
 {0x24, 10, {14,		14,			14,			14,			14,			14,			14,			14}},
 {0x64, 10, {goN,		goE,		goN,		goN,		goN,		goE,		goN,		goN}}};
unsigned long S;															// index to the current state 
unsigned long Input; 
int main(void){ volatile unsigned long delay;
  PLL_Init();																	// 80 MHz, Program 10.1
  SysTick_Init();															// Program 10.2
	
  SYSCTL_RCGC2_R |= 0x32;											// 1) clock on ports B, E, F
  delay = SYSCTL_RCGC2_R;											// 2) no need to unlock
	
	GPIO_PORTB_AMSEL_R &= 0xFFFFFFF8;						// 3) disable analog function on PB0-2
  GPIO_PORTB_PCTL_R &= 0xFFFFFFF8;						// 4) enable regular GPIO
  GPIO_PORTB_DIR_R &= 0xFFFFFFF8;							// 5) inputs on PB0-2
  GPIO_PORTB_AFSEL_R &= 0xFFFFFFF8;						// 6) regular function on PB0-2
  GPIO_PORTB_DEN_R |= 0x07;										// 7) enable digital on PB0-2

  GPIO_PORTE_AMSEL_R &= 0xFFFFFFC0;						// 3) disable analog function on PE0-5
  GPIO_PORTE_PCTL_R &= 0xFFFFFFC0;						// 4) enable regular GPIO
  GPIO_PORTE_DIR_R |= 0x3F;										// 5) output on PE0-5
  GPIO_PORTE_AFSEL_R &= 0xFFFFFFC0;						// 6) regular function on PE0-5
  GPIO_PORTE_DEN_R |= 0x3F;										// 7) enable digital on PE0-5
	
	GPIO_PORTF_AMSEL_R &= 0xFFFFFFF5;						// 8) disable analog function on PF1, PF3
  GPIO_PORTF_PCTL_R &= 0xFFFFFFF5;						// 9) enable regular GPIO
  GPIO_PORTF_DIR_R |= 0x0A;										// 10) outputs on PF1, PF3
  GPIO_PORTF_AFSEL_R &= 0xFFFFFFF5;						// 11) regular function on PF1, PF3
  GPIO_PORTF_DEN_R |= 0x0A;										// 12) enable digital on PF1, PF3
	
  S = goN;

  while(1){
		GPIO_PORTE_DATA_R = FSM[S].Out;						// set lights
		GPIO_PORTF_DATA_R = (FSM[S].Out & 0x40) >> 5 | (FSM[S].Out & 0x80) >> 4;	//set PF1 and PF3 bits
    SysTick_Wait10ms(FSM[S].Time);
    Input = GPIO_PORTB_DATA_R & 7;						// read sensors
		S = FSM[S].Next[Input];
  }
}

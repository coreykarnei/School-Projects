// EdgeInterrupt.c
// Runs on LM4F120 or TM4C123
// Request an interrupt on the falling edge of PF4 (when the user
// button is pressed) and increment a counter in the interrupt.  Note
// that button bouncing is not addressed.
// Daniel Valvano
// September 14, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
   Volume 1, Program 9.4
   
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Volume 2, Program 5.6, Section 5.5

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// user button connected to PC4 (increment counter on falling edge)

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Menu.h"

// global variable visible in Watch window of debugger
// increments at least once per button press
volatile unsigned long FallingEdges = 0;
volatile unsigned short Button0Pressed = 0;
// ***************** Button0_Init ****************
// Activate button on PC4 to interrupt on falling edge
// Inputs: none
// Outputs: none
void Button0_Init(void){
  SYSCTL_RCGC2_R |= 0x04;   		// a) activate port C
  FallingEdges = 0;          		// b)initialize counter
  GPIO_PORTC_DIR_R &= ~0x10;    // c) make PC4 in
	GPIO_PORTC_DEN_R |= 0x10; 		//		enable digital I/O on PC4
  GPIO_PORTC_IS_R &= ~0x10;			// d) PC4 is edge-sensitive 
	GPIO_PORTC_IBE_R &= ~0x10;		//		PC4 is not both edges 
	GPIO_PORTC_IEV_R &= ~0x10;		// d) PC4 falline edge event 
  GPIO_PORTC_ICR_R = 0x10;			// e) clear flag4
  GPIO_PORTC_IM_R |= 0x10;    	// f) arm interrupt on PC4
  NVIC_PRI0_R = (NVIC_PRI0_R&0xFF00FFFF)|0x00A00000; // g) priority 5
// interrupts enabled in the main program after all devices initialized
// vector number 18, interrupt number 2
  NVIC_EN0_R = 1<<2;           	// h) enable IRQ 2 in NVIC
//  TIMER0_CTL_R = 0x00000001;  // 10?) enable TIMER0A
}

void GPIOPortC_Handler(void){
	GPIO_PORTC_ICR_R = 0x10;				// acknowledge flag4
	FallingEdges = FallingEdges + 1;
	Button0Pressed = 1;
 // (*PeriodicTask0)();                // execute user task
}

short Button0_Pressed(void){
	if(Button0Pressed){
		Button0Pressed=0;
		return 1;
	}
	return 0;
}


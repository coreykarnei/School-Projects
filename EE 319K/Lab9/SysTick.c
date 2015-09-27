// SysTick.h
// Runs on LM4F120
// Provide functions that initialize the SysTick module, wait at least a
// designated number of clock cycles, and wait approximately a multiple
// of 10 milliseconds using busy wait.  After a power-on-reset, the
// LM4F120 gets its clock from the 16 MHz precision internal oscillator,
// which can vary by +/- 1% at room temperature and +/- 3% across all
// temperature ranges.  If you are using this module, you may need more
// precise timing, so it is assumed that you are using the PLL to set
// the system clock to 50 MHz.  This matters for the function
// SysTick_Wait10ms(), which will wait longer than 10 ms if the clock is
// slower.
// Daniel Valvano
// October 25, 2012

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2012
   Program 2.11, Section 2.6

 Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
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


// Initialize SysTick with busy wait running at bus clock.
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include "UART1.h"
//#define NVIC_ST_CTRL_R      (*((volatile unsigned long *)0xE000E010))
//#define NVIC_ST_RELOAD_R    (*((volatile unsigned long *)0xE000E014))
//#define NVIC_ST_CURRENT_R   (*((volatile unsigned long *)0xE000E018))
#define PF2      					  (*((volatile uint32_t *)0x40025010))
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000007;      // enable SysTick with core clock
	NVIC_ST_RELOAD_R = 2000000-1; 	 // number of counts to wait (40hz)
  NVIC_ST_CURRENT_R = 0;      		 // any value written to CURRENT clears
}
// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}
// 10000us equals 10ms
void SysTick_Wait10ms(unsigned long delay){
  unsigned long i;
  for(i=0; i<delay; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}

/*	Used in lab 8
*uint32_t ADCMail;							// 12-bit ADC
*uint32_t ADCStatus;
*/
uint32_t TxCounter;							// number of ADC samples collected
void SysTick_Handler(){
	PF2 ^= 0x04;      						// 1. Toggle heartbeat 		
	uint32_t data = ADC_In();  		// 2. Sample ADC (12-bit channel 1)
	PF2 ^= 0x04;      						// 3. Toggle heartbeat
	data=Convert(data);						// 4. Convert to distance and create the 8-byte message
	uint8_t message[]= {0x02,0,0x2E,0,0,0,0x0D,0x03};
	message[1] = (data/1000)%10 + 0x30;
	message[3] = (data/100)%10 + 0x30;
	message[4] = (data/10)%10 + 0x30;
	message[5] = data%10 + 0x30;
	for(int i=0; i<8; i++){				// 5. Send the 8-byte message to the other computer (calls UART1_OutChar 8 times)
		UART1_OutChar(message[i]);
	}
	TxCounter++;									// 6. increment TxCounter (for debugging)
	PF2 ^= 0x04;   				   			// 7. Toggle heartbeat
	NVIC_ST_RELOAD_R = 2000000-1; // 8. Return from interrupt
  NVIC_ST_CURRENT_R = 0;				// Reset SysTick timer for .25ms
}


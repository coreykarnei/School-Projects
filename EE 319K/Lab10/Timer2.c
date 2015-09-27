// Timer2.c
// Runs on LM4F120/TM4C123
// Use TIMER2 in 32-bit periodic mode to request interrupts at a periodic rate
// Daniel Valvano
// Last Modified: 3/6/2015 
// You can use this timer only if you learn how it works

/* This example accompanies the book
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2015
  Program 7.5, example 7.6

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
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ADC.h"
#include "UART1.h"

void (*PeriodicTask2)(void);   // user function

// ***************** Timer0_Init ****************
// Activate TIMER2 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
// Outputs: none
void Timer2_Init(void(*task)(void), uint32_t period){
  SYSCTL_RCGCTIMER_R |= 0x04;   // 0) activate TIMER2
  PeriodicTask2 = task;         // user function
  TIMER2_CTL_R = 0x00000000;    // 1) disable TIMER2A during setup
  TIMER2_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER2_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER2_TAILR_R = period-1;    // 4) reload value
  TIMER2_TAPR_R = 0;            // 5) bus clock resolution
  TIMER2_ICR_R = 0x00000001;    // 6) clear TIMER2A timeout flag
  TIMER2_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI5_R = (NVIC_PRI5_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN0_R = 1<<23;           // 9) enable IRQ 23 in NVIC
  TIMER2_CTL_R = 0x00000001;    // 10) enable TIMER2A
}

/*void Timer2A_Handler(void){
  TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
  (*PeriodicTask2)();                // execute user task
}*/
uint32_t TxCounter;							// number of ADC samples collected
void Timer2A_Handler(void){
	uint32_t data2[2];
	ADC0_In(data2);
	uint32_t data = data2[0];  		// 2. Sample ADC (12-bit channel 1)
//	data=Convert(data);						// 4. Convert to distance and create the 8-byte message
	uint8_t message[]= {0x02,0,0x2E,0,0,0,0x0D,0x03};
	message[1] = (data/1000)%10 + 0x30;
	message[3] = (data/100)%10 + 0x30;
	message[4] = (data/10)%10 + 0x30;
	message[5] = data%10 + 0x30;
	for(int i=0; i<8; i++){				// 5. Send the 8-byte message to the other computer (calls UART1_OutChar 8 times)
		UART1_OutChar(message[i]);
	}
	TxCounter++;									// 6. increment TxCounter (for debugging)
	TIMER2_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout... Systick was using .25ms interrupts to transmit data
}

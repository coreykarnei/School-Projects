// Piano.c
// This software configures the off-board piano keys
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 8/25/2014 
// Last Modified: 3/6/2015 
// Section 1-2pm     TA: Wooseok Lee
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "tm4c123gh6pm.h"

// **************Piano_Init*********************
// Initialize piano key inputs, called once 
// Input: none 
// Output: none
void Piano_Init(void){
	volatile unsigned long delay;
	
	SYSCTL_RCGC2_R 			|= 0x10;						// 1) clock on port E
  delay = SYSCTL_RCGC2_R;									// 2) no need to unlock
	
	GPIO_PORTE_AMSEL_R 	&= ~0xF;						// 3) disable analog function on PE0-3
  GPIO_PORTE_PCTL_R 	&= ~0xF;						// 4) enable regular GPIO
  GPIO_PORTE_DIR_R 		&= ~0xF;						// 5) INPUTS on PE0-3
  GPIO_PORTE_AFSEL_R 	&= ~0xF;						// 6) regular function on PE0-3
  GPIO_PORTE_DEN_R 		|= 0xF;
}

// **************Piano_In*********************
// Input from piano key inputs 
// Input: none 
// Output: 0 to 7 depending on keys
// 0x01 is just Key0, 0x02 is just Key1, 0x04 is just Key2
uint32_t Piano_In(void){
	
  return GPIO_PORTE_DATA_R & 0xF; // replace this line with actual code
}

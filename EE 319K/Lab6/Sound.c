// Sound.c
// This module contains the SysTick ISR that plays sound
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
#include "dac.h"
#include "tm4c123gh6pm.h"
#include "piano.h"
#include "SysTick.h"



uint32_t wave[] = {8,9,11,12,13,14,14,15,15,15,14,14,13,12,11,9,8,7,5,4,3,2,2,1,1,1,2,2,3,4,5,7}; //32 numbers
//uint32_t wave[] = {5,5,4,1,1,3,8,11,11,9,4,2,5,11,15,13,9,7,5,5,6,8,8,7,4,3,3,3,3,4,5,5};				//guitar
unsigned long wave_index;
// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(uint32_t period){	//34 peices of each array
	DAC_Init();
	wave_index=0;
	SysTick_Init();
	SysTick_Wait10ms(1);
	
}


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none
void Sound_Play(uint32_t period){ 
	
	switch(Piano_In())
	{
		case 1:
				SysTick_Wait(4664);
				DAC_Out(wave[wave_index]);
				break;
		case 2:
				SysTick_Wait(4132);
				DAC_Out(wave[wave_index]);				
				break;
		case 4:
				SysTick_Wait(3681);	
				DAC_Out(wave[wave_index]);				
				break;
		case 8:
				SysTick_Wait(3071);
				DAC_Out(wave[wave_index]);				
				break;
  } 
	SysTick_Wait(2);
}

void SysTick_Handler(){
		GPIO_PORTB_DATA_R |= 0x10;
		if(wave_index <31){ 
			wave_index++;
		}
		else wave_index = 0;
		GPIO_PORTB_DATA_R &= ~0x10;
		Sound_Play(0);
		//SysTick_Wait(4780);
}

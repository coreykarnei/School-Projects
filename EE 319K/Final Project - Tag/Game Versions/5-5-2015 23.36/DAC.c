// DAC_Init initializes DAC on ports B0-3, and DAC_Out outputs 4 bit data 
// Michael Darden mkd788 & Brent Atchison bma862, 4/28/2015

#include <stdint.h>
#include "tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC, called once 
// Input: none
// Output: none
void DAC_Init(void){
	volatile unsigned long delay;
	
	SYSCTL_RCGC2_R 			|= 0x2;							// 1) clock on ports B
  delay = SYSCTL_RCGC2_R;									// 2) no need to unlock
	
	GPIO_PORTB_AMSEL_R 	&= ~0x1F;						// 3) disable analog function on PB0-3
  GPIO_PORTB_PCTL_R 	&= ~0x1F;						// 4) enable regular GPIO
	GPIO_PORTB_DR8R_R   |= 0x1F;
  GPIO_PORTB_DIR_R 		|= 0x1F;						// 5) Outputs on PB0-3
  GPIO_PORTB_AFSEL_R 	&= ~0x1F;						// 6) regular function on PB0-3
  GPIO_PORTB_DEN_R 		|= 0x1F;
}

// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(uint32_t data){
																					//change only the last 4 bits
	GPIO_PORTB_DATA_R = (GPIO_PORTB_DATA_R & ~0xF) | (data & 0xF);
}

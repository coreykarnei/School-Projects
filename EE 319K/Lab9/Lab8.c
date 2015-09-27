// Lab8.c
// Runs on LM4F120 or TM4C123
// Brent Atchison bma862 + Michael Darden mkd788
// Last modification date: 4/16/2015
// Last Modified: 3++/16/2015 

// Analog Input connected to PE2=ADC1
// displays on Sitronox ST7735
// PF3, PF2, PF1 are heartbeats

#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "UART1.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

#define PF1       (*((volatile uint32_t *)0x40025008))
#define PF2       (*((volatile uint32_t *)0x40025010))
#define PF3       (*((volatile uint32_t *)0x40025020))
// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x20;
	while ((SYSCTL_PRGPIO_R&0x20) == 0){};
	GPIO_PORTF_AMSEL_R &= ~0x0E;
	GPIO_PORTF_AFSEL_R &= ~0x0E;
	GPIO_PORTF_PCTL_R = 0x00;
	GPIO_PORTF_DIR_R |= 0x0E;
	GPIO_PORTF_DEN_R |= 0x0E;
}

int main1(void){
	TExaS_Init();
	ADC_Init();         // turn on ADC, set channel to 1
	ST7735_InitR(INITR_REDTAB); 
  PortF_Init();
	SysTick_Init();			// This makes things work
	UART1_Init();
	while(1){}					// do-nothing loop
}

#include "FIFO.h"
int Status[20];             // entries 0,7,12,19 should be false, others true
unsigned char GetData[10];  // entries 1 2 3 4 5 6 7 8 should be 1 2 3 4 5 6 7 8
int main2(void){
  FIFO_Init(); // initializes a FIFO that holds 6 elements 
  for(;;){
    Status[0]  = FIFO_Get(&GetData[0]);  // should fail,    empty
    Status[1]  = FIFO_Put(1);            // should succeed, 1 
    Status[2]  = FIFO_Put(2);            // should succeed, 1 2
    Status[3]  = FIFO_Put(3);            // should succeed, 1 2 3
    Status[4]  = FIFO_Put(4);            // should succeed, 1 2 3 4
    Status[5]  = FIFO_Put(5);            // should succeed, 1 2 3 4 5
    Status[6]  = FIFO_Put(6);            // should succeed, 1 2 3 4 5 6
    Status[7]  = FIFO_Put(7);            // should fail,    1 2 3 4 5 6 
    Status[8]  = FIFO_Get(&GetData[1]);  // should succeed, 2 3 4 5 6
    Status[9]  = FIFO_Get(&GetData[2]);  // should succeed, 3 4 5 6
    Status[10] = FIFO_Put(7);            // should succeed, 3 4 5 6 7
    Status[11] = FIFO_Put(8);            // should succeed, 3 4 5 6 7 8
    Status[12] = FIFO_Put(9);            // should fail,    3 4 5 6 7 8 
    Status[13] = FIFO_Get(&GetData[3]);  // should succeed, 4 5 6 7 8
    Status[14] = FIFO_Get(&GetData[4]);  // should succeed, 5 6 7 8
    Status[15] = FIFO_Get(&GetData[5]);  // should succeed, 6 7 8
    Status[16] = FIFO_Get(&GetData[6]);  // should succeed, 7 8
    Status[17] = FIFO_Get(&GetData[7]);  // should succeed, 8
    Status[18] = FIFO_Get(&GetData[8]);  // should succeed, empty
    Status[19] = FIFO_Get(&GetData[9]);  // should fail,    empty
  }
}

int main(void){
	TExaS_Init();
  ADC_Init();         // turn on ADC, set channel to 1
	ST7735_InitR(INITR_REDTAB);
	PortF_Init();
	SysTick_Init();			// This makes things work
	UART1_Init();

	FIFO_Init();
	unsigned char data;
	while(1){
			while(data != 0x02){
					FIFO_Get(&data);					// Look for new data byte
			}
			ST7735_SetCursor(0,0);
			for(int i = 0; i<5; i++){			// Print next 5 elements
					FIFO_Get(&data);
					ST7735_OutChar(data);
			}
			ST7735_OutString(" cm");			// Print units
	}
}


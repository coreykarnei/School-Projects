
#include "tm4c123gh6pm.h"
#include "FIFO.h"
#define PF2      					  (*((volatile uint32_t *)0x40025010))
int ErrorCount;
//------------UART1_Init------------
// Initialize UART1 at 100,000 baud rate (assuming 80 MHz bus clock). Uses PC4 and PC5
// Input: none
// Output: none
void UART1_Init (void){
	SYSCTL_RCGCUART_R |= 0x0002;	// activate UART1
	SYSCTL_RCGCGPIO_R |= 0x0004;	// activate port C
	UART1_CTL_R &= ~0x0001;				// disable UART
	UART1_IBRD_R = 50;
// IBRD = int(80,000,000/(16*100,000)) = int(50.0)
	UART1_FBRD_R = 0;
// FBRD = round(0.0 * 64) = 0
	UART1_LCRH_R = 0x0070;				// 8-bit length, enable FIFO
	GPIO_PORTC_AFSEL_R |= 0x30;		// alt function on PC4,5
	GPIO_PORTC_PCTL_R = 
					(GPIO_PORTC_PCTL_R&0xFF00FFFF)+0x00220000;
	GPIO_PORTC_DEN_R |= 0x30;			// digital I/O on PC5,4
	GPIO_PORTC_AMSEL_R &= ~0x30;	// No analog on PA1-0
		//----------New Stuff---------------
	UART1_IM_R |= 0x10;
	UART1_IFLS_R = (UART1_IFLS_R&(~0x0038)) + 0x10;
	UART1_CTL_R |= 0x0301;				// enable RXE, TXE and UART
	NVIC_PRI1_R = (NVIC_PRI1_R&0xFF00FFFF) | 0x00400000; // bits 21-23
	NVIC_EN0_R |= 0x40;						// enable interrupt 6 in NVIC
	FIFO_Init();									// Initialize FIFO
	ErrorCount = 0;
}


//------------UART1_OutChar------------
// Send one byte using Busy-wait synchronization
// Input: Byte to write
// Output: none
void UART1_OutChar(uint8_t data){
	while((UART1_FR_R&0x0020) != 0){};
		// wait until TXFF is 0
	UART1_DR_R = data;
}

int RxCounter;
void UART1_Handler(){
	//UART1_ICR_R = 0x10;
	PF2 ^= 0x04;      							// 1. Toggle heartbeat 		
	PF2 ^= 0x04;      							// 2. Toggle heartbeat
	while((UART1_FR_R&0x0010) == 0)
		if(FIFO_Put(UART1_DR_R&0xFF)==(-1))
			ErrorCount++;
		// a) Read bytes from UART1_DR_R 
		// b) Put all bytes into your software FIFO (should be exactly 8 bytes, but could be more)
		// c) the message will be interpreted in the main program
	
	RxCounter++;
	UART1_ICR_R = 0x10;
	PF2 ^= 0x04;   				   			// 7. Toggle heartbeat
}


#include "tm4c123gh6pm.h"
#include "Character.h"
#include "FIFO.h"
#include "ST7735.h"
#include "UART1.h"
#include "Screen.h"
#include "AI.h"
short multiplayer;

void TwoPlayer_Init(short data){
	multiplayer=data;
}
uint8_t TwoPlayer_MasterRequest(void){
	// ask to be master
	// answer slave request
	// send map number
	// send AI count
	// send AI difficulty
	short data;
	UART1_OutChar(0xFE);
	UART1_OutChar(0xDC);
	FIFO_Get(&data);
	if(data == 0x56){			// check if slave is ready to receive
		FIFO_Get(&data);
		if(data == 0x78){

			UART1_OutChar(currentMap);					// send map number
			UART1_OutChar(aiCount);							// send AI count
			UART1_OutChar(aiDifficulty);				// send AI difficulty
			UART1_OutChar(0x00);
			return 1;														
		}		
	}
	return 0;																// not connected
}
uint8_t TwoPlayer_SlaveRequest(void){
	// ask to be slave
	// answer master request
	// receive map
	// receive AI count
	// Receive difficulty
	short data;
	UART1_OutChar(0x56);
	UART1_OutChar(0x78);
	FIFO_Get(&data);
	if(data == 0xFE){			// check if master is ready to send
		FIFO_Get(&data);
		if(data == 0xDC){
				FIFO_Get(&data);									// receive map
				currentMap=data;
				FIFO_Get(&data);									// receive AI count
				aiCount=data;
				FIFO_Get(&data);									// Receive difficulty
				aiDifficulty=data;
				FIFO_Get(&data);									// balance FIFO
			return 1;
		}		
	}
	return 0;
}

// take input from slave for Character[1]
void TwoPlayer_Master(void){
	short data;
	FIFO_Get(&data);
	if(data == 0x12){			// check if slave is ready to receive
		FIFO_Get(&data);
		if(data == 0x34){
			// Output  master intro message
			UART1_OutChar(0xAB);
			UART1_OutChar(0xCD);
			for(int i = 0; i < characterCount; i++){
				//Output x Coordinate
				UART1_OutChar(Characters[i].x&0x000000FF);
				UART1_OutChar((Characters[i].x&0x0000FF00)>>8);
				//Output y Coordinate
				UART1_OutChar(Characters[i].y&0x000000FF);
				UART1_OutChar((Characters[i].y&0x0000FF00)>>8);
				//Output active
				UART1_OutChar(Characters[i].active&0x000000FF);
				UART1_OutChar((Characters[i].active&0x0000FF00)>>8);
			}
		}
	}
}

// receive all character x-y coordinates and Active data from master
void TwoPlayer_Slave(void){
	short data;
	short data2;
	FIFO_Get(&data);
	if(data == 0xAB){											// Check 3 byte key
		FIFO_Get(&data);
		if(data == 0xCD){
			//for(int i = 0; i < characterCount; i++){
				FIFO_Get(&data);									// receive and print x
				FIFO_Get(&data2);
				data+=(data2<<8);
				ST7735_SetCursor(0,3);
				ST7735_OutString("     - x");			// Print units
				ST7735_SetCursor(0,3);
				LCD_OutDec(data);
				Characters[1].x=data;
				
				FIFO_Get(&data);									// receive and print y
				FIFO_Get(&data2);
				data+=(data2<<8);
				//ST7735_SetCursor(0,4);
			//	ST7735_OutString("     - y");			// Print units
				//ST7735_SetCursor(0,4);
			//	LCD_OutDec(data);
				Characters[1].y=data;
				
				FIFO_Get(&data);									// receive and print y
				FIFO_Get(&data2);
				data+=(data2<<8);
				Characters[1].active=data;
				
				UART1_OutChar(0x12);							// ask for a new message
				UART1_OutChar(0x34);
			//}
		}
	}
}

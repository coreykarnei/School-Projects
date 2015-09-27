#ifndef UART1_H
#define UART1_H
// Initialize UART1 on PC5 and PC4
void UART1_Init (void);
// Send one byte using Busy-wait synchronization
void UART1_OutChar(uint16_t data);

extern int ErrorCount;
#endif

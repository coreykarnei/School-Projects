// Initialize UART1 on PC5 and PC4
void UART1_Init (void);
// Send one byte using Busy-wait synchronization
void UART1_OutChar(uint8_t data);

extern int ErrorCount;

//initializes the FIFO
void FIFO_Init(void);

// stores 1 byte to the FIFO, returns -1 if fails
int FIFO_Put(short data);

// gets 1 byte from the FIFO, returns -1 if fails
int FIFO_Get(short* data);


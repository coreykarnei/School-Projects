#ifndef TWOPLAYER_H
#define TWOPLAYER_H
extern short multiplayer;
void TwoPlayer_Init(short data);
uint8_t TwoPlayer_MasterRequest(void);
uint8_t TwoPlayer_SlaveRequest(void);
void TwoPlayer_Master(void);
void TwoPlayer_Slave(void);
#endif

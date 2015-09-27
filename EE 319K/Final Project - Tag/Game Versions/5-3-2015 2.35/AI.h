#ifndef AI_H
#define AI_H
// ******************** Functions *********************
//------------AI_Init------------
// Initialize AI by setting up character struct
// Input: uint8_t containing bits of AI to be initialized (i.e. 0x01 for ai1, 0x02 for ai2, 0x04 for ai3, etc.)
// Output: none
extern void AI_Init(uint8_t aiSelect);
//------------AI_Update------------
// Decide which which AI each computer character will follow
// Input: none
// Output: none
extern void AI_Update(void);
void AI_AI1(Character *data);
#endif

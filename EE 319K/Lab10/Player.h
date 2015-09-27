#ifndef PLAYER_H
#define PLAYER_H

#include "Character.h"
#define player Characters[p1]
//extern Character player;
// ******************** Functions *********************
//------------Player_Init------------
// Initialize player by setting up character struct
// Input: none
// Output: none
void Player_Init(void);
//------------Player_InitP------------
// Initialize player by setting up character struct with parameters
// Input: sprite pointer, x coordinate, y coordinate, speed, hp
// Output: none
void Player_InitP(Bitmap* bmp, short x, short y, short spd, short hp);
//------------Player_Update------------
// Update player by calling other functions accordingly
// Input: none
// Output: none
void Player_Update(void);
//------------Player_Move------------
// Change coordinates based on analog input
// Input: none
// Output: none
void Player_Move(void);
//------------Player_UpdateVelocity------------
// Change x and y velocity based on ADC input
// Input: none
// Output: none
void Player_UpdateVelocity(void);
#endif

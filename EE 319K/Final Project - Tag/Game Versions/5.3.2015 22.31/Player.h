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
#endif

#ifndef HOTPOTATO_H
#define HOTPOTATO_H

// *********************** Functions *************************
//------------HotPotato_Init------------
// Setup hot-potato game mode elements
// Input: none
// Output: none
void HotPotato_Init(void);
//------------HotPotato_Update------------
// Update hot-potato game mode elements
// Input: none
// Output: none
void HotPotato_Update(void);
//------------HotPotato_WhosIt------------
// Find out which character is it (mainly for AI)
// Input: none
// Output: index of "it" character
int HotPotato_WhosIt(void);
//------------HotPotato_SetIt------------
// Set "it" index to input
// Input: index of new "it" character
// Output: none
void HotPotato_SetIt(int data);
//------------HotPotato_Cooldown------------
// Step down the tag-back cooldown on "it" character
// Input: none
// Output: none
void HotPotato_Cooldown(void);
//------------HotPotato_Burn------------
// Burns the player holding the potato, lowering their hp every clock interupt
// Input: none
// Output: none
void HotPotato_Burn(void);
//------------HotPotato_DrawStatus------------
// Draws p1 status elements on the top of the screen
// Input: none
// Output: none
void HotPotato_DrawStatus(void);
//------------HotPotato_Eliminate------------
// Kills off characters with 0 hp
// Input: none
// Output: none
void HotPotato_Eliminate(void);
#endif

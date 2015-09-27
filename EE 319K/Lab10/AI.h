#ifndef AI_H
#define AI_H

extern short aiCount;
extern short aiDifficulty;
// ******************** Functions *********************
//------------AI_Init------------
// Initialize AI by setting up character structs
// Input: number of ai to initialize and thir difficulty level
// Output: none
extern void AI_Init(void);
//------------AI_Update------------
// Decide which which AI each computer character will follow
// Input: none
// Output: none
extern void AI_Update(void);
//------------AI_AI1------------
// Decide Moves AI randomly, accounting for collisions
// Input: AI character pointer
// Output: none
void AI_AI1(Character *data);
//------------AI_AI2------------
// Runs away from "it"; if "it, chases nearest character
// Input: AI character pointer
// Output: none
void AI_AI2(Character *data);
//------------AI_AI4------------
// Runs away from "it" - If "it", chases player (velocity based)
// Input: AI character pointer
// Output: none
void AI_AI4(Character *data);
#endif

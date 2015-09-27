#ifndef POWERUP_H
#define POWERUP_H
#include "Bitmaps.h"
#include "Character.h"

// ******************* Character Structure ******************
typedef struct Powerup_struct{
	Bitmap *bmp;							// Pointer to Bitmap struct containing a bitmap length, width, and array
	short width;							// width (for collisions, may be different then sprite width)
	short height;							// height (for collisions, may be different then sprite height)
	short x;									// x-coordinate
	short y;									// y coordinate
	short active;							// dicatates whether or not the powerup is in play (1 is on, 0 is off)
	void (*function)(Character*);	// function dictating what power up does
} Powerup;
// Access definitions
// NOTE: access definitions start lower-case, actual arrays start upper-case
typedef enum{
  noUp,
	speedUp,
	slowDown,
	powerupCount	// Not associated with any powerup, just tracks amount of characters
} powerups;
// Array cointaining all available powerups
extern Powerup Powerups[powerupCount];

// *********************** Functions *************************
void Powerup_Init(void);
//------------Powerup_Generate------------
// Activate a new power up
// Input: none
// Output: none
void Powerup_Generate(void);
//------------Powerup_Draw------------
// Draw a powerup onto the screen
// Input: powerup struct to be drawn
// Output: none
void Powerup_Draw(Powerup data);
//------------Powerup_DrawPowerups------------
// Draw active powerups to the screen
// Input: Amount of powerups to be drawn
// Output: none
void Powerup_DrawPowerups(void);
//------------Powerup_SpeedUp------------
// Doubles characters speed temporarily
// Input: character to be altered
// Output: none
void PowerUp_SpeedUp(Character *data);
//------------Powerup_SlowDown------------
// lowers characters speed temporarily
// Input: character to be altered
// Output: none
void PowerUp_SlowDown(Character *data);

#endif

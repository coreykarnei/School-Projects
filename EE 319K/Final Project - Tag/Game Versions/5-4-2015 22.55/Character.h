#ifndef CHARACTER_H
#define CHARACTER_H
#include "Bitmaps.h"
#define hot short
// ******************* Character Structure ******************
typedef struct Character_struct{
	Bitmap *bmp;					// Pointer to Bitmap struct containing a bitmap length, width, and array
	short x;								// x-coordinate
	short y;								// y coordinate
	short width;						// width (for collisions, may be differnt then sprite width)
	short height;						// height (for collisions, may be differnt then sprite height)
	short spd;							// movement speed
	short mass;							// mass for collisions
	short cStateP;					// player Collision state
	short cStateS;					// structure Collision state
	hot potato;							// "it" state
	short hp;								// health points
	short active;						// dicatates whether or not the character is in play (1 is on, 0 is off)
} Character;

// Access definitions
// NOTE: access definitions start lower-case, actual arrays start upper-case
typedef enum{
	p1, p2, p3, p4, 
	characterCount	// Not associated with any character, just tracks amount of characters
} characters;
// Array cointaining all available characters
extern Character Characters[characterCount];		

// *********************** Functions *************************
//------------Character_Draw------------
// Draw a character onto the screen
// Input: Character struct to be drawn
// Output: none
void Character_Draw(Character data);
//------------Character_DrawCharacters------------
// Draw specified amount of Characters to the screen
// Input: Amount of characters to be drawn
// Output: none
void Character_DrawCharacters(int data);
//------------Character_Collision------------
// Compare two characters to tell where they collide
// Input: Two characters to be compared
// Output: (RELATIVE TO CHAR1) 0=no collision, 1=bottom-left, 2=mid-left, 3=top-left, 4=top-mid, 5=top-right, 6=mid-right, 7=bottom-right, 8=bottom-mid, 9=other collision
int Character_Collision(Character char1, Character char2);
//------------Character_UpdateCollisions------------
// Update collision status for all characters
// Input: none
// Output: none
void Character_UpdateCollisions(void);
//------------Character_UpdateCollisionsS------------
// Update structure collision status for all characters (Map borders, structures)
// Input: none
// Output: none
void Character_UpdateCollisionsS(void);
//------------Character_CollisionStatus------------
// Print the collision status for the selected character in the top left of the screen
// Input: Character to be evaluated
// Output: none
void Character_CollisionStatus(short data);
#endif

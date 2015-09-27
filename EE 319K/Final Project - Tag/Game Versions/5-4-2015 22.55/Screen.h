#ifndef SCREEN_H
#define SCREEN_H

extern int frameXCord;
extern int frameYCord;
extern int mapWidth;
extern int mapHeight;
extern int screenHeight;
void Screen_Update(void);
void Screen_Get(void);
void Screen_Move(void);
void Screen_Draw(void);

#endif

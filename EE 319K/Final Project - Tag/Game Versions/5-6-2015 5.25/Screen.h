#ifndef SCREEN_H
#define SCREEN_H

extern int frameXCord;
extern int frameYCord;
extern int screenHeight;
extern int mapScale;
extern int currentMap;
void Screen_Update(void);
void Screen_Get(void);
void Screen_Move(void);
void Screen_Draw(void);

#endif

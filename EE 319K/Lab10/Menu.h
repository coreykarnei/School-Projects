#ifndef MENU_H
#define MENU_H
extern short menu;
extern int debug;

void Menu_Init(void);
void Menu_Start(void);
void Menu_Pregame(void);
void Menu_Game(void);
void Delay100ms(uint32_t count);

#endif

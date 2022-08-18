#include <stdio.h>
#include "DMA.h"
#include "text.h"

extern volatile unsigned short *videoBuffer;


#define REG_DISPCTL (*(volatile unsigned short *)0x4000000)
#define MODE3 (3)
#define BG2 (1<<10)
#define SCREEN_HEIGHT 160
#define SCREEN_WIDTH 240

#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))

#define COLOR(r, g, b) ((b)<<10 | (g)<<5 | (r))
#define WHITE COLOR(31,31,31)
#define BLACK COLOR(0,0,0)
#define RED COLOR(31,0,0)
#define BLUE COLOR(0,0,31)
#define GREEN COLOR(0,31,0)
#define GREY COLOR(16,16,16)
#define YELLOW COLOR(31,31,0)
#define CYAN COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)


#define SCANLINECOUNTER *(volatile short *)0x4000006

// Buttons
#define BUTTONS *(volatile unsigned int *)0x4000130

#define BUTTON_A 0
#define BUTTON_B 1
#define BUTTON_SELECT 2
#define BUTTON_START 3
#define BUTTON_RIGHT 4
#define BUTTON_LEFT 5
#define BUTTON_UP 6
#define BUTTON_DOWN 7
#define BUTTON_L 8
#define BUTTON_R 9

#define NBUTTONS (10)

#define KEY_DOWN(key, buttons) (~(buttons) & (1<<key))

#define MAX_BALLS (8)



//Prototypes
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int width, int height, unsigned short color);
void delay(int n);
void waitForVBlank(void);
int rand(void);


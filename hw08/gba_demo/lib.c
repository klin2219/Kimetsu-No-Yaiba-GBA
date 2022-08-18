#include "lib.h"

volatile unsigned short *videoBuffer = (unsigned short *)0x6000000;


void setPixel(int row, int col, unsigned short color) {
        *(videoBuffer + OFFSET(row, col, SCREEN_WIDTH) ) = color;

}

void drawRect(int row, int col, int width, int height, unsigned short color) {

        volatile unsigned short lcolor;
        lcolor = color;
        for(int r=0; r<height; r++) {
//              for(int c=0; c<width; c++)
//                      setPixel(row+r, col+c, color);

                DMA[3].src = &lcolor;
                DMA[3].dst = &videoBuffer[OFFSET(row+r,col,SCREEN_WIDTH)];
                DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED | DMA_DESTINATION_INCREMENT;
        }
}

void delay(int n) {
        // delay for n tenths of a second
        volatile int x = 0;
        for (int i=0; i<n*8000; i++)
                x++;
}

void waitForVBlank(void) {
        while (SCANLINECOUNTER> 160)
                ;
        while (SCANLINECOUNTER < 160)
                ;
}

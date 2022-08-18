
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


//Prototypes
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int width, int height, unsigned short color);


// Bouncing ball demo
int main(int argc, char *argv[]) {
	REG_DISPCTL = MODE3 | BG2;

	drawRect(30, 40, 5, 5, RED);

}



volatile unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color) {
        *(videoBuffer + OFFSET(row, col, SCREEN_WIDTH) ) = color;

}

void drawRect(int row, int col, int width, int height, unsigned short color) {
        for(int r=0; r<height; r++) {
              for(int c=0; c<width; c++)
                      setPixel(row+r, col+c, color);

        }
}


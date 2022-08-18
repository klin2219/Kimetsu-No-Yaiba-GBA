
#define REG_DISPCTL (*(volatile unsigned short *)0x4000000)
#define MODE3 (3)
#define BG2 (1<<10)
#define SCREEN_HEIGHT 160
#define SCREEN_WIDTH 240

#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))

//Prototypes
void setPixel(int row, int col, unsigned short color);


// Bouncing ball demo
int main(int argc, char *argv[]) {
	REG_DISPCTL = MODE3 | BG2;

	setPixel(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, 0x7fff);

}



volatile unsigned short *videoBuffer = (unsigned short *)0x6000000;

void setPixel(int row, int col, unsigned short color) {
        *(videoBuffer + OFFSET(row, col, SCREEN_WIDTH) ) = color;

}


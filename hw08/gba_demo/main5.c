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
int rand(void);


// Bouncing ball demo
int main(int argc, char *argv[]) {
	REG_DISPCTL = MODE3 | BG2;

	// What it takes to define a ball
	struct ball {
		int row;
		int col;
		int rd;
		int cd;
		unsigned short color;
	};

	struct ball ball;
	
	int size;

	// initialize the parameters for bouncing
	size = 5;
	struct ball *bp = &ball;
	bp->row = 0;
	bp->col = 0;
	bp->rd = 1;
	bp->cd = 1;
	bp->color = RED;

	// Run forever (or until the power runs out)
	while (1) {

		// See if we need to bounce off a wall
		bp = &ball;
		bp->row = bp->row + bp->rd;
		bp->col += bp->cd;
		if(bp->row < 0) {
			bp->row = 0;
			bp->rd = -bp->rd;
		}
		if(bp->row > SCREEN_HEIGHT) {
			bp->row = SCREEN_HEIGHT;
			bp->rd = -bp->rd;
		}
		if(bp->col < 0) {
			bp->col = 0;
			bp->cd = -bp->cd;
			}
		if(bp->col > SCREEN_WIDTH) {
			bp->col = SCREEN_WIDTH;
			bp->cd = -bp->cd;
		}

		// Draw the ball
		drawRect(bp->row, bp->col,
			size, size,
			bp->color);

	} // Game Loop

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


#include "DMA.h"

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


// Bouncing ball demo
int main(int argc, char *argv[]) {
	REG_DISPCTL = MODE3 | BG2;

	int buttonWasDown[NBUTTONS] = {0};
	int buttonJustReleased[NBUTTONS] = {0};

	int deltas[] = { -3, -2, -1, 1, 2, 3 };
	int ndeltas = sizeof(deltas) / sizeof(deltas[0]);
	int colors[] = { RED, BLUE, GREEN, WHITE, CYAN, YELLOW, MAGENTA };
	int ncolors = sizeof(colors) / sizeof(colors[0]);
	unsigned short bgcolor = BLACK;

	enum gamestate {
		GS_INITBOUNCE,
		GS_BOUNCE
	};

	// What it takes to define a ball
	struct ball {
		int row;
		int col;
		int rd;
		int cd;
		unsigned short color;
	};

	// All the info for the current state of the game
	struct state {
		enum gamestate gamestate;
		int size;
		int nballs;
		struct ball balls[MAX_BALLS];
	};

	struct ball *bp, *obp;

	// current and previous states
	struct state cs, ps;

	cs.gamestate = GS_INITBOUNCE;

	// Run forever (or until the power runs out)
	while (1) {

		// Keep track of the state we're about to overwrite
		ps = cs;

		// Update button status
		for (int i=0; i<NBUTTONS; i++) {
			buttonJustReleased[i] = KEY_DOWN(i, BUTTONS) == 0 && buttonWasDown[i];
			buttonWasDown[i] = KEY_DOWN(i, BUTTONS) != 0;
		}

		// Compute the graphics needed for the current state
		switch (cs.gamestate) {

		// Balls are bouncing — act on buttons calculate positions
		case GS_BOUNCE:
			// See if we need to do something with buttons
			if (buttonJustReleased[BUTTON_UP]) {
				cs.size++;
				if(cs.size>150) {
					cs.size = 150;
				}
			}
			if (buttonJustReleased[BUTTON_DOWN]) {
				cs.size--;
				if(cs.size<3) {
					cs.size = 3;
				}
			}
			if(buttonJustReleased[BUTTON_LEFT]) {
				if (cs.nballs > 1)
					cs.nballs--;
			}
			if(buttonJustReleased[BUTTON_RIGHT]) {
				if (cs.nballs < MAX_BALLS)
					cs.nballs++;
			}
			// See if we need to bounce off a wall
			for (int i = 0; i < cs.nballs; i++) {
				bp = &cs.balls[i];
				bp->row = bp->row + bp->rd;
				bp->col += bp->cd;
				if(bp->row < 0) {
					bp->row = 0;
					bp->rd = -bp->rd;
				}
				if(bp->row > SCREEN_HEIGHT-cs.size) {
					bp->row = SCREEN_HEIGHT - cs.size;
					bp->rd = -bp->rd;
				}
				if(bp->col < 0) {
					bp->col = 0;
					bp->cd = -bp->cd;
				}
				if(bp->col > SCREEN_WIDTH-cs.size) {
					bp->col = SCREEN_WIDTH - cs.size;
					bp->cd = -bp->cd;
				}
			}
			break;

		case GS_INITBOUNCE:
			// initialize the parameters for bouncing
			cs.nballs = MAX_BALLS;
			cs.size = 5;
			for (int i = 0; i < cs.nballs; i++) {
				cs.balls[i].row = rand() % SCREEN_HEIGHT;
				cs.balls[i].col = rand() % SCREEN_WIDTH;
				cs.balls[i].rd = deltas[rand() % ndeltas];
				cs.balls[i].cd = deltas[rand() % ndeltas];
				cs.balls[i].color = colors[i % ncolors];
			}
			break;

		}

		// Wait for a Vblank to update the screen
		waitForVBlank();

		// Do the drawing and choose the next state
		switch (cs.gamestate) {

		// Draw the initial splash screen image
		// Initialize the balls for bouncing
		case GS_INITBOUNCE:
			cs.gamestate = GS_BOUNCE;
			break;

		// Bounce the balls around
		case GS_BOUNCE:
			// Erase the old balls
			for (int i = 0; i < ps.nballs; i++) {
				obp = &ps.balls[i];
				drawRect(obp->row, obp->col,
					ps.size, ps.size,
					bgcolor);
			}

			// Draw the new balls
			for (int i = 0; i < cs.nballs; i++) {
				bp = &cs.balls[i];
				drawRect(bp->row, bp->col,
					cs.size, cs.size,
					bp->color);
			}

			break;

		}

	} // Game Loop

}



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

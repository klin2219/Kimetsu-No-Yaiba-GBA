#include "lib.h"

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
	int score = 0;

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
					score++;
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

			// Erase and draw the new score
			char buffer[51];
			sprintf(buffer, "Score: %d", score);
			drawRect(150, 5, 200, 10, bgcolor);
			drawString(150, 5, buffer, YELLOW);

			break;

		}

	} // Game Loop

}

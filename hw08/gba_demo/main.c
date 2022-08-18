#define REG_DISPCTL (*(volatile unsigned short *)0x04000000)
#define SCANLINECOUNTER (*(volatile unsigned short *)0x04000006)
#define MODE3 (3)
#define BG2 (1<<10)

#define SCREEN_HEIGHT 160
#define SCREEN_WIDTH 240

#define OFFSET(r, c, rowlen) ((r)*(rowlen)+(c))

#define COLOR(r, g, b) ((b) << 10 | (g) << 5 | (r))
#define WHITE COLOR(31, 31, 31)
#define RED COLOR(31, 0, 0)
#define BLACK COLOR(0, 0, 0)
#define GREEN COLOR(0, 31, 0)
#define BLUE COLOR(31, 0, 0)

#define BUTTONS (*(volatile unsigned int*)0x4000130)

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

#define MAX_BALLS (10);

// Prototypes
void setPixel(int row, int col, unsigned short color);
void drawRect(int row, int col, int width, int height, unsigned short color);
void delay(int n);
int rand(void);
void waitForVBlank(void);

volatile unsigned short *videoBuffer = (unsigned short *)0x6000000;

// Bouncing ball demo
int main (int argc, char *argv[]) {
    REG_DISPCTL = MODE3 | BG2;

    int buttonWasDown[NBUTTONS] = {0};
    int buttonJustReleased[NBUTTONS] = {0};

    unsigned short bgcolor = BLACK;

    enum gamestate {
            GS_INITBOUNCE,
            GS_BOUNCE,

    };

    // Define a ball
    struct ball {
        int row;
        int col;
        int rd;
        int cd;
        unsigned short color;
    };

    struct state {
        enum gamestate gamestate;
        int size;
        struct ball balls[MAX_BALLS];
    };

    struct state cs, ps;

    struct ball *bp, *obp;

    struct ball ball;

    // Initialize ball
    cs.size = 5;
    bp = &cs.balls;
    bp->row = 0;
    bp->col = 0;
    bp->rd = 1;
    bp->cd = 1;
    bp->color = RED;

    // Run forever
    while (1) {
        // Keep track of the state we're goign to overwrite
        ps = cs;

        // Update button status
        for (int i = 0; i < NBUTTONS; i++) {
            buttonJustReleased[i] = KEY_DOWN(i, BUTTONS) == 0 && buttonWasDown[i];
            buttonWasDown[i] = KEY_DOWN(i, BUTTONS) != 0;
        }

    switch(cs.gamestate) {

    case GS_BOUNCE:
        // Take action on buttons
        if (buttonJustReleased[BUTTON_UP]) {
            cs.size++;
            if (cs.size > 150)
                cs.size = 150;
        }
        if (buttonWasDown[BUTTON_DOWN]) {
            cs.size--;
            if (cs.size < 3)
                cs.size = 3;
        }

        // See if we need to bounce off a wall
        bp = &cs.balls;
        bp->row += bp->rd;
        bp->col += bp->cd;
        if (bp->row < 0) {
            bp->row = 0;
            bp->rd = -bp->rd;
        }
        if (bp->row > SCREEN_HEIGHT - cs.size) {
            bp->row = SCREEN_HEIGHT - cs.size;
            bp->rd = -bp->rd;
        }
        if (bp->col < 0) {
            bp->col = 0;
            bp->cd = -bp->cd;
        }
        if (bp->col > SCREEN_WIDTH - cs.size) {
            bp->col = SCREEN_WIDTH - cs.size;
            bp->cd = -bp->cd;
        }
        break;

    case GS_INITBOUNCE:
        cs.size = 5;
        bp = &cs.balls;
        bp->row = 0;
        bp->col = 0;
        bp->rd = 1;
        bp->cd = 1;
        bp->color = RED;
        break;
    }

    // Update screen
    waitForVBlank();

    switch (cs.gamestate) { 
    case GS_INITBOUNCE:
        cs.gamestate = GS_BOUNCE;
        break;
    case GS_BOUNCE:
        //delay(1);

        // Erase the ball
        obp = &ps.balls;
        drawRect(obp->row, obp->col, ps.size, ps.size, bgcolor);
        // Draw the ball
        bp = &cs.balls;
        drawRect(bp->row, bp->col, cs.size, cs.size, bp->color);
    }

    //setPixel(SCREEN_HEIGHT / 2, SCREEN_WIDTH / 2, RED);
    //drawRect(30, 40, 5, 5, GREEN);
    }
}

void setPixel(int row, int col, unsigned short color) {
    videoBuffer[OFFSET(row, col, SCREEN_WIDTH)] = color;
}

void drawRect(int row, int col, int width, int height, unsigned short color) {
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            setPixel(r + row, c + col, color);
        }
    }
}

void delay(int n) {
    volatile int x = 0;
    for (int i = 0; i < n * 8000; i++) {
        x++;
    }
}

void waitForVBlank(void) {
    while (SCANLINECOUNTER > SCREEN_HEIGHT);
    while (SCANLINECOUNTER < SCREEN_HEIGHT);
    
}
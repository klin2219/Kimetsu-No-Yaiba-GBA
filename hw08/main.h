#ifndef MAIN_H
#define MAIN_H

#include "gba.h"
#define SLAYER_ROW 65
#define SLAYER_COL 105
#define DEMON_ROW 10
#define DEMON_COL 10

// TODO: Create any necessary structs

/*
* For example, for a Snake game, one could be:
*
* struct snake {
*   int heading;
*   int length;
*   int row;
*   int col;
* };
*
* Example of a struct to hold state machine data:
*
* struct state {
*   int currentState;
*   int nextState;
* };
*
*/

struct slayer {
    int row;
    int col;
    int rd;
    int cd;
};

struct demon {
    int row;
    int col;
};


/*
struct state {
    slayer currentState;
    slayer nextState;
};
*/

#endif

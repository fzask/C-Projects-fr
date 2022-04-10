#ifndef MAIN_H
#define MAIN_H
 
#include "gba.h"

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

//my Structs

typedef struct {
    int row;
    int col;
    int width;
    int height;
    int found;
} Player;

typedef struct {
    int row;
    int col;
    int rd;
    int cd;
    unsigned short color;
} Bubble;

typedef struct {
    Player player;
    int numLeft;
    Player items[6];
    int numIt;
    int nbubs;
    Bubble bubs[10];
} State;


//functions for me

int areColliding(Player player, Player item);
int areCollidingBall(Player player, Bubble bub);


#endif

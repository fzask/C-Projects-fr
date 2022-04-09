#include "main.h" 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "images/theGrotto_fr.h"
#include "images/grottoBackground.h"
#include "images/ariel.h"
#include "images/comb.h"
#include "images/dinglehopper.h"
#include "images/shell.h"
#include "images/sandDollar.h"
#include "images/win.h"
#include "images/lose.h"

#include "gba.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"


/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  START_WAIT,
  PLAY_ONE,
  PLAY,
  WIN,
  WIN_WAIT,
  LOSE,
  LOSE_WAIT
};



int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  Player player = {.row = 22, .col = 22, .width = ARIEL_WIDTH, .height = ARIEL_HEIGHT, .found = 1};
  State as = {.player = player};
  State bs;
  // Load initial application state
  enum gba_state state = START;


  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw

    switch (state) {
      case START:

        waitForVBlank();
        drawFullScreenImageDMA(theGrotto_fr);

        char title[] = "Ariel's Grotto ^_^"; 
        char credit[] = "by Fiza Shaikh - CS2110 Spring22";
        char startStr[] = "Oh no! Ariel lost all of her items!";
        char startStr2[] = "Make sure to dodge poison bubbles!";
        char startStr3[] = "Press START to save Ariel's things!";
        drawCenteredString(20, 40, 180, 30, title, GREEN);
        drawCenteredString(40, 40, 180, 30, credit, GREEN);
        drawCenteredString(80, 40, 160, 10, startStr, WHITE);
        drawCenteredString(90, 40, 160, 10, startStr2, WHITE);
        drawCenteredString(100, 40, 160, 10, startStr3, WHITE);

        state = START_WAIT;
        break;

      case START_WAIT:
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
          state = PLAY_ONE;
        }
        break;

      case PLAY_ONE:
        waitForVBlank();
        drawFullScreenImageDMA(grottoBackground);

        as.player.row = 22;
        as.player.col = 22;
        as.numLeft = 4;
        as.nbubs = 4;
        as.numIt = 4;

        //generate item locations
        for (int i = 0; i < as.numIt; i++) {
          as.items[i].row = randint(50, 159 - 20);
          as.items[i].col = randint(50, 239 - 20);
          as.items[i].width = 20;
          as.items[i].height = 20;
          as.items[i].found = 0;
        }

        //generate ball locations
        for (int i = 0; i < as.nbubs; i++) {
          as.bubs[i].row = randint(50, 157 - 10);
          as.bubs[i].col = randint(50, 239-10);
          as.bubs[i].rd = 0;
          as.bubs[i].cd = -1;
          as.bubs[i].color = YELLOW;
        }

        waitForVBlank();
        drawImageDMA(as.items[0].row, as.items[0].col, 20, 20, comb);
        drawImageDMA(as.items[1].row, as.items[1].col, 20, 20, shell);
        drawImageDMA(as.items[2].row, as.items[2].col, 20, 20, dinglehopper);
        drawImageDMA(as.items[3].row, as.items[3].col, 20, 20, sandDollar);

        state = PLAY;
        break;

      case PLAY:
        bs = as;

        //go back to start functionality
        if (KEY_JUST_PRESSED(BUTTON_SELECT, previousButtons, currentButtons)) {
          state = START;
        }

        //player up
        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          if (as.player.row <= 22) {
            as.player.row = 22;
          } else {
            as.player.row--;
          }
        }

        //player down
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          if (as.player.row >= 159) {
            as.player.row = 159;
          } else {
            as.player.row++;
          }
        }

        //player right
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          if (as.player.col >= 239 - ARIEL_WIDTH + 1) {
            as.player.col = 239 - ARIEL_WIDTH + 1;
          } else {
            as.player.col++;;
          }
        }

        //player left
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          if (as.player.col <= 0) {
            as.player.col = 0;
          } else {
            as.player.col--;
          }
        }


        //moving the bubble elements randomly to make things harder
        for (int i = 0; i < as.nbubs; i++) {
          as.bubs[i].row += as.bubs[i].rd;
          as.bubs[i].col += as.bubs[i].cd;

          if (as.bubs[i].row < 15) {
            as.bubs[i].row = 15;
            as.bubs[i].rd = -as.bubs[i].rd;
          }
          if (as.bubs[i].row > 159 - 10 + 1) {
            as.bubs[i].col = 159-10+1;
            as.bubs[i].cd = -as.bubs[i].rd;
          }

          if (as.bubs[i].col < 0) {
            as.bubs[i].col = 0;
            as.bubs[i].cd = -as.bubs[i].cd;
          }
          if (as.bubs[i].col > 239-10+1) {
            as.bubs[i].col = 239-10+1;
            as.bubs[i].cd = -as.bubs[i].cd;
          }
        }

        //checking to see if our player is hit with a ball
        for (int i = 0; i < as.nbubs; i++) {
          if (areCollidingBall(bs.player, as.bubs[i])) {
            state = LOSE;
            
          }
        }



        waitForVBlank();
      





        //check for collision w accessory
        for (int i = 0; i < as.numIt; i++) {
          if (!(as.items[i].found)) { 
            if (areColliding(as.player, as.items[i]) && (!areColliding(bs.player, bs.items[i]))) {
              as.numLeft--;
              as.items[i].found = 1;
              undrawImageDMA(as.items[i].row, as.items[i].col, 30, 30, grottoBackground);
              undrawImageDMA(0, 220, 10, 20, grottoBackground);
            }
          }
        }


        //score
        char itemsLeft[] = {as.numLeft + 48, 0};
        char score[16];
        strcpy(score, "Items left: ");
        strcat(score, (char *) itemsLeft);

        //erase prior bubs
        for (int i = 0; i< as.nbubs; i++) {
          undrawImageDMA(bs.bubs[i].row, bs.bubs[i].col, 10, 10, grottoBackground);
        }

        //new ones
        for (int i = 0; i < as.nbubs; i++) {
          drawRectDMA(as.bubs[i].row, as.bubs[i].col, 10, 10, as.bubs[i].color);
        }

        //redraw all the items which still havent been found yet

        // drawImageDMA(as.items[0].row, as.items[0].col, 20, 20, comb);
        // drawImageDMA(as.items[1].row, as.items[1].col, 20, 20, shell);
        // drawImageDMA(as.items[2].row, as.items[2].col, 20, 20, dinglehopper);
        // drawImageDMA(as.items[3].row, as.items[3].col, 20, 20, sandDollar);
        for (int i = 0; i < as.numIt; i++) {
          if (!(as.items[i].found)) {
            switch(i){
              case 0:
                drawImageDMA(as.items[i].row, as.items[i].col, 20, 20, comb);
                break;
              case 1:
                drawImageDMA(as.items[1].row, as.items[1].col, 20, 20, shell);
                break;
              case 2:
                drawImageDMA(as.items[2].row, as.items[2].col, 20, 20, dinglehopper);
                break;
              case 3:
                drawImageDMA(as.items[3].row, as.items[3].col, 20, 20, sandDollar);
                break;

              default:
                break;
            }
          }
        }


        //drawing/un for player movement
        undrawImageDMA(bs.player.row, bs.player.col, ARIEL_WIDTH, ARIEL_HEIGHT, grottoBackground);
        drawImageDMA(as.player.row, as.player.col, ARIEL_WIDTH, ARIEL_HEIGHT, ariel);

        //score's display
        drawString(10, 140, score, WHITE);

        //winning condition
        if (as.numLeft == (int) 0) {
          state = WIN;
        }
        break;



      case WIN:

        waitForVBlank();
        drawFullScreenImageDMA(win);
        char winStr[] = "Ariel found all of her things!";
        char winStr1[] = "Press SELECT to return to home screen:)";
        drawCenteredString(20, 20, 160, 10, winStr, MAGENTA);
        drawCenteredString(130, 40, 160, 10, winStr1, MAGENTA);
        state = WIN_WAIT;
        break;

      case WIN_WAIT:
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          state = START;
        }
        
        break;



      case LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(lose);
        char loser[] = "Ariel did not find her stuff..";
        char loser1[] = "Press SELECT to return to home screen:(";
        drawCenteredString(20, 20, 160, 10, loser, CYAN);
        drawCenteredString(130, 40, 160, 10, loser1, CYAN);

        state = LOSE_WAIT;


        // state = ?
        break;

      case LOSE_WAIT:
        if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
          state = START;
        }
        break;
    }

    previousButtons = currentButtons; // Store the current state of the buttons
  }

  //UNUSED(previousButtons); // You can remove this once previousButtons is used

  return 0;
}

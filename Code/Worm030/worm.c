// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#include "prep.h"
#include "worm.h"
#include "worm_model.h"
#include "board_model.h"


// ********************************************************************************************
// Forward declarations of functions
// ********************************************************************************************
// This avoids problems with the sequence of function declarations inside the code.
// Note: this kind of problem is solved by header files later on!

// Management of the game
void initializeColors();
void readUserInput(enum GameStates* agame_state );
enum ResCodes doLevel();

// ********************************************************************************************
// Functions
// ********************************************************************************************

// ************************************
// Management of the game
// ************************************

// Initialize colors of the game
void initializeColors() {
    // Define colors of the game
    start_color();
    init_pair(COLP_USER_WORM,COLOR_GREEN,COLOR_BLACK);
    init_pair(COLP_FREE_CELL,COLOR_BLACK,COLOR_BLACK);
}

void readUserInput(enum GameStates* agame_state ) {
    int ch; // For storing the key codes

    if ((ch = getch()) > 0) {
        // Is there some user input?
        // Blocking or non-blocking depends of config of getch
        switch(ch) {
            case 'q' :    // User wants to end the show
                *agame_state = WORM_GAME_QUIT;
                break;
            case KEY_UP :// User wants up
                setWormHeading(WORM_UP);
                break;
            case KEY_DOWN :// User wants down
                setWormHeading(WORM_DOWN);
                break;
            case KEY_LEFT :// User wants left
                setWormHeading(WORM_LEFT);
                break;
            case KEY_RIGHT :// User wants right
                setWormHeading(WORM_RIGHT);
                break;
            case 's' : // User wants single step
                nodelay(stdscr, FALSE); 
                // We simply make getch blocking
                break;
            case ' ' : // Terminate single step; make getch non-blocking again
                nodelay(stdscr, TRUE);
                // Make getch non-blocking again
                break;
        }
    }
    return;
}

enum ResCodes doLevel() {
    enum GameStates game_state; // The current game_state

    enum ResCodes res_codes; // Result code from functions
    bool end_level_loop;    // Indicates whether we should leave the main loop

    int bottomLeft_y, bottomLeft_x;   // Start positions of the worm

    // At the beginnung of the level, we still have a chance to win
    game_state = WORM_GAME_ONGOING;

    // There is always an initialized user worm.
    // Initialize the userworm with its size, position, heading.
    bottomLeft_y =  getLastRow();
    bottomLeft_x =  0;

    res_codes = initializeWorm(WORM_LENGTH, bottomLeft_y, bottomLeft_x , WORM_RIGHT, COLP_USER_WORM);
    if ( res_codes != RES_OK) {
        return res_codes;
    }

    // Show worm at its initial position
    showWorm();

    // Display all what we have set up until now
    refresh();

    // Start the loop for this level
    end_level_loop = false; // Flag for controlling the main loop
    while(!end_level_loop) {
        // Process optional user input
        readUserInput(&game_state); 
        if ( game_state == WORM_GAME_QUIT ) {
            end_level_loop = true;
            continue; // Go to beginning of the loop's block and check loop condition
        }
        
        // Process userworm
        // Clean the tail of the worm
        cleanWormTail();
        // Now move the worm for one step
        moveWorm(&game_state);
        
        // Bail out of the loop if something bad happened
        if ( game_state != WORM_GAME_ONGOING ) {
            end_level_loop = true;
            continue; // Go to beginning of the loop's block and check loop condition
        }
          // Show the worm at its new position
        showWorm();
        // END process userworm
        // Sleep a bit before we show the updated window
        napms(NAP_TIME);
        // Display all the updates
        refresh();
        // Start next iteration
    }

    // Preset res_code for rest of the function
    res_codes = RES_OK;

    // For some reason we left the control loop of the current level.
    // However, in this version we do not yet check for the reason.
    // There is no user feedback at the moment!

    // Normal exit point
    return res_codes;
}

// ********************************************************************************************

// ********************************************************************************************
// MAIN
// ********************************************************************************************

int main(void) {
    enum ResCodes res_codes;         // Result code from functions

    // Here we start
    initializeCursesApplication();  // Init various settings of our application
    initializeColors();             // Init colors used in the game

    // Maximal LINES and COLS are set by curses for the current window size.
    // Note: we do not cope with resizing in this simple examples!

    // Check if the window is large enough to display messages in the message area
    // a has space for at least one line for the worm
    if ( LINES < MIN_NUMBER_OF_ROWS || COLS < MIN_NUMBER_OF_COLS ) {
        // Since we not even have the space for displaying messages
        // we print a conventional error message via printf after
        // the call of cleanupCursesApp()
        cleanupCursesApp();
        printf("Das Fenster ist zu klein: wir brauchen mindestens %dx%d\n",
                MIN_NUMBER_OF_COLS, MIN_NUMBER_OF_ROWS );
        res_codes = RES_FAILED;
    } else {
        res_codes = doLevel();
        cleanupCursesApp();
    }
    return (int) res_codes;
}

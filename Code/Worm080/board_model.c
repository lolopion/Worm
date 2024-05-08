// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// The board model

#include <stdlib.h>
#include <curses.h>

#include "worm.h"
#include "board_model.h"
#include "messages.h"

// *************************************************
// Placing and removing items from the game board
// Check boundaries of game board
// *************************************************


// Place an item onto the curses display.
void placeItem(struct board* aboard, int y, int x, enum BoardCodes board_code, chtype symbol, enum ColorPairs color_pair) {
        //  Store item on the display (symbol code)
    move(y, x);                         // Move cursor to (y,x)
    attron(COLOR_PAIR(color_pair));     // Start writing in selected color
    addch(symbol);                      // Store symbol on the virtual display
    attroff(COLOR_PAIR(color_pair));   // Stop writing in selected color
    aboard->cells[y][x] = board_code;
}

// Getters
// Get the last usable row on the display
int getLastRowOnBoard(struct board* aboard) {
    return aboard->last_row;
}

// Get the last usable column on the display
int getLastColOnBoard(struct board* aboard) {
    return aboard->last_col;
}

int getNumberOfFoodItems(struct board* aboard) {
    return aboard->food_items;
}

enum BoardCodes getContentAt(struct board* aboard, struct pos position) { 
  if(position.y == -1 && position.x == -1){
    return BC_FREE_CELL;
  }
  return aboard->cells[position.y][position.x];
}

// Setters
void setNumberOfFoodItems(struct board* aboard, int n) {
  aboard->food_items = n;
}
void decrementNumberOfFoodItems(struct board* aboard) {
  aboard->food_items = aboard->food_items - 1;
}

enum ResCodes initializeBoard(struct board* aboard) {
 aboard->last_row = LINES - ROWS_RESERVED - 1;

 aboard->last_col = COLS - 1;
// Check dimensions of the board
 if ( COLS < MIN_NUMBER_OF_COLS || LINES < MIN_NUMBER_OF_ROWS + ROWS_RESERVED) {
 char buf[100];
 sprintf(buf,"Das Fenster ist zu klein: wir brauchen %dx%d",
 MIN_NUMBER_OF_COLS , MIN_NUMBER_OF_ROWS + ROWS_RESERVED );
 showDialog(buf,"Bitte eine Taste druecken");
 return RES_FAILED;
 }

 aboard->cells = malloc(LINES*sizeof(enum BoardCodes*));
      if (aboard->cells == NULL) {
       showDialog("Abbruch: Zu wenig Speicher","Bitte eine Taste druecken");
       exit (RES_FAILED);
      }

 for (int y = 0; y < LINES; y++) {
      // Allocate array of columns for each y
      aboard->cells[y] = malloc(COLS*sizeof(int)); // Hier Speicher allozieren
      
      if (aboard->cells[y] == NULL) {
       showDialog("Abbruch: Zu wenig Speicher","Bitte eine Taste druecken");
       exit (RES_FAILED);
       //Fehlermeldung wie oben
      }
 }
 return RES_OK;
}




enum ResCodes initializeLevel(struct board* aboard) {
    // define local variables for loops etc
    int i,y,x;
      // Fill board and screen buffer with empty cells.
    for (y = 0; y <= aboard->last_row - 1; y++) {
      for (x = 0; x <= aboard->last_col - 1; x++) {
          placeItem(aboard,y,x,BC_FREE_CELL,SYMBOL_FREE_CELL,COLP_FREE_CELL);
      }
    }  
// Draw a line in order to separate the message area
// Note: we cannot use funtion placeItem() since the message area
// is outside the board!
    y = aboard->last_row + 1;
    for (x = 0; x <= aboard->last_col; x++) {
        move (y,x);
        attron (COLOR_PAIR(COLP_BARRIER));
        addch (SYMBOL_BARRIER);
        attroff (COLOR_PAIR(COLP_BARRIER));
    }

// Barriers: use a loop
    for (i=0 ; i <= 10 ; i++ ) {
    y=3 + i;
    x=(MIN_NUMBER_OF_COLS);
    placeItem(aboard,y,x,BC_BARRIER,SYMBOL_BARRIER,COLP_BARRIER);
    }
    for (i=0 ; i <=12 ; i++) {
    y=9 + i;
    x=(MIN_NUMBER_OF_COLS)/4;
    placeItem(aboard,y,x,BC_BARRIER,SYMBOL_BARRIER,COLP_BARRIER);
    }
// Food
    placeItem(aboard, 4, 2,BC_FOOD_1,SYMBOL_FOOD_1,COLP_FOOD_1);
    placeItem(aboard, 15, 27,BC_FOOD_1,SYMBOL_FOOD_1,COLP_FOOD_1); //Catgo1

    placeItem(aboard, 11,25,BC_FOOD_2,SYMBOL_FOOD_2,COLP_FOOD_2);
    placeItem(aboard, 11,62,BC_FOOD_2,SYMBOL_FOOD_2,COLP_FOOD_2);
    placeItem(aboard, 12,12,BC_FOOD_2,SYMBOL_FOOD_2,COLP_FOOD_2);
    placeItem(aboard, 14,(MIN_NUMBER_OF_COLS)/2,BC_FOOD_2,SYMBOL_FOOD_2,COLP_FOOD_2); //Catgo2

    placeItem(aboard, 20,40,BC_FOOD_3,SYMBOL_FOOD_3,COLP_FOOD_3);
    placeItem(aboard, 14,8,BC_FOOD_3,SYMBOL_FOOD_3,COLP_FOOD_3);
    placeItem(aboard, 4,(MIN_NUMBER_OF_COLS)/3,BC_FOOD_3,SYMBOL_FOOD_3,COLP_FOOD_3);
    placeItem(aboard, 9,67,BC_FOOD_3,SYMBOL_FOOD_3,COLP_FOOD_3); //Catgo3

// Initialize number of food items
// Attention: must match number of items placed on the board above
    aboard->food_items = 10;
    return RES_OK;
    }    

void cleanupBoard(struct board* aboard) {
  for(int y=0; y<LINES; y++){
  free(aboard->cells[y]);
  }
  free(aboard->cells);
}


  

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
#include <string.h>

// ********************************************************************************************
// Constants, data structures
// ********************************************************************************************

// Result codes of functions
#define RES_OK 0
#define RES_FAILED 1

// *********************************************
// Standard curses initialization and cleanup
// *********************************************

// Initialize application with respect to curses settings
void initializeCursesApplication() {
    initscr(); // Initialize the curses screen

    // Note:
    // The call to initscr() defines various global variables of the curses framework.
    // stdscr, LINES, COLS, TRUE, FALSE

    noecho();  // Characters typed are not echoed
    cbreak();  // No buffering of stdin
    nonl();    // Do not translate 'return key' on keyboard to newline character
    keypad(stdscr, TRUE); // Enable the keypad
    curs_set(0);          // Make cursor invisible
    // Begin in non-single-step mode (getch will not block)
    nodelay(stdscr, TRUE);  // make getch to be a non-blocking call
}

// Reset display to normale state and terminate curses application
void cleanupCursesApp(void)
{
    standend();   // Turn off all attributes
    refresh();    // Write changes to terminal
    curs_set(1);  // Set cursor state to normal visibility
    endwin();     // Terminate curses application
}

// ********************************************************************************************
// MAIN
// ********************************************************************************************

int main(void)
{
    int res_code;                                                                                  // Result code from functions
    char* message_template = "Das Fenster hat xxx Zeilen und yyy Spalten";
    int msg_len;
    int min_rows = 3;

 initializeCursesApplication();                                                                    // Init various settings of our application
   
 msg_len=strlen(message_template);                                                                 // Compute length of our template
                                                                                                   // Maximal LINES and COLS are set by curses for the current window size.
                                                                                                   // Check if the window is large enough to display our message 
   
  if ( LINES < min_rows || COLS < msg_len )
  {
     cleanupCursesApp();                                                                           // Cleanup special curses settings and restore the normal terminal functionality
     printf("Das Fenster ist zu klein: wir brauchen mindestens %03dx%03d \n", msg_len, min_rows ); // Print a conventional error message via printf.
     res_code = 1;                                                                                 // Set the result code to report the error
  }    

    else
    {
        int mid_row;                                                                               // Center output
        mid_row = LINES/2;
        int start_col; 
        start_col = (COLS-msg_len)/2;
          
        char topLeft= 'A';
        char topRight= 'B';
        char botRight= 'C';
        char botLeft= 'D';
                                                                                                   // Write letter A to the top left  corner of our display
        move(0,0);                                                                                 // Move to position
        addch(topLeft);                                                                            // Put character there
        
        mvaddch(0,COLS-1,topRight);                                                                // Write letter B to the top right corner of our display
        mvaddch(LINES-1,COLS-1,botRight);                                                          // Write letter C to the bottom right corner of our display
        mvaddch(LINES-1,0,botLeft);                                                                // Write letter D to the bottom left  corner of our display
        
        mvprintw(mid_row, start_col,"Das Fenster hat %03d Zeilen und %03d Spalten", LINES, COLS);  // Write our message centered onto the display

        refresh();                                                                                 // Refresh the screen in order to show all changes on the screen
        
        nodelay(stdscr, FALSE);                                                                    // make getch to be a blocking call 
        getch();                                                                                   // Wait for user to press a key

        res_code  = 0;                                                                             // Set the result code to report success

        cleanupCursesApp();                                                                        // Cleanup special curses settings and restore the normal terminal functionality 
    }
    return res_code;
}

// A simple variant of the game Snake
//
// Used for teaching in classes
//
// Author:
// Franz Regensburger
// Ingolstadt University of Applied Sciences
// (C) 2011
//
// Command line options

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "messages.h"
#include "worm.h"
#include "options.h"

void usage() {
    char buf[100];
    sprintf(buf,"Aufruf: worm [-h] [-n ms] [-s] [ Dateiname ] ");
    showDialog(buf,"Bitte eine Taste druecken");
}

// Read command line options.
// See manual page getopt(3) for documentation
//
enum ResCodes readCommandLineOptions(struct game_options* somegops,
        int argc, char* argv[]) {
    signed char c;

    // Initialize;
    somegops -> nap_time = NAP_TIME;
    somegops -> start_single_step = 0;
    somegops -> start_level_filename = NULL;

    while((c = getopt(argc, argv, "n:s")) != -1)
        switch(c) {
            case('h'):
                usage();
                return RES_WRONG_OPTION;
            case('n'):
                somegops -> nap_time = atoi(optarg);
                continue;
            case('s'):
                somegops -> start_single_step = true;
                continue;
            default:
                usage();
                return RES_WRONG_OPTION;
                }

    // Skip all options processed
    argc -= optind;
    argv += optind;

    // Are there any non-option arguments left?
    // In our case no further arguments are allowed
    if (argc > 0 ) {
        usage();
        return RES_WRONG_OPTION;
    }
    if (argc == 1){
       somegops-> start_level_filename = strdup(argv[0]);
    }
       return RES_OK; 
}


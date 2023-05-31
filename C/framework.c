#include "scenario.h"
#include <ctype.h>  // isgraph
#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h> // strchr

void quit(void) {
    exit(0);
}

void unexpected_input(int command) {
    printf("Unexpected input %d (0x%.2X) ('%c')\n",
           command, command, isgraph(command) ? command : '.');
}

int input_command(char *allowed) {
    int command = 0;
    while ((command = getchar()) != EOF) {
        if (strchr(allowed, command) != NULL) {
            break;
        }
        switch (command) {
            case 10:
                break;
            case 'q':
                printf("Quit\n");
                quit();
                break;
            default:
                unexpected_input(command);
                break;
        }
    }
    return command;
}

extern void first_scenario(void);

Scenario Scenario_Play_again = {
    .description = "Play again? y/n",
    .number_of_choices = 2,
    .choices = {
        {
            .key_to_press = 'y',
            .action = "play again",
            .next_method = first_scenario,
        },
        {
            .key_to_press = 'n',
            .action = "Quit\nThankyou for playing",
            .next_method = quit,
        },
    }};

void play_again(void) {
    scenario(&Scenario_Play_again);
}

void game_won(void) {
    printf("Congratulations! You have won!!\n\n");
    play_again();
}

void game_lost(void) {
    play_again();
}

void start_game(void) {
    printf("Press q to quit at any time.\n\n");
    first_scenario();
}

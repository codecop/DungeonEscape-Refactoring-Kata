#include "scenario.h"
#include <assert.h>
#include <ctype.h>  // isgraph
#include <stdio.h>
#include <stdlib.h> // exit
#include <string.h> // strchr

void _game_quit(void) {
    exit(0);
}

void _game_unexpected_input(int command) {
    printf("Unexpected input %d (0x%.2X) ('%c')\n",
           command, command, isgraph(command) ? command : '.');
}

int _game_input_command(char *allowed) {
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
                _game_quit();
                break;
            default:
                _game_unexpected_input(command);
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
            .next_method = _game_quit,
        },
    }};

void game_execute_scenario(const Scenario *const scenario) {
    scenario_introduce(scenario);

    char *choice_keys = scenario_list_choice_keys(scenario);
    int command = _game_input_command(choice_keys);
    assert(strchr(choice_keys, command) != NULL);

    scenario_execute_choice(scenario, command);
}

void game_start(void) {
    printf("Press q to quit at any time.\n\n");
    first_scenario();
}

void _game_play_again(void) {
    game_execute_scenario(&Scenario_Play_again);
}

void game_won(void) {
    printf("Congratulations! You have won!!\n\n");
    _game_play_again();
}

void game_lost(void) {
    _game_play_again();
}

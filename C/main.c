#include "scenario.h"
#include <ctype.h>
#include <stdbool.h> // bool type
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   // strchr
#include <sys/stat.h> // stat
#include <windows.h>

bool HAVE_BEEN_HIDING = false;

void room_in_a_cell();

void room_right_corridor();

void room_guardroom();

bool file_exists(char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void write_location(char *location) {
    FILE *fp = fopen("location.txt", "a");
    fprintf(fp, "%s\n", location);
    fclose(fp);
}

void delete_location() {
    remove("location.txt");
}

void quit(char *message) {
    printf("Quit\n");
    printf(message);
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
                quit("");
                break;
            default:
                unexpected_input(command);
                break;
        }
    }
    return command;
}

void play_again() {
    printf("Play again? y/n\n");
    int command = input_command("yn");
    switch (command) {
        case 'y':
            printf("play again\n");
            room_in_a_cell();
            break;
        case 'n':
            quit("Thankyou for playing");
            break;
    }
}

void game_won(char *scenario) {
    printf("%s\nCongratulations! You have won!!\n\n", scenario);
    play_again();
}

void search() {
    char *scenario = "You are searching the guardroom";
    printf("%s.\n", scenario);
    if (file_exists("location.txt")) {
        delete_location();
        game_won("You have found the escaped prisonor! Take them back to their cell.");
    } else {
        scenario = "There is no-one here";
        printf("%s\n", scenario);
        room_guardroom();
    }
}

void hiding_loop() {
    HAVE_BEEN_HIDING = true;
    int counter = 0;
    char *scenario;
    while (file_exists("location.txt") && counter < 2) {
        scenario = "The screams are quieter.";
        printf("%s\n", scenario);
        Sleep(500);
        counter++;
    }
}

void game_lost(char *scenario) {
    printf("%s\n", scenario);
    play_again();
}

void hide();

Scenario Scenario_In_hiding = {
    .description = "You are hiding in the guardroom",
    .number_of_choices = 2,
    .choices = {
        {
            .description = "go back (o)ut of the guardroom",
            .action = "out of the guardroom",
            .next_scenario = room_right_corridor,
        },
        {
            .description = "Continue (h)iding",
            .action = "hide",
            .next_scenario = hide,
        },
    }};

void hide() {
    char *description = "You are hiding in the guardroom";
    printf("%s.\n", description);
    write_location("guardroom");
    hiding_loop();

    if (file_exists("location.txt")) {
        scenario(&Scenario_In_hiding);
    } else {
        game_lost("You realize the room is not empty any more. A guard appears and captures you. Soon you find yourself back in your cell, feeling miserable that your escape attempt failed.");
    }
}

Scenario Scenario_Guardroom = {
    .description = "You are standing in an empty guard room that looks to have been hastily abandoned. There is no-one here but the screams grow louder. Suddenly the light from the torch outside goes out",
    .number_of_choices = 3,
    .choices = {
        {
            .description = "go back (o)ut of the guardroom",
            .action = "out of the guardroom",
            .next_scenario = room_right_corridor,
        },
        {
            .description = "(h)ide in the guardroom",
            .action = "hide",
            .next_scenario = hide,
        },
        {
            .description = "(s)earch the guardroom",
            .action = "search",
            .next_scenario = search,
        },
    }};

void room_guardroom() {
    scenario(&Scenario_Guardroom);
}

void room_upstairs() {
    game_won("The stairs lead to the dungeon exit. Your friend Freija the Magnificent Warrior runs towards you and embraces you. 'I am here to rescue you!' she says.");
}

Scenario Scenario_Left_corridor = {
    .description = "You are standing at the bottom of some stairs in the dungeon. There is a torch burning on your left and a closed door on your right. You hear screams behind you",
    .number_of_choices = 3,
    .choices = {
        {
            .description = "go (u)p the stairs",
            .action = "up the stairs",
            .next_scenario = room_upstairs,
        },
        {
            .description = "go (b)ack",
            .next_scenario = room_right_corridor,
        },
        {
            .description = "go (t)hrough the door",
            .next_scenario = room_guardroom,
        },
    }};

void room_left_corridor() {
    scenario(&Scenario_Left_corridor);
}

void fight_man() {
    game_lost("Unfortunately you lose the fight and fall to the floor dead.");
}

Scenario Scenario_Right_corridor = {
    .description = "You are standing in a corridor in the dungeon. It is very dark. A man runs towards you screaming, carrying a big sword",
    .number_of_choices = 2,
    .choices = {
        {
            .description = "(r)un the other way",
            .action = "run",
            .next_scenario = room_left_corridor,
        },
        {
            .description = "(f)ight him",
            .action = "fight",
            .next_scenario = fight_man,
        },
    }};

void room_right_corridor() {
    if (HAVE_BEEN_HIDING) {
        delete_location();
    }

    scenario(&Scenario_Right_corridor);
}

Scenario Scenario_Corridor_outside_cell = {
    .description = "You are standing in a corridor in the dungeon. To the left you can see a torch burning and some steps. To the right it is dark and you hear screams",
    .number_of_choices = 2,
    .choices = {
        {
            .description = "go (l)eft",
            .next_scenario = room_left_corridor,
        },
        {
            .description = "go (r)ight",
            .next_scenario = room_right_corridor,
        },
    }};

void room_corridor_outside_cell() {
    scenario(&Scenario_Corridor_outside_cell);
}

Scenario Scenario_In_a_cell = {
    .description = "You are standing in a cell in the dungeon. The recent earthquake has broken the door",
    .number_of_choices = 2,
    .choices = {
        {
            .description = "(s)tay in the cell",
            .next_scenario = room_in_a_cell,
        },
        {
            .description = "go through the (d)oor",
            .next_scenario = room_corridor_outside_cell,
        },
    }};

void room_in_a_cell() {
    scenario(&Scenario_In_a_cell);
}

int main() {
    printf("Press q to quit at any time.\n\n");

    room_in_a_cell();

    return 0;
}

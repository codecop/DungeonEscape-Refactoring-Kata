#include "framework.h"
#include <stdbool.h>  // bool type
#include <stdio.h>
#include <sys/stat.h> // stat
#include <windows.h>  // sleep

bool HAVE_BEEN_HIDING = false;

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

// game scenarios

void found_prisoner(void) {
    printf("You have found the escaped prisonor! Take them back to their cell.\n");
    game_won();
}

void room_guardroom();

void search_guardroom(void) {
    // this is a special scenario fork: a decision is made which scenario to play
    printf("You are searching the guardroom.\n");

    if (file_exists("location.txt")) {
        delete_location();
        found_prisoner();

    } else {
        printf("There is no-one here\n");
        room_guardroom();
    }
}

void room_right_corridor();

void hide(void);

void in_hiding(void) {
    Scenario scenario = {
        .description = "You are hiding in the guardroom",
        .number_of_choices = 2,
        .choices = {
            {
                .description = "go back (o)ut of the guardroom",
                .action = "out of the guardroom",
                .next_method = room_right_corridor,
            },
            {
                .description = "Continue (h)iding",
                .action = "hide",
                .next_method = hide,
            },
        }};

    game_execute_scenario(&scenario);
}

void been_captured(void) {
    printf("You realize the room is not empty any more. A guard appears and captures you. Soon you find yourself back in your cell, feeling miserable that your escape attempt failed.\n");
    game_lost();
}

void hide(void) {
    // this is a special scenario fork: a decision is made which scenario to play

    printf("You are hiding in the guardroom.\n");

    // hiding loop
    write_location("guardroom");
    HAVE_BEEN_HIDING = true;
    int counter = 0;
    while (file_exists("location.txt") && counter < 2) {
        printf("The screams are quieter.\n");
        Sleep(500);
        counter++;
    }

    if (file_exists("location.txt")) {
        in_hiding();

    } else {
        been_captured();
    }
}

void room_guardroom(void) {
    Scenario scenario = {
        .description = "You are standing in an empty guard room that looks to have been hastily abandoned. There is no-one here but the screams grow louder. Suddenly the light from the torch outside goes out",
        .number_of_choices = 3,
        .choices = {
            {
                .description = "go back (o)ut of the guardroom",
                .action = "out of the guardroom",
                .next_method = room_right_corridor,
            },
            {
                .description = "(h)ide in the guardroom",
                .action = "hide",
                .next_method = hide,
            },
            {
                .description = "(s)earch the guardroom",
                .action = "search",
                .next_method = search_guardroom,
            },
        }};

    game_execute_scenario(&scenario);
}

void room_upstairs(void) {
    printf("The stairs lead to the dungeon exit. Your friend Freija the Magnificent Warrior runs towards you and embraces you. 'I am here to rescue you!' she says.\n");
    game_won();
}

void room_left_corridor(void) {
    Scenario scenario = {
        .description = "You are standing at the bottom of some stairs in the dungeon. There is a torch burning on your left and a closed door on your right. You hear screams behind you",
        .number_of_choices = 3,
        .choices = {
            {
                .description = "go (u)p the stairs",
                .action = "up the stairs",
                .next_method = room_upstairs,
            },
            {
                .description = "go (b)ack",
                .next_method = room_right_corridor,
            },
            {
                .description = "go (t)hrough the door",
                .next_method = room_guardroom,
            },
        }};

    game_execute_scenario(&scenario);
}

void fight_man(void) {
    printf("Unfortunately you lose the fight and fall to the floor dead.\n");
    game_lost();
}

void room_right_corridor(void) {
    Scenario scenario = {
        .description = "You are standing in a corridor in the dungeon. It is very dark. A man runs towards you screaming, carrying a big sword",
        .number_of_choices = 2,
        .choices = {
            {
                .description = "(r)un the other way",
                .action = "run",
                .next_method = room_left_corridor,
            },
            {
                .description = "(f)ight him",
                .action = "fight",
                .next_method = fight_man,
            },
        }};

    // extra logic
    if (HAVE_BEEN_HIDING) {
        delete_location();
    }

    game_execute_scenario(&scenario);
}

void room_corridor_outside_cell(void) {
    Scenario scenario = {
        .description = "You are standing in a corridor in the dungeon. To the left you can see a torch burning and some steps. To the right it is dark and you hear screams",
        .number_of_choices = 2,
        .choices = {
            {
                .description = "go (l)eft",
                .next_method = room_left_corridor,
            },
            {
                .description = "go (r)ight",
                .next_method = room_right_corridor,
            },
        }};

    game_execute_scenario(&scenario);
}

void room_in_a_cell(void) {
    Scenario scenario = {
        .description = "You are standing in a cell in the dungeon. The recent earthquake has broken the door",
        .number_of_choices = 2,
        .choices = {
            {
                .description = "(s)tay in the cell",
                .next_method = room_in_a_cell,
            },
            {
                .description = "go through the (d)oor",
                .next_method = room_corridor_outside_cell,
            },
        }};

    game_execute_scenario(&scenario);
}

int main() {
    game_start(room_in_a_cell);

    return 0;
}

#include "framework.h"
#include "location.h"
#include <stdbool.h> // bool type
#include <stdio.h>
#include <windows.h> // sleep

void search_guardroom_found_prisoner(void) {
    // notify other instance of being found
    delete_location();

    printf("You have found the escaped prisonor! Take them back to their cell.\n");
    game_won();
}

void room_guardroom(void);

void search_guardroom(void) {
    ScenarioFork scenario = {
        .description = "You are searching the guardroom",
        .is_first_choice = exists_location,
        .choices = {
            {
                .next_method = search_guardroom_found_prisoner,
            },
            {
                .action = "There is no-one here",
                .next_method = room_guardroom,
            },
        }};

    scenario_execute_fork(&scenario);
}

void room_right_corridor(void);

void hiding_leaving_guardroom(void) {
    // extra logic
    delete_location();

    room_right_corridor();
}

void hiding_in_guardroom(void);

void in_hiding(void) {
    Scenario scenario = {
        .description = "You are hiding in the guardroom",
        .number_of_choices = 2,
        .choices = {
            {
                .description = "go back (o)ut of the guardroom",
                .action = "out of the guardroom",
                .next_method = hiding_leaving_guardroom,
            },
            {
                .description = "Continue (h)iding",
                .action = "hide",
                .next_method = hiding_in_guardroom,
            },
        }};

    game_user_choice_of_scenario(&scenario);
}

void been_captured(void) {
    // location has been deleted by other instance
    printf("You realize the room is not empty any more. A guard appears and captures you. Soon you find yourself back in your cell, feeling miserable that your escape attempt failed.\n");
    game_lost();
}

void hiding_in_guardroom(void) {
    printf("You are hiding in the guardroom.\n");
    write_location("guardroom");

    // hiding loop
    int counter = 0;
    while (exists_location() && counter < 2) {
        printf("The screams are quieter.\n");
        Sleep(500);
        counter++;
    }

    if (exists_location()) {
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
                .next_method = hiding_in_guardroom,
            },
            {
                .description = "(s)earch the guardroom",
                .action = "search",
                .next_method = search_guardroom,
            },
        }};

    game_user_choice_of_scenario(&scenario);
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

    game_user_choice_of_scenario(&scenario);
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

    game_user_choice_of_scenario(&scenario);
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

    game_user_choice_of_scenario(&scenario);
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

    game_user_choice_of_scenario(&scenario);
}

int main() {
    game_start(room_in_a_cell);

    return 0;
}

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
    char *scenario = "You are searching the guardroom.";
    printf("%s\n", scenario);
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

void hide() {
    char *scenario = "You are hiding in the guardroom.";
    printf("%s\n", scenario);
    write_location("guardroom");
    hiding_loop();
    if (file_exists("location.txt")) {
        scenario = "You are hiding in the guardroom";
        char *choices = "\n\t go back (o)ut of the guardroom\n\t Continue (h)iding";
        printf("%s. Would you like to:%s\n\n", scenario, choices);

        int command = input_command("oh");

        switch (command) {
            case 'o':
                printf("out of the guardroom\n");
                if (HAVE_BEEN_HIDING) {
                    delete_location();
                }
                room_right_corridor();
                break;
            case 'h':
                printf("hide\n");
                hide();
                break;
        }

    } else {
        game_lost("You realize the room is not empty any more. A guard appears and captures you. Soon you find yourself back in your cell, feeling miserable that your escape attempt failed.");
    }
}

void room_guardroom() {
    char *scenario = "You are standing in an empty guard room that looks to have been hastily abandoned. There is no-one here but the screams grow louder. Suddenly the light from the torch outside goes out";
    char *choices = "\n\t go back (o)ut of the guardroom\n\t (h)ide in the guardroom\n\t (s)earch the guardroom";
    printf("%s. Would you like to:%s\n\n", scenario, choices);

    int command;
    while (true) {
        command = input_command("ohs");
        if (command == 'o') {
            printf("out of the guardroom\n");
            if (HAVE_BEEN_HIDING) {
                delete_location();
            }
            room_right_corridor();
            break;
        }
        switch (command) {
            case 'h':
                printf("hide\n");
                hide();
                break;
            case 's':
                printf("search\n");
                search();
                break;
        }
    }
}

void room_upstairs() {
    game_won("The stairs lead to the dungeon exit. Your friend Freija the Magnificent Warrior runs towards you and embraces you. 'I am here to rescue you!' she says.");
}

void room_left_corridor() {
    char *scenario = "You are standing at the bottom of some stairs in the dungeon. There is a torch burning on your left and a closed door on your right. You hear screams behind you";
    char *choices = "\n\t go (u)p the stairs\n\t go (b)ack\n\t go (t)hrough the door";
    printf("%s. Would you like to:%s\n\n", scenario, choices);

    int command = input_command("ubt");

    switch (command) {
        case 'u':
            printf("up the stairs\n");
            room_upstairs();
            break;
        case 'b':
            printf("go back\n");
            room_right_corridor();
            break;
        case 't':
            printf("go through the door\n");
            room_guardroom();
            break;
    }
}

void room_right_corridor() {
    char *scenario = "You are standing in a corridor in the dungeon. It is very dark. A man runs towards you screaming, carrying a big sword";
    char *choices = "\n\t (r)un the other way\n\t (f)ight him";
    printf("%s. Would you like to:%s\n\n", scenario, choices);

    int command = input_command("rf");

    switch (command) {
        case 'r':
            printf("run\n");
            room_left_corridor();
            break;
        case 'f':
            printf("fight\n");
            game_lost("Unfortunately you lose the fight and fall to the floor dead.");
            break;
    }
}

void room_corridor_outside_cell() {
    char *scenario = "You are standing in a corridor in the dungeon. To the left you can see a torch burning and some steps. To the right it is dark and you hear screams";
    char *choices = "\n\t go (l)eft\n\t go (r)ight";
    printf("%s. Would you like to:%s\n\n", scenario, choices);

    int command = input_command("lr");

    switch (command) {
        case 'l':
            printf("go left\n");
            room_left_corridor();
            break;
        case 'r':
            printf("go right\n");
            room_right_corridor();
            break;
    }
}

#define MAX_SCENARIO_DESCRIPTION 100
#define MAX_CHOICES 10
#define MAX_CHOICE_DESCRIPTION 40

typedef struct Choice {
    char description[MAX_CHOICE_DESCRIPTION + 1];
    char key_to_press;
    char *action;
    void (*next_scenario)(void);
} Choice;

typedef struct Scenario {
    char description[MAX_SCENARIO_DESCRIPTION];
    int number_of_choices;
    Choice choices[MAX_CHOICES];
} Scenario;

char *scenario_describe(Scenario *scenario) {
    return scenario->description;
}

char choices_description_buffer[MAX_CHOICES * MAX_CHOICE_DESCRIPTION + 1];

char *scenario_describe_choices(Scenario *scenario) {
    choices_description_buffer[0] = 0;
    for (int i = 0; i < scenario->number_of_choices; i++) {
        strcat(choices_description_buffer, "\n\t ");
        strcat(choices_description_buffer, scenario->choices[i].description);
    }
    return choices_description_buffer;
}

char choices_key_buffer[MAX_CHOICES * 1 + 1];

char *scenario_list_choice_keys(Scenario *scenario) {
    choices_key_buffer[0] = 0;
    for (int i = 0; i < scenario->number_of_choices; i++) {
        choices_key_buffer[i] = scenario->choices[i].key_to_press;
        choices_key_buffer[i + 1] = 0;
    }
    return choices_key_buffer;
}

void scenario_execute_choice(Scenario *scenario, int command) {
    for (int i = 0; i < scenario->number_of_choices; i++) {
        if (command == scenario->choices[i].key_to_press) {
            printf("%s\n", scenario->choices[i].action);
            scenario->choices[i].next_scenario();
            break;
        }
    }
}

void scenario(Scenario *scenario) {
    char *description = scenario_describe(scenario);
    char *choices = scenario_describe_choices(scenario);
    printf("%s. Would you like to:%s\n\n", description, choices);
    int command = input_command(scenario_list_choice_keys(scenario));
    scenario_execute_choice(scenario, command);
}

Scenario Scenario_in_a_cell = {
    .description = "You are standing in a cell in the dungeon. The recent earthquake has broken the door",
    .number_of_choices = 2,
    .choices = {
        {
            .description = "(s)tay in the cell",
            .key_to_press = 's',
            .action = "stay in the cell",
            .next_scenario = room_in_a_cell,
        },
        {
            .description = "go through the (d)oor",
            .key_to_press = 'd',
            .action = "go through the door",
            .next_scenario = room_corridor_outside_cell,
        },
    }};

void room_in_a_cell() {
    scenario(&Scenario_in_a_cell);
}

int main() {
    printf("Press q to quit at any time.\n\n");

    room_in_a_cell();

    return 0;
}

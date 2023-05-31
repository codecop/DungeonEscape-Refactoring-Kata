#include "scenario.h"
#include <stdio.h>
#include <string.h> // strcat

int input_command(char *allowed);

char choices_description_buffer[MAX_CHOICES * MAX_CHOICE_DESCRIPTION + 1];
char choices_key_buffer[MAX_CHOICES * 1 + 1];

char *scenario_describe(Scenario *scenario) {
    return scenario->description;
}

char *scenario_describe_choices(Scenario *scenario) {
    choices_description_buffer[0] = 0;
    for (int i = 0; i < scenario->number_of_choices; i++) {
        strcat(choices_description_buffer, "\n\t ");
        strcat(choices_description_buffer, scenario->choices[i].description);
    }
    return choices_description_buffer;
}

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

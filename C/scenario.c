#include "scenario.h"
#include <stdio.h>
#include <string.h> // strcat

int input_command(char *allowed);

char choices_description_buffer[MAX_CHOICES * MAX_CHOICE_DESCRIPTION + 1];
char choices_key_buffer[MAX_CHOICES * 1 + 1];
char choice_action_buffer[MAX_CHOICE_DESCRIPTION + 1];

const char *scenario_describe(const Scenario *const scenario) {
    return scenario->description;
}

const char *scenario_describe_choices(const Scenario *const scenario) {
    choices_description_buffer[0] = 0;
    for (int i = 0; i < scenario->number_of_choices; i++) {
        strcat(choices_description_buffer, "\n\t ");
        strcat(choices_description_buffer, scenario->choices[i].description);
    }
    return choices_description_buffer;
}

char *scenario_list_choice_keys(const Scenario *const scenario) {
    choices_key_buffer[0] = 0;
    for (int i = 0; i < scenario->number_of_choices; i++) {
        choices_key_buffer[i] = scenario->choices[i].key_to_press;
        choices_key_buffer[i + 1] = 0;
    }
    return choices_key_buffer;
}

char *choice_action_from_description(const char *description) {
    size_t sourceLength = strlen(description);
    size_t destIndex = 0;

    for (size_t i = 0; i < sourceLength; i++) {
        if (description[i] != '(' && description[i] != ')') {
            choice_action_buffer[destIndex] = description[i];
            destIndex++;
        }
    }

    choice_action_buffer[destIndex] = '\0';
    return choice_action_buffer;
}

void choice_execute(const Choice *const choice) {
    char *action = choice->action;
    if (action == 0) {
        action = choice_action_from_description(choice->description);
    }
    printf("%s\n", action);
    choice->next_scenario();
}

void scenario_execute_choice(const Scenario *const scenario, int command) {
    for (int i = 0; i < scenario->number_of_choices; i++) {
        const Choice *const choice = &scenario->choices[i];
        if (command == choice->key_to_press) {
            choice_execute(choice);
            break;
        }
    }
}

void scenario(const Scenario *const scenario) {
    const char *description = scenario_describe(scenario);
    const char *choices = scenario_describe_choices(scenario);
    printf("%s. Would you like to:%s\n\n", description, choices);
    int command = input_command(scenario_list_choice_keys(scenario));
    scenario_execute_choice(scenario, command);
}

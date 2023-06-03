#include "scenario.h"
#include <assert.h>
#include <stdio.h>
#include <string.h> // strcat

extern int input_command(char *allowed);

char choices_description_buffer[SCENARIO_MAX_CHOICES * CHOICE_MAX_DESCRIPTION + 1];
char choices_key_buffer[SCENARIO_MAX_CHOICES * 1 + 1];

int scenario_string_not_empty(const char *string) {
    return string && string[0] != '\0';
}

const char *scenario_describe(const Scenario *const scenario) {
    return scenario->description;
}

const char *scenario_describe_choices(const Scenario *const scenario) {
    choices_description_buffer[0] = '\0';
    for (int i = 0; i < scenario->number_of_choices; i++) {
        const Choice *const choice = &scenario->choices[i];
        choice_describe(choice, choices_description_buffer);
    }
    return choices_description_buffer;
}

void scenario_introduce(const Scenario *const scenario) {
    const char *description = scenario_describe(scenario);
    const char *choices = scenario_describe_choices(scenario);
    if (scenario_string_not_empty(choices)) {
        printf("%s. Would you like to:%s\n\n", description, choices);
    } else {
        printf("%s\n", description);
    }
}

char *scenario_list_choice_keys(const Scenario *const scenario) {
    choices_key_buffer[0] = '\0';
    for (int i = 0; i < scenario->number_of_choices; i++) {
        const Choice *const choice = &scenario->choices[i];
        choices_key_buffer[i] = choice_get_key(choice);
        choices_key_buffer[i + 1] = '\0';
    }
    return choices_key_buffer;
}

void scenario_execute_choice(const Scenario *const scenario, int choice_key) {
    for (int i = 0; i < scenario->number_of_choices; i++) {
        const Choice *const choice = &scenario->choices[i];
        if (choice_key == choice_get_key(choice)) {
            choice_execute(choice);
            if (choice->next_scenario) {
                scenario_execute(choice->next_scenario);
            }
            break;
        }
    }
}

void scenario_execute(const Scenario *const scenario) {
    scenario_introduce(scenario);

    char *choice_keys = scenario_list_choice_keys(scenario);
    int command = input_command(choice_keys);
    assert(strchr(choice_keys, command) != NULL);

    scenario_execute_choice(scenario, command);
}

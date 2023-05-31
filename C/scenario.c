#include "scenario.h"
#include <assert.h>
#include <stdio.h>
#include <string.h> // strcat

int input_command(char *allowed);

char choices_description_buffer[MAX_CHOICES * MAX_CHOICE_DESCRIPTION + 1];
char choices_key_buffer[MAX_CHOICES * 1 + 1];
char choice_action_buffer[MAX_CHOICE_DESCRIPTION + 1];

const char *scenario_describe(const Scenario *const scenario) {
    return scenario->description;
}

void choice_describe(const Choice *const choice) {
    strcat(choices_description_buffer, "\n\t ");
    strcat(choices_description_buffer, choice->description);
}

const char *scenario_describe_choices(const Scenario *const scenario) {
    choices_description_buffer[0] = '\0';
    for (int i = 0; i < scenario->number_of_choices; i++) {
        const Choice *const choice = &scenario->choices[i];
        choice_describe(choice);
    }
    return choices_description_buffer;
}

char choice_key_from_description(const char *description) {
    const char *openingParentheses = strchr(description, '(');
    assert(openingParentheses != 0); // must have ()
    const char *characterPtr = openingParentheses + 1;
    return *characterPtr;
}

char choice_get_key(const Choice *const choice) {
    char key = choice->key_to_press;
    if (key == 0) {
        key = choice_key_from_description(choice->description);
    }
    return key;
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

char *choice_action_from_description(const char *description) {
    size_t length = strlen(description);
    size_t index = 0;

    for (size_t i = 0; i < length; i++) {
        if (description[i] != '(' && description[i] != ')') {
            choice_action_buffer[index] = description[i];
            index++;
        }
    }

    choice_action_buffer[index] = '\0';
    return choice_action_buffer;
}

void choice_execute(const Choice *const choice) {
    char *action = choice->action;
    if (action == NULL || action[0] == '\0') {
        action = choice_action_from_description(choice->description);
    }
    printf("%s\n", action);
    choice->next_scenario();
}

void scenario_execute_choice(const Scenario *const scenario, int command) {
    for (int i = 0; i < scenario->number_of_choices; i++) {
        const Choice *const choice = &scenario->choices[i];
        if (command == choice_get_key(choice)) {
            choice_execute(choice);
            break;
        }
    }
}

void scenario(const Scenario *const scenario) {
    const char *description = scenario_describe(scenario);
    const char *choices = scenario_describe_choices(scenario);
    printf("%s. Would you like to:%s\n\n", description, choices);

    char *choice_keys = scenario_list_choice_keys(scenario);
    int command = input_command(choice_keys);
    assert(strchr(choice_keys, command) != NULL);

    scenario_execute_choice(scenario, command);
}

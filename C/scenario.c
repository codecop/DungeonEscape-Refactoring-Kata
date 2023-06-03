#include "scenario.h"
#include <stdio.h>

char choices_description_buffer[SCENARIO_MAX_CHOICES * CHOICE_MAX_DESCRIPTION + 1];
char choices_key_buffer[SCENARIO_MAX_CHOICES * 1 + 1];

int _scenario_string_not_empty(const char *string) {
    return string && string[0] != '\0';
}

const char *_scenario_describe(const Scenario *const scenario) {
    return scenario->description;
}

const char *_scenario_describe_choices(const Scenario *const scenario) {
    choices_description_buffer[0] = '\0';
    for (int i = 0; i < scenario->number_of_choices; i++) {
        const Choice *const choice = &scenario->choices[i];
        choice_describe(choice, choices_description_buffer);
    }
    return choices_description_buffer;
}

void scenario_introduce(const Scenario *const scenario) {
    const char *description = _scenario_describe(scenario);
    const char *choices = _scenario_describe_choices(scenario);
    if (_scenario_string_not_empty(choices)) {
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
            break;
        }
    }
}

const char *_fork_describe(const ScenarioFork *const scenario) {
    return scenario->description;
}

void scenario_execute_fork(const ScenarioFork *const scenario) {
    const char *description = _fork_describe(scenario);
    printf("%s.\n", description);

    if (scenario->is_first_choice()) {
        const Choice *const choice = &scenario->choices[0];
        choice_execute(choice);
    } else {
        const Choice *const choice = &scenario->choices[1];
        choice_execute(choice);
    }
}

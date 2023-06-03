#ifndef SCENARIO_H
#define SCENARIO_H

#include "choice.h"

#define SCENARIO_MAX_DESCRIPTION 190
#define SCENARIO_MAX_CHOICES 10

typedef struct Scenario {
    char description[SCENARIO_MAX_DESCRIPTION + 1];
    int number_of_choices;
    Choice choices[SCENARIO_MAX_CHOICES];
} Scenario;

void scenario_introduce(const Scenario *const scenario);
char *scenario_list_choice_keys(const Scenario *const scenario);
void scenario_execute_choice(const Scenario *const scenario, int choice_key);

#endif // SCENARIO_H

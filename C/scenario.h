#ifndef SCENARIO_H
#define SCENARIO_H

#include "choice.h"

extern int input_command(char *allowed);

#define SCENARIO_MAX_DESCRIPTION 190
#define SCENARIO_MAX_CHOICES 10

typedef struct Scenario {
    char description[SCENARIO_MAX_DESCRIPTION + 1];
    int number_of_choices;
    Choice choices[SCENARIO_MAX_CHOICES];
} Scenario;

void scenario_execute(const Scenario *const scenario);

#endif // SCENARIO_H

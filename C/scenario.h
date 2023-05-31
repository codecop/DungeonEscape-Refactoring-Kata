#ifndef SCENARIO_H
#define SCENARIO_H

#define MAX_SCENARIO_DESCRIPTION 190
#define MAX_CHOICES 10
#define MAX_CHOICE_DESCRIPTION 40

typedef struct Choice {
    char description[MAX_CHOICE_DESCRIPTION + 1]; // can be omitted/null
    char key_to_press; // can be omitted/null - derived from description
    char *action; // can be omitted/null - derived from description
    void (*next_scenario)(void);
} Choice;

typedef struct Scenario {
    char description[MAX_SCENARIO_DESCRIPTION + 1];
    int number_of_choices;
    Choice choices[MAX_CHOICES];
} Scenario;

void scenario(const Scenario *const scenario);

#endif // SCENARIO_H

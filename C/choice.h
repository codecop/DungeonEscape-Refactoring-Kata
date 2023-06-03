#ifndef CHOICE_H
#define CHOICE_H

#define CHOICE_MAX_DESCRIPTION 40

typedef struct Choice {
    char description[CHOICE_MAX_DESCRIPTION + 1]; // can be omitted/null
    char key_to_press;                            // can be omitted/null - derived from description
    char *action;                                 // can be omitted/null - derived from description
    void (*next_method)(void);                    // can be omitted/null
    struct Scenario *next_scenario;               // can be omitted/null
} Choice;

void choice_describe(const Choice *const choice, char *choices_description_buffer);
char choice_get_key(const Choice *const choice);
void choice_execute(const Choice *const choice);

#endif // CHOICE_H

#ifndef CHOICE_H
#define CHOICE_H

#include <stdbool.h>

#define CHOICE_MAX_DESCRIPTION 40

typedef struct Choice {
    bool (*is_enabled)(void);                     // can be omitted/null - true
    char description[CHOICE_MAX_DESCRIPTION + 1]; // can be omitted/null
    char key_to_press;                            // can be omitted/null - derived from description
    char *action;                                 // can be omitted/null - derived from description
    void (*next_method)(void);                    // can be omitted/null
} Choice;

bool choice_is_enabled(const Choice *const choice);
void choice_describe(const Choice *const choice, char *choices_description_buffer);
char choice_get_key(const Choice *const choice);
void choice_execute(const Choice *const choice);

#endif // CHOICE_H

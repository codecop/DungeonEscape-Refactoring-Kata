#include "choice.h"
#include <assert.h>
#include <stdio.h>
#include <string.h> // strchr

char choice_action_buffer[CHOICE_MAX_DESCRIPTION + 1];

int _choice_string_not_empty(const char *string) {
    return string && string[0] != '\0';
}

bool choice_is_enabled(const Choice *const choice) {
    return choice->is_enabled == NULL || choice->is_enabled();
}

void choice_describe(const Choice *const choice, char *choices_description_buffer) {
    if (!choice_is_enabled(choice)) {
        return;
    }

    const char *description = choice->description;
    if (_choice_string_not_empty(description)) {
        strcat(choices_description_buffer, "\n\t ");
        strcat(choices_description_buffer, description);
    }
}

char _choice_key_from_description(const char *description) {
    const char *openingParentheses = strchr(description, '(');
    assert(openingParentheses != 0); // must have ()
    const char *characterPtr = openingParentheses + 1;
    return *characterPtr;
}

char choice_get_key(const Choice *const choice) {
    char key = choice->key_to_press;
    if (key == 0) {
        assert(choice->description);
        key = _choice_key_from_description(choice->description);
    }
    return key;
}

char *_choice_action_from_description(const char *description) {
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

void _choice_print_action(const Choice *const choice) {
    char *action = choice->action;
    if (action == NULL || action[0] == '\0') {
        if (_choice_string_not_empty(choice->description)) {
            action = _choice_action_from_description(choice->description);
        };
    }
    if (_choice_string_not_empty(action)) {
        printf("%s\n", action);
    }
}

void choice_execute(const Choice *const choice) {
    _choice_print_action(choice);

    if (choice->next_method) {
        choice->next_method();
    }
}

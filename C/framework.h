#ifndef FRAMEWORK_H
#define FRAMEWORK_H

#include "scenario.h"

void game_user_choice_of_scenario(const Scenario *const scenario);
void game_start(void (*first_room)(void));
void game_won(void);
void game_lost(void);

#endif // FRAMEWORK_H

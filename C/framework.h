#include "scenario.h"

#ifndef FRAMEWORK_H
#define FRAMEWORK_H

void game_won(void);
void game_lost(void);
void start_game(void);
void execute_scenario(const Scenario *const scenario);

#endif // FRAMEWORK_H

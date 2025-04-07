#ifndef MATCH_H
#define MATCH_H

#include "automaton.h"

typedef struct {
    State* states[MAX_STATES];
    unsigned int count;
} StateSet;

/**
 * add_state - Add a state to look at if it has not already been visited
 * @param set:StateSet* -> Stack of states
 * @param state:State* -> Current state to peek
 * @param visited:char*(bool*) -> Array of booleans
 * @return void
 */
void add_state(StateSet*, State*, char*);
/**
 * step - Computes the next set of active states
 * @param current:StateSet* -> Current set of active states
 * @param c:char -> Current character to analyze
 * @param next:StateSet* -> Next set (or result) of active states
 * @return void
 */
void step(const StateSet*, char, StateSet*);
/**
 * match - Returns whether the given word is accepted by the given automaton
 * @param start:State* -> The starting state
 * @param final:State* -> The final state
 * @param input:char* -> The word to match
 * @return char(bool)
 */
char match(State*, const State*, const char*);

#endif //MATCH_H

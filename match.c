#include "match.h"

#include <stdio.h>

void add_state(StateSet* set, State* state, char *visited) {
    if (state && !visited[state->id]) {
        visited[state->id] = 1;
        set->states[set->count++] = state;

        // Epsilon closure
        if (state->transition == 0) {
            add_state(set, state->out1, visited);
            add_state(set, state->out2, visited);
        }
    }
}

void step(const StateSet* current, const char c, StateSet* next) {
    next->count = 0;
    char visited[MAX_STATES] = {0};

    for (unsigned int i = 0; i < current->count; i++) {
        const State* s = current->states[i];
        printf("|  Peeking State %d\n", s->id);
        if (s->transition == c && s->out1) {
            printf("|->Transition State %d -> State %d\n", s->id, s->out1->id);
            add_state(next, s->out1, visited);
        }
    }
}

char match(State* start, const State* final, const char* input) {
    StateSet current, next;

    current.count = 0;
    char visited[MAX_STATES] = {0};
    add_state(&current, start, visited);

    while (*input) {
        printf("Current character: '%c'\n----------------------\n", *input);
        step(&current, *input, &next);
        current = next;  // Shallow copy here
        input++;

        printf("----------------------\n");
    }

    for (unsigned int i = 0; i < current.count; i++) {
        if (current.states[i] == final) return 1;
    }
    return 0;
}
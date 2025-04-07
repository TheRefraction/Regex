#include <stdio.h>
#include <stdlib.h>

#include "automaton.h"

unsigned int state_counter = 0;

State* create_state(const char c, State* s1, State* s2) {
    if (state_counter >= MAX_STATES - 1) {
        printf("Error! Too many states!\n");
        exit(1);
    }

    State* state = malloc(sizeof(State));
    state->id = state_counter++;
    state->transition = c;
    state->out1 = s1;
    state->out2 = s2;

    return state;
}

void free_states(State* start) {
    char visited[MAX_STATES] = { 0 };
    State* stack[MAX_STATES] = {NULL};
    int top = 0;
    stack[top++] = start;

    while (top > 0) {
        State* s = stack[--top];
        if (s && s->id < MAX_STATES && !visited[s->id]) {
            visited[s->id] = 1;

            if (s->out1) stack[top++] = s->out1;
            if (s->out2) stack[top++] = s->out2;

            free(s);
        }
    }
}

Fragment tree_to_automaton_rec(Node* node) {
    Fragment res;
    res.start = NULL;
    res.final = NULL;

    if (!node) return res;

    switch (node->type) {
        case NODE_CHAR: {
            State* final = create_state(0, NULL, NULL);
            State* start = create_state(node->value, final, NULL);

            res.start = start;
            res.final = final;

            break;
        }

        case NODE_CONCAT: {
            const Fragment left = tree_to_automaton_rec(node->left);
            const Fragment right = tree_to_automaton_rec(node->right);

            left.final->transition = 0;
            left.final->out1 = right.start;

            res.start = left.start;
            res.final = right.final;

            break;
        }

        case NODE_UNION: {
            const Fragment left = tree_to_automaton_rec(node->left);
            const Fragment right = tree_to_automaton_rec(node->right);

            State* final = create_state(0, NULL, NULL);
            State* start = create_state(0, left.start, right.start);

            left.final->transition = 0;
            left.final->out1 = final;

            right.final->transition = 0;
            right.final->out1 = final;

            res.start = start;
            res.final = final;

            break;
        }

        case NODE_STAR: {
            const Fragment sub = tree_to_automaton_rec(node->left);

            State* final = create_state(0, NULL, NULL);
            State* start = create_state(0, sub.start, final);

            sub.final->transition = 0;
            sub.final->out1 = sub.start;
            sub.final->out2 = final;

            res.start = start;
            res.final = final;

            break;
        }
        case NODE_PLUS: { // A+ = AA*
            const Fragment sub = tree_to_automaton_rec(node->left);

            State* final = create_state(0, NULL, NULL);
            sub.final->transition = 0;
            sub.final->out1 = sub.start;  // loop
            sub.final->out2 = final;

            res.start = sub.start;
            res.final = final;

            break;
        }

        case NODE_OPTIONAL: { // A? = A | ε
            const Fragment sub = tree_to_automaton_rec(node->left);

            State* final = create_state(0, NULL, NULL);
            State* start = create_state(0, sub.start, final);

            sub.final->transition = 0;
            sub.final->out1 = final;

            res.start = start;
            res.final = final;

            break;
        }
    }

    return res;
}

Fragment tree_to_automaton(Node* node) {
    state_counter = 0;
    Fragment res = tree_to_automaton_rec(node);

    return res;
}

void print_automaton(State* start) {
    char visited[MAX_STATES] = {0};
    State* stack[MAX_STATES];
    unsigned int top = 0;
    stack[top++] = start;

    while (top > 0) {
        State* s = stack[--top];
        if (!s || visited[s->id]) continue;
        visited[s->id] = 1;

        printf("State %d: ", s->id);

        if (s->transition == 0) printf("\\0");
        else printf("'%c'", s->transition);

        if (s->out1) printf(" -> %d", s->out1->id);
        if (s->out2) printf(", %d", s->out2->id);

        putchar('\n');

        if (s->out1) stack[top++] = s->out1;
        if (s->out2) stack[top++] = s->out2;
    }
}
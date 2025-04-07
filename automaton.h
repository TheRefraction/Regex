#ifndef AUTOMATON_H
#define AUTOMATON_H

#define MAX_STATES 1024

#include "node.h"

typedef struct State {
    unsigned int id;
    char transition;
    struct State* out1;
    struct State* out2;
} State;

typedef struct Fragment {
    State* start;
    State* final;
} Fragment;

/**
 * create_state - Create a state for an automaton
 * @param c:char -> Transition character. Put /0 for epsilon
 * @param s1:State* -> Output state n°1
 * @param s2:State* -> Output state n°2
 * @return State*
 */
State* create_state(char, State*, State*);
/**
 * free_states - Free all states from a given starting state
 * @param start:State* -> The starting state to free
 * @return void
 */
void free_states(State*);

/**
 * tree_to_automaton_rec - Creates recursively an automaton from a given tree
 * @param node:Node* -> The node to transform
 * @return Fragment
 */
Fragment tree_to_automaton_rec(Node*);
/**
 * tree_to_automaton - Creates an automaton from a given tree
 * @param node:Node* -> The node to transform
 * @return Fragment
 */
Fragment tree_to_automaton(Node*);
/**
 * print_automaton - Prints whole automaton from the starting state
 * @param start:State* -> The automaton to print
 * @return void
 */
void print_automaton(State*);

#endif //AUTOMATON_H

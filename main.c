#include <stdio.h>

#include "automaton.h"
#include "match.h"
#include "node.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("USAGE: regex.exe <regex> <string>\n");
        return 1;
    }

    char* regex = argv[1];
    char* word = argv[2];

    Node* tree = parse(regex);

    printf("======================\nSyntax tree for regex: \"%s\"\n----------------------\n", regex);
    print_tree(tree);

    const Fragment automaton = tree_to_automaton(tree);
    printf("======================\nNon-Deterministic Finite Automaton:\n----------------------\n");
    print_automaton(automaton.start);

    free_node(tree);

    printf("======================\nMatching word \"%s\":\n----------------------\n", word);
    const char res = match(automaton.start, automaton.final, word);
    printf("\nWord \"%s\" is %s!\n", word, res ? "accepted" : "not accepted");

    free_states(automaton.start);

    return 0;
}
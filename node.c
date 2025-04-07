#include <stdlib.h>
#include <stdio.h>

#include "node.h"

const char* current;

/**
 * peek - Get current character
 * @return char
 */
char peek() {
    return *current;
}

/**
 * next - Get next character and updates pointer
 * @return char
 */
char next() {
    return *current++;
}

Node* allocate_node(const NodeType type, const char value, Node* left, Node* right) {
    Node* node = malloc(sizeof(Node));
    if (node) {
        node->type = type;
        node->value = value;
        node->left = left;
        node->right = right;
    }

    return node;
}

void free_node(Node* node) {
    if (node) {
        free_node(node->left);
        free_node(node->right);

        free(node);
    }
}

Node* parse_atom() {
    if (peek() == '(') {
        next();
        Node* node = parse_union();
        if (peek() == ')') {
            next();
        } else {
            printf("Error! Malformed regex!\n");
            exit(1);
        }
        return node;
    }

    if (peek() != 0 && peek() != ')' && peek() != '|' && peek() != '*') {
        return allocate_node(NODE_CHAR, next(), NULL, NULL);
    }

    return NULL;
}

Node* parse_repeat() {
    Node* node = parse_atom();

    while (1) {
        if (peek() == '*') {
            next();
            node = allocate_node(NODE_STAR, 0, node, NULL);
        } else if (peek() == '+') {
            next();
            node = allocate_node(NODE_PLUS, 0, node, NULL);
        } else if (peek() == '?') {
            next();
            node = allocate_node(NODE_OPTIONAL, 0, node, NULL);
        } else {
            break;
        }
    }

    return node;
}

Node* parse_concat() {
    Node* left = parse_repeat();
    while (peek() && peek() != '|' && peek() != ')') {
        Node* right = parse_repeat();
        if (!right) break;
        left = allocate_node(NODE_CONCAT, 0, left, right);
    }

    return left;
}

Node* parse_union() {
    Node* left = parse_concat();
    while (peek() == '|') {
        next();
        Node* right = parse_concat();
        left = allocate_node(NODE_UNION, 0, left, right);
    }
    return left;
}

Node* parse(const char* input) {
    current = input;
    return parse_union();
}

void print_tree_rec(Node* node, const char* prefix, const char is_last) {
    if (!node) return;

    printf("%s", prefix);
    printf("%s", is_last ? "|_ " : "|- ");

    switch (node->type) {
        case NODE_CHAR:
            printf("CHAR('%c')\n", node->value); break;
        case NODE_CONCAT:
            printf("CONCAT\n"); break;
        case NODE_UNION:
            printf("UNION\n"); break;
        case NODE_STAR:
            printf("STAR\n"); break;
        case NODE_PLUS:
            printf("PLUS\n"); break;
        case NODE_OPTIONAL:
            printf("OPTIONAL\n"); break;
        default:
            printf("UNKNOWN\n");
    }

    char new_prefix[1024];
    snprintf(new_prefix, sizeof(new_prefix), "%s%s", prefix, is_last ? "    " : "|   ");

    unsigned int child_count = 0;
    if (node->left) child_count++;
    if (node->right) child_count++;

    if (node->left)
        print_tree_rec(node->left, new_prefix, child_count == 1 && !node->right);

    if (node->right)
        print_tree_rec(node->right, new_prefix, 1);
}

void print_tree(Node* node) {
    printf("ROOT\n");
    print_tree_rec(node, "", 1);
}
#ifndef NODE_H
#define NODE_H

typedef enum ENUM_NODE_TYPE {
    NODE_CHAR = 0,
    NODE_CONCAT,
    NODE_UNION,
    NODE_STAR,
    NODE_PLUS,
    NODE_OPTIONAL
} NodeType;

typedef struct STRUCT_NODE {
    NodeType type;
    char value;
    struct STRUCT_NODE* left;
    struct STRUCT_NODE* right;
} Node;

/**
 * allocate_node - Allocate a new node
 * @param type:NodeType -> Type of the node to create
 * @param value:char -> Character of the current node
 * @param left:Node* -> Left node
 * @param right:Node* -> Right node
 * @return Node*
 */
Node* allocate_node(NodeType, char, Node*, Node*);
/**
 * free_node - Free all subnodes from given node then itself
 * @param node:Node* -> The node to free
 * @return void
 */
void free_node(Node*);
/**
 * print_tree_rec - Recursive printing of a given tree
 * @param node:Node* -> The node to print
 * @param prefix:char* -> A string that holds the "indentation" level
 * @param is_last:char(bool) -> Is the given node the last child?
 * @return void
 */
void print_tree_rec(Node*, const char*, char);
/**
 * print_tree - Print a tree
 * @param node:Node* -> The node to print
 * @return void
 */
void print_tree(Node*);

/**
 * parse_atom - Parse atomic and grouped expressions
 * @return Node*
 */
Node* parse_atom();
/**
 * parse_repeat - Parse closures within regular expressions
 * @return Node*
 */
Node* parse_repeat();
/**
 * parse_concat - Parse concatenation of two sub-regular expressions
 * @return Node*
 */
Node* parse_concat();
/**
 * parse_union - Parse unions of a given regular expression
 * @return Node*
 */
Node* parse_union();
/**
 * parse - Parse a regular expression
 * @return Node*
 */
Node* parse(const char*);

#endif //NODE_H

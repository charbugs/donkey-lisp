#ifndef PARSE_H
#define PARSE_H

#include "list.h"
#include "tokenize.h"

#define T_ROOT 0
#define T_APPL 1
#define T_STR 2
#define T_INT 3

typedef struct {
    List *children;
    int type;
    char *val;
} Node;

// List<Token> *tokens
Node *parse(List *tokens);
void print_ast(Node *node);
Node *new_node(int type, char *val);
char* type_to_string(int type);

#endif
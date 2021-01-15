#ifndef PARSE_H
#define PARSE_H

#include "list.h"
#include "tokenize.h"

#define T_ROOT  1
#define T_INT   2
#define T_STR   4
#define T_IDF   8
#define T_APPL  16
#define T_LST   32
#define T_NONE   64
#define T_FUN   128

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
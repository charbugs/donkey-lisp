#ifndef PARSE_H
#define PARSE_H

#include "list.h"
#include "tokenize.h"

#define NODE_ROOT 0
#define NODE_APPL 1
#define NODE_STR 2
#define NODE_INT 3

// generic node
typedef struct {
    int type;
    List *children;
} Node;

// root node
typedef struct {
    Node node;
    char* label;
} Root;

// function application node
typedef struct {
    Node node;
    char* func;
} Appl;

// string node
typedef struct {
    Node node;
    char *val;
} Str;

// integer node
typedef struct {
    Node node;
    int val;
} Int;

// List<Token> *tokens
Root *parse(List *tokens);
void print_ast(Node *node);
char* node_type_to_string(int type);

Root *new_root(char *label);
Appl *new_appl(char *func);
Str *new_str(char *val);
Int *new_int(int val);


#endif
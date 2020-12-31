#ifndef BUILDINS_H
#define BUILDINS_H

#include "parse.h"

Node *buildin_def(Node *appl);
Node *buildin_const(List *args);
Node *buildin_if(List* args);
Node *buildin_list(List *args);

Node *buildin_add(List *args);
Node *buildin_sub(List *args);
Node *buildin_mul(List *args);
Node *buildin_div(List *args);

Node *buildin_head(List *args);
Node *buildin_tail(List *args);
Node *buildin_eq(List *args);
Node *buildin_empty(List *args);
Node *buildin_cons(List *args);

Node *buildin_println(List *args);
Node *buildin_printstack(List *args);

Node *buildin_strlen(List *args);

#endif
#ifndef BUILDINS_H
#define BUILDINS_H

#include "parse.h"

Node *buildin_def(Node *appl);
Node *buildin_const(List *args);
Node *buildin_if(List* args);

// type check
Node *buildin_isint(List *args);
Node *buildin_isstr(List *args);
Node *buildin_isundefined(List *args);
Node *buildin_islist(List *args);

// math
Node *buildin_add(List *args);
Node *buildin_sub(List *args);
Node *buildin_mul(List *args);
Node *buildin_div(List *args);
Node *buildin_mod(List *args);

// list
Node *buildin_list(List *args);
Node *buildin_empty(List *args);
Node *buildin_cons(List *args);
Node *buildin_head(List *args);
Node *buildin_tail(List *args);
Node *buildin_last(List *args);
Node *buildin_init(List *args);

// compare
Node *buildin_eq(List *args);

//logic
Node *buildin_not(List *args);
Node *buildin_and(List *args);
Node *buildin_or(List *args);

// io
Node *buildin_println(List *args);

// debug
Node *buildin_printstack(List *args);

#endif
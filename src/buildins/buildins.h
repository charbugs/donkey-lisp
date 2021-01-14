#ifndef BUILDINS_H
#define BUILDINS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../parse.h"
#include "../resolve.h"
#include "../stack.h"

// utils
int to_boolean(Node *node);
void assert_arg_type(char* func, Node *arg, int pos, int valid_type);
void assert_args_len(char* func, List *args, int len);
void assert_args(char *func, List* args, int len, int valid_types[]);

// misc
Node *buildin_define(List *args);
Node *buildin_func(List *args);
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
Node *buildin_isempty(List *args);
Node *buildin_cons(List *args);
Node *buildin_head(List *args);
Node *buildin_tail(List *args);
Node *buildin_last(List *args);
Node *buildin_init(List *args);

// compare
Node *buildin_eq(List *args);
Node *buildin_lt(List *args);
Node *buildin_gt(List *args);
Node *buildin_le(List *args);
Node *buildin_ge(List *args);

//logic
Node *buildin_not(List *args);
Node *buildin_and(List *args);
Node *buildin_or(List *args);

// io
Node *buildin_print(List *args);

// debug
Node *buildin_printstack(List *args);

#endif
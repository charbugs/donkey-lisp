#ifndef BUILDINS_H
#define BUILDINS_H

#include "parse.h"

Node *buildin_add(List *args);
Node *buildin_sub(List *args);
Node *buildin_mul(List *args);
Node *buildin_div(List *args);
Node *buildin_strlen(List *args);
Node *buildin_const(List *args);
Node *buildin_if(List* args);
Node *buildin_def(Node *def);

#endif
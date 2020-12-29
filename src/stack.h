#ifndef STACK_H
#define STACK_H

#include "parse.h"

#define MAX_STACK_ITEMS 100000

typedef struct {
    char* name;
    Node *node;
} StackItem;

void stack_push(char* name, Node* node);
Node *stack_pop();
Node* stack_get(char* name);
void print_stack();

#endif
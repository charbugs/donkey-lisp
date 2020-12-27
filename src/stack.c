#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"


static StackItem stack[MAX_STACK_ITEMS];
static int count = 0;

void stack_push(char* name, Node* node) {
    int type = node->type;
    
    if (type == T_INT || type == T_STR || T_APPL) {
        stack[count].name = name;
        stack[count].node = node;
        count++;
    } 
    else {
        printf("stack: only string and int values can be stored on stack, got %s",
            type_to_string(type));
        exit(1);
    }
}

Node *stack_get(char *name) {
    for (int i = count - 1; i > -1; i--) {
        if (strcmp(stack[i].name, name) == 0) {
            return stack[i].node;
        }
    }
    return NULL;
}

void print_stack() {
    for (int i = 0; i < count; i++) {
        printf("%s: %s: %s\n",
            stack[i].name,
            type_to_string(stack[i].node->type),
            stack[i].node->val);
    }
}

Node *stack_pop() {
    count--;
    Node *ret = stack[count].node;
    stack[count].node = NULL;
    stack[count].name = "__removed__";
    return ret;
}
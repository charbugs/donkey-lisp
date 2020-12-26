#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buildins.h"
#include "parse.h"
#include "resolve.h"
#include "stack.h"


static int argtoi(List *args, int pos) {
    return atoi(((Node*)list_get(args, pos))->val);
}

static char* argtos(List *args, int pos) {
    return ((Node*)list_get(args, pos))->val;
}

static char* itos(int i) {
    char *s = malloc(sizeof(char) * 20);
    sprintf(s, "%d", i);
    return s;
}

static void check_args(char *func, List* args, int len, int types[]) {
    if (len != args->length) {
        printf("buildins: expected %d arguments for function %s but got %d\n",
            len, func, args->length);
        exit(1);
    }

    for (int i = 0; i < len; i++) {
        Node *arg = list_get(args, i);
        if (arg->type != types[i]) {
            printf("buildins: expected type %d for argument %d of function %s but got type %d\n",
                types[i], i, func, arg->type);
            exit(1);
        }
    }
}

static int _buildin_add (int x, int y) {
    return x + y;
}

Node *buildin_add(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT};
    check_args("add", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_add(arg1, arg2));
    return new_node(T_INT, res); 
}


static int _buildin_sub(int x, int y) {
    return x - y;
}

Node *buildin_sub(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT};
    check_args("sub", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_sub(arg1, arg2));   
    return new_node(T_INT, res);
}

static int _buildin_strlen(char* s) {
    return strlen(s);
}

Node *buildin_strlen(List *args) {
    args = resolve_all(args);
    int types[] = { T_STR};
    check_args("strlen", args, 1, types);
    char* arg1 = argtos(args, 0);
    char *res = itos(_buildin_strlen(arg1));
    return new_node(T_INT, res);
}

Node *buildin_const(List *args) {
    Node *idf = list_get(args, 0);
    Node *const_node = resolve(list_get(args, 1));
    if (idf->type != T_IDF) {
        printf ("buildin const: expected an identifier as first argument but got type %d\n",
            idf->type);
        exit(1);
    }

    if (const_node->type != T_INT && const_node->type != T_STR) {
        printf("buildin const: can only put values of type int or string on the stack, got %d\n",
            const_node->type);
        exit(1);
    }

    if (stack_get(idf->val) != NULL) {
        printf("buildin const: overwriting const values is not allowed: %s\n", idf->val);
        exit(1);
    }

    stack_push(idf->val, const_node);
    return const_node;
}
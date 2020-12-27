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

static int to_boolean(Node *node) {
    if (node->type == T_INT) {
        return atoi(node->val) != 0;
    } else if (node->type == T_STR) {
        return strlen(node->val) > 0;
    } else {
        printf("can not evaluate value of type %d to boolean\n", node->type);
        exit(1);
    }
}

static void assert_arg_type(char* func, Node *arg, int pos, int valid_type) {
    if ((arg->type & valid_type) == 0) {
        printf("assert args: expected typ<e %d for argument %d of function %s but got type %d\n",
                valid_type, pos, func, arg->type);
        exit(1);
    }
}

static void assert_args_len(char* func, List *args, int len) {
    if (len != args->length) {
        printf("assert args: expected %d arguments for function %s but got %d\n",
            len, func, args->length);
        exit(1);
    }
}

static void assert_args(char *func, List* args, int len, int valid_types[]) {
    assert_args_len(func, args, len);

    for (int i = 0; i < len; i++) {
        Node *arg = list_get(args, i);
        assert_arg_type(func, arg, i + 1, valid_types[i]);
    }
}

static int _buildin_add (int x, int y) {
    return x + y;
}

Node *buildin_add(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args("add", args, 2, types);
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
    int types[] = { T_INT, T_INT };
    assert_args("sub", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_sub(arg1, arg2));   
    return new_node(T_INT, res);
}

static int _buildin_mul(int x, int y) {
    return x * y;
}

Node *buildin_mul(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args("sub", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_mul(arg1, arg2));   
    return new_node(T_INT, res);
}

static int _buildin_div(int x, int y) {
    return x / y;
}

Node *buildin_div(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args("sub", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_div(arg1, arg2));   
    return new_node(T_INT, res);
}

static int _buildin_strlen(char* s) {
    return strlen(s);
}

Node *buildin_strlen(List *args) {
    args = resolve_all(args);
    int types[] = { T_STR };
    assert_args("strlen", args, 1, types);
    char* arg1 = argtos(args, 0);
    char *res = itos(_buildin_strlen(arg1));
    return new_node(T_INT, res);
}

Node *buildin_const(List *args) {
    assert_args_len("const", args, 2);
    // the identifier node must not be resolved here
    Node *idf = list_get(args, 0);
    assert_arg_type("const", idf, 1, T_IDF);
    // so we resolve only the next argument to get the constant value
    Node *const_node = resolve(list_get(args, 1));
    assert_arg_type("const", const_node, 2, T_INT | T_STR);

    if (stack_get(idf->val) != NULL) {
        printf("buildin const: overwriting const values is not allowed: %s\n", idf->val);
        exit(1);
    }

    stack_push(idf->val, const_node);
    return const_node;
}

Node *buildin_if(List* args) {
    int types[] = {
        T_INT | T_STR | T_IDF | T_APPL,
        T_INT | T_STR | T_IDF | T_APPL,
        T_INT | T_STR | T_IDF | T_APPL,
    };
    assert_args("if", args, 3, types);
    Node *condition = list_get(args, 0);
    Node *true_node = list_get(args, 1);
    Node *false_node = list_get(args, 2);

    if (to_boolean(resolve(condition))) {
        return resolve(true_node);       
    } else {
        return resolve(false_node);
    }
}

Node *buildin_def(Node *def) {
    List *args = def->children;
    int types[] = { T_IDF, T_APPL, T_APPL };
    assert_args("def", args, 3, types);

    Node *name = list_get(args, 0);
    Node *params = list_get(args, 1);

    if (strcmp(params->val, "params") != 0) {
        printf("def: 2nd argument to def must be a parameter list: %s", name->val);
        exit(1);
    }
    
    for (int i = 0; i < params->children->length; i++) {
        Node *param = list_get(params->children, i);
        if (param->type != T_IDF) {
            printf("def: each parameter must be an identifier: %s\n", name->val);
            exit(1);
        }
    }

    if (stack_get(name->val) != NULL) {
        printf("def: redefine function is not allowed: %s\n", name->val);
        exit(1);
    }

    // we store a pointer to the entire definition.
    stack_push(name->val, def);
    return name;   
}
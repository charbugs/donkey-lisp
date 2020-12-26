#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "exec.h"
#include "parse.h"
#include "buildins.h"
#include "stack.h"


static void check_signature(char* func, List *args) {
    Node *node;
    Signature *sig = get_signature(func);

    if (!sig) {
        printf("exec: could not find signature for function %s\n", func);
        exit(1);
    }

    if (args->length != sig->arg_count) {
        printf("exec: function %s takes %d arguments but got %d\n", 
            func, sig->arg_count, args->length);
        exit(1);
    }

    for (int i = 0; i < sig->arg_count; i++) {
        node = list_get(args, i);
        if (node->type != sig->arg_types[i]) {
            printf("exec: argument #%d for function %s should be %s but got %s\n", i + 1, func, 
                type_to_string(sig->arg_types[i]),
                type_to_string(node->type));
            exit(1);
        }
    }

}


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

static Node *call(Node *appl) {
    char *func = appl->val;
    List *args = appl->children;

    check_signature(func, args);

    if (strcmp(func, "add") == 0) {
        int arg1 = argtoi(args, 0);
        int arg2 = argtoi(args, 1);
        char *res = itos(buildin_add(arg1, arg2)); 
        return new_node(T_INT, res);
    } 
    else if (strcmp(func, "sub") == 0) {
        int arg1 = argtoi(args, 0);
        int arg2 = argtoi(args, 1);
        char *res = itos(buildin_sub(arg1, arg2));
        return new_node(T_INT, res);
    }
    else if (strcmp(func, "strlen") == 0) {
        char* arg1 = argtos(args, 0);
        char *res = itos(buildin_strlen(arg1));
        return new_node(T_INT, res);
    }
    else {
        printf("exec: unknown function name: %s\n", func);
        exit(1);
    }
}

static Node *resolve (Node *node) {
    Node *child;

    if (node->type == T_INT || node->type == T_STR) {
        return node;
    }

    if (node->type == T_IDF) {
        Node *stack_item = stack_get(node->val);
        if (stack_item == NULL) {
            printf("exec: could not find constant %s on the stack\n", node->val);
            exit(1);
        }
        return stack_item;
    }

    if (node->type == T_APPL) {
        for (int i = 0; i < node->children->length; i++) {
            child = list_get(node->children, i);
            list_replace(node->children, i, resolve(child));
        }
        return call(node);
    }

    printf("exec: resolve: unknown node type %d\n", node->type);
    exit(1);
}

void handle_const(Node* appl) {
    char *name = ((Node*)list_get(appl->children, 0))->val;
    Node *item = list_get(appl->children, 1);

    if (stack_get(name) == NULL) {
        stack_push(name, resolve(item));
    } else {
        printf("exec: trying to overwrite constant %s\n", name);
        exit(1);
    }

}

Node *exec(Node *root) {
    Node *child;
    Node *new_root = new_node(T_ROOT, "executed root");
    for (int i = 0; i < root->children->length; i++) {

        child = list_get(root->children, i);
        
        if (child->type == T_APPL) {
            if (strcmp(child->val, "const") == 0) {
                handle_const(child);
            }
            else {
                list_push(new_root->children, resolve(child));
            }
        }
    }

    return new_root;
}
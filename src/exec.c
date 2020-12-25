#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "exec.h"
#include "parse.h"
#include "buildins.h"


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
                node_type_to_string(sig->arg_types[i]),
                node_type_to_string(node->type));
            exit(1);
        }
    }

}

static Node *call(Appl *appl) {
    char *func = appl->func;
    List *args = appl->node.children;

    check_signature(func, args);

    if (strcmp(func, "add") == 0) {
        int arg1 = ((Int*)list_get(args, 0))->val;
        int arg2 = ((Int*)list_get(args, 1))->val;
        return (Node*) new_int(buildin_add(arg1, arg2));
    } 
    else if (strcmp(func, "sub") == 0) {
        int arg1 = ((Int*)list_get(args, 0))->val;
        int arg2 = ((Int*)list_get(args, 1))->val;
        return (Node*) new_int(buildin_sub(arg1, arg2));
    }
    else if (strcmp(func, "strlen") == 0) {
        char* arg1 = ((Str*)list_get(args, 0))->val;
        return (Node*) new_int(buildin_strlen(arg1));
    }
    else {
        printf("exec: unknown function name: %s\n", func);
        exit(1);
    }
}

static Node *resolve (Node *node) {
    Node *child;

    if (node->type == NODE_INT || node->type == NODE_STR) {
        return node;
    }

    if (node->type == NODE_APPL) {
        for (int i = 0; i < node->children->length; i++) {
            child = list_get(node->children, i);
            list_replace(node->children, i, resolve(child));
        }
        return call((Appl*)node);
    }

    printf("exec: resolve: unknown node type\n");
    exit(1);
}

void exec(Root *root) {
    Node *child;
    for (int i = 0; i < root->node.children->length; i++) {
        child = list_get(root->node.children, i);
        if (child->type == NODE_APPL) {
            list_replace(root->node.children, i, resolve(child));
        }
    }
}
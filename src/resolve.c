#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "resolve.h"
#include "parse.h"
#include "buildins.h"
#include "stack.h"


static Node *resolve_appl(Node *appl) {
    char *func = appl->val;
    List *args = appl->children;

    // TODO: lookup table that maps function names to ints 
    // so that we can avoid that many strcmps
    if (strcmp(func, "add") == 0) return buildin_add(args);
    if (strcmp(func, "sub") == 0) return buildin_sub(args);
    if (strcmp(func, "strlen") == 0) return buildin_strlen(args);
    if (strcmp(func, "strlen") == 0) return buildin_strlen(args);
    if (strcmp(func, "const") == 0) return buildin_const(args);
    if (strcmp(func, "if") == 0) return buildin_if(args);    

    printf("exec: unknown function name: %s\n", func);
    exit(1);
}

static Node *resolve_idf(Node *idf) {
    Node *stack_item = stack_get(idf->val);
    if (stack_item == NULL) {
        printf("resolve: could not find stack item %s\n", idf->val);
        exit(1);
    }
    return stack_item;
}

static Node *resolve_root(Node *root) {
    Node *ret = new_node(T_ROOT, "executed root");
    for (int i = 0; i < root->children->length; i++) {
        Node *child = list_get(root->children, i);
        if (child->type == T_APPL) {
            list_push(ret->children, resolve(child));
        }
    }
    return ret;
}

Node *resolve(Node *node) {
    switch(node->type) {
        case T_INT: return node;
        case T_STR: return node;
        case T_IDF: return resolve_idf(node);
        case T_APPL: return resolve_appl(node);
        case T_ROOT: return resolve_root(node);
    }

    printf("resolve: unknown node type %d\n", node->type);
    exit(1);
}

List *resolve_all(List *nodes) {
    List *new = list_create();
    Node *node;
    for (int i = 0; i < nodes->length; i++) {
        node = list_get(nodes, i);
        list_push(new, resolve(node));
    }
    return new;
}
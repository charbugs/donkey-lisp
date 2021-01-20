#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "resolve.h"
#include "parse.h"
#include "buildins/buildins.h"
#include "stack.h"

static Node *resolve_appl(Node *appl) {
    char *fname = appl->val;
    List *args = appl->children;

    // TODO: lookup table that maps function names to ints 
    // so that we can avoid that many strcmps
    if (strcmp(fname, "int?") == 0) return buildin_isint(args);
    if (strcmp(fname, "str?") == 0) return buildin_isstr(args);
    if (strcmp(fname, "list?") == 0) return buildin_islist(args);
    if (strcmp(fname, "none?") == 0) return buildin_isnone(args);
    if (strcmp(fname, "function?") == 0) return buildin_isfunction(args);
    if (strcmp(fname, "+") == 0) return buildin_add(args);
    if (strcmp(fname, "-") == 0) return buildin_sub(args);
    if (strcmp(fname, "*") == 0) return buildin_mul(args);
    if (strcmp(fname, "/") == 0) return buildin_div(args);
    if (strcmp(fname, "mod") == 0) return buildin_mod(args);
    if (strcmp(fname, "define") == 0) return buildin_define(args);
    if (strcmp(fname, "if") == 0) return buildin_if(args);    
    if (strcmp(fname, "list") == 0) return buildin_list(args);
    if (strcmp(fname, "head") == 0) return buildin_head(args);
    if (strcmp(fname, "tail") == 0) return buildin_tail(args);
    if (strcmp(fname, "last") == 0) return buildin_last(args);
    if (strcmp(fname, "init") == 0) return buildin_init(args);
    if (strcmp(fname, "=") == 0) return buildin_eq(args);
    if (strcmp(fname, "<") == 0) return buildin_lt(args);
    if (strcmp(fname, ">") == 0) return buildin_gt(args);
    if (strcmp(fname, "<=") == 0) return buildin_le(args);
    if (strcmp(fname, ">=") == 0) return buildin_ge(args);
    if (strcmp(fname, "not") == 0) return buildin_not(args);
    if (strcmp(fname, "and") == 0) return buildin_and(args);
    if (strcmp(fname, "or") == 0) return buildin_or(args);
    if (strcmp(fname, "empty?") == 0) return buildin_isempty(args);
    if (strcmp(fname, "cons") == 0) return buildin_cons(args);
    if (strcmp(fname, "append") == 0) return buildin_append(args);
    if (strcmp(fname, "print") == 0) return buildin_print(args);
    if (strcmp(fname, "printstack") == 0) return buildin_printstack(args);
    if (strcmp(fname, "none") == 0) return buildin_none();
    if (strcmp(fname, "->") == 0) return buildin_func(args);

    return buildin_call(fname, args);
}

static Node *resolve_idf(Node *idf) {
    Node *stack_item = stack_get(idf->val);
    if (stack_item == NULL) {
        printf("resolve: could not find identifier %s on the stack\n", idf->val);
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
        case T_FUN: return node;
        case T_NONE: return node;
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
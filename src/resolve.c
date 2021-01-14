#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "resolve.h"
#include "parse.h"
#include "buildins/buildins.h"
#include "stack.h"


static Node* call(char* fname, List *args) {
    args = resolve_all(args);
    
    Node *body, *locals, *param, *func = stack_get(fname); // T_FUN
    int param_len = 0;
    
    if (func == NULL) {
        printf("call: could not find function %s\n", fname);
        exit(1);
    }

    for (int i = 0; i < func->children->length - 1; i++) {
        param = list_get(func->children, i);
        if (param->type != T_IDF) {
            break;
        } else {
            param_len++;
        }
    }

    if (args->length != param_len) {
        printf("call: expected %d arguments for function %s but got %d",
            param_len, fname, args->length);
        exit(1);
    }

    for (int i = 0; i < param_len; i++) {
        char* name = ((Node*)list_get(func->children, i))->val;
        Node* object = list_get(args, i);
        stack_push(name, object);
    }
    
    if (list_get(func->children, param_len + 1) == NULL) {
        locals = NULL;
        body = list_get(func->children, param_len);
    } else {
        locals = list_get(func->children, param_len);
        body = list_get(func->children, param_len + 1);
    }

    if (locals) {
        for (int i = 0; i < locals->children->length; i += 2) {
            char* name = ((Node*)list_get(locals->children, i))->val;
            Node* object = resolve(list_get(locals->children, i + 1));
            stack_push(name, object);
        }
    }

    Node *ret = resolve(body);

    if (locals) {
        for (int i = 0; i < locals->children->length; i += 2) {
            stack_pop();
        } 
    }

    for (int i = 0; i < param_len; i++) {
        stack_pop();
    }

    return ret;
}

static Node *resolve_appl(Node *appl) {
    char *func = appl->val;
    List *args = appl->children;

    // TODO: lookup table that maps function names to ints 
    // so that we can avoid that many strcmps
    if (strcmp(func, "int?") == 0) return buildin_isint(args);
    if (strcmp(func, "str?") == 0) return buildin_isstr(args);
    if (strcmp(func, "list?") == 0) return buildin_islist(args);
    if (strcmp(func, "undefined?") == 0) return buildin_isundefined(args);
    if (strcmp(func, "+") == 0) return buildin_add(args);
    if (strcmp(func, "-") == 0) return buildin_sub(args);
    if (strcmp(func, "*") == 0) return buildin_mul(args);
    if (strcmp(func, "/") == 0) return buildin_div(args);
    if (strcmp(func, "mod") == 0) return buildin_mod(args);
    if (strcmp(func, "define") == 0) return buildin_define(args);
    if (strcmp(func, "if") == 0) return buildin_if(args);    
    if (strcmp(func, "list") == 0) return buildin_list(args);
    if (strcmp(func, "head") == 0) return buildin_head(args);
    if (strcmp(func, "tail") == 0) return buildin_tail(args);
    if (strcmp(func, "last") == 0) return buildin_last(args);
    if (strcmp(func, "init") == 0) return buildin_init(args);
    if (strcmp(func, "=") == 0) return buildin_eq(args);
    if (strcmp(func, "<") == 0) return buildin_lt(args);
    if (strcmp(func, ">") == 0) return buildin_gt(args);
    if (strcmp(func, "<=") == 0) return buildin_le(args);
    if (strcmp(func, ">=") == 0) return buildin_ge(args);
    if (strcmp(func, "not") == 0) return buildin_not(args);
    if (strcmp(func, "and") == 0) return buildin_and(args);
    if (strcmp(func, "or") == 0) return buildin_or(args);
    if (strcmp(func, "empty?") == 0) return buildin_isempty(args);
    if (strcmp(func, "cons") == 0) return buildin_cons(args);
    if (strcmp(func, "print") == 0) return buildin_print(args);
    if (strcmp(func, "printstack") == 0) return buildin_printstack(args);    
    if (strcmp(func, "->") == 0) return buildin_func(args);

    return call(func, args);
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
        case T_FUN: return node;
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
#include "buildins.h"


Node *buildin_not(List *args) {
    args = resolve_all(args);
    assert_args_len("not", args, 1);
    Node *node = list_get(args, 0);
    return to_boolean(node) == 1
        ? new_node(T_INT, "0")
        : new_node(T_INT, "1");
}

Node *buildin_and(List *args) {
    args = resolve_all(args);
    Node *node;

    if (args->length < 1) {
        printf("and: need at least one argument\n");
        exit(1);
    }

    for (int i = 0; i < args->length; i++) {
        node = list_get(args, i);
        if (!to_boolean(node)) {
            break;
        }
    }
    
    return node;
}

Node *buildin_or(List *args) {
    args = resolve_all(args);
    Node *node;

    if (args->length < 1) {
        printf("or: need at least one argument\n");
        exit(1);
    }

    for (int i = 0; i < args->length; i++) {
        node = list_get(args, i);
        if (to_boolean(node)) {
            break;
        }
    }
    
    return node;
}
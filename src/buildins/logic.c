#include "buildins.h"

int to_boolean(Node *node) {
    if (node->type == T_INT) {
        return atoi(node->val) != 0;
    }
    else if (node->type == T_STR) {
        return strlen(node->val) > 0;
    }
    else if (node->type == T_NONE) {
        return 0;
    }
    else if (node->type == T_LST) {
        return 1;
    }
    else if (node->type == T_FUN) {
        return 1;
    }
    else {
        printf("can not evaluate value of type %s to boolean\n",
            type_to_string(node->type));
        exit(1);
    }
}

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
        printf("function and: need at least one argument\n");
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
        printf("function or: need at least one argument\n");
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
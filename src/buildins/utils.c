#include "buildins.h"

int to_boolean(Node *node) {
    if (node->type == T_INT) {
        return atoi(node->val) != 0;
    }
    else if (node->type == T_STR) {
        return strlen(node->val) > 0;
    }
    else if (node->type == T_UND) {
        return 0;
    }
    else if (node->type == T_LST) {
        return 1;
    }
    else {
        printf("can not evaluate value of type %d to boolean\n", node->type);
        exit(1);
    }
}

void assert_arg_type(char* func, Node *arg, int pos, int valid_type) {
    if ((arg->type & valid_type) == 0) {
        printf("assert args: expected typ<e %d for argument %d of function %s but got type %d\n",
                valid_type, pos, func, arg->type);
        exit(1);
    }
}

void assert_args_len(char* func, List *args, int len) {
    if (len != args->length) {
        printf("assert args: expected %d arguments for function %s but got %d\n",
            len, func, args->length);
        exit(1);
    }
}

void assert_args(char *func, List* args, int len, int valid_types[]) {
    assert_args_len(func, args, len);

    for (int i = 0; i < len; i++) {
        Node *arg = list_get(args, i);
        assert_arg_type(func, arg, i + 1, valid_types[i]);
    }
}
#include "buildins.h"


void assert_arg_type(char* func, Node *arg, int pos, int valid_type) {
    if ((arg->type & valid_type) == 0) {
        printf("function %s: expected type %s for argument %d but got %s\n",
                func, type_to_string(valid_type), pos, type_to_string(arg->type));
        exit(1);
    }
}

void assert_args_len(char* func, List *args, int len) {
    if (len != args->length) {
        printf("function %s: expected %d arguments but got %d\n",
            func, len, args->length);
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
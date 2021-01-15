#include "buildins.h"


Node *buildin_define(List *args) {
    assert_args_len("define", args, 2);
    // the identifier node must not be resolved here
    Node *idf = list_get(args, 0);
    assert_arg_type("const", idf, 1, T_IDF);
    // so we resolve only the next argument to get the constant value
    Node *object = resolve(list_get(args, 1));
    assert_arg_type("const", object, 2, T_INT | T_STR | T_LST | T_FUN);

    if (stack_get(idf->val) != NULL) {
        printf("define: overwriting constant values is not allowed: %s\n", idf->val);
        exit(1);
    }

    stack_push(idf->val, object);
    return new_node(T_UND, "undefined");
}

Node *buildin_func(List *args) {
    Node *cur;
    int i = args->length; 

    // get last arg
    cur = list_get(args, --i);
    if (!cur) {
        printf("function expression: function body missing\n");
        exit(1);
    }

    // get second last arg
    cur = list_get(args, --i);
    if (cur && cur->type == T_APPL && strcmp(cur->val, "locals") == 0) {

        if (cur->children->length % 2 != 0) {
            printf("function expression: number of arguments to the locals list must be even (pairs of key and value)\n");
            exit(1);
        }

        for (int i = 0; i < cur->children->length; i +=2) {
            Node *key = list_get(cur->children, i);
            if (key->type != T_IDF) {
                printf("function expression: expected an identifier at position %d in the locals list but got %s\n",
                    i, type_to_string(key->type));
                exit(1);
            }
        }

        // get third last arg
        cur = list_get(args, --i);
    }

    while ((cur = list_get(args, i--)) != NULL) {
        if (cur->type != T_IDF) {
            printf("function expression: parameter list may only contain identifiers\n");
            exit(1);
        }
    }

    Node *func = new_node(T_FUN, "<function>");
    func->children = args;
    return func;
}

Node *buildin_if(List* args) {
    int types[] = {
        T_INT | T_STR | T_IDF | T_APPL | T_UND | T_LST | T_FUN,
        T_INT | T_STR | T_IDF | T_APPL | T_UND | T_LST | T_FUN,
        T_INT | T_STR | T_IDF | T_APPL | T_UND | T_LST | T_FUN,
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

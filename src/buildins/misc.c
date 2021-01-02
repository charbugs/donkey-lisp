#include "buildins.h"


Node *buildin_const(List *args) {
    assert_args_len("const", args, 2);
    // the identifier node must not be resolved here
    Node *idf = list_get(args, 0);
    assert_arg_type("const", idf, 1, T_IDF);
    // so we resolve only the next argument to get the constant value
    Node *const_node = resolve(list_get(args, 1));
    assert_arg_type("const", const_node, 2, T_INT | T_STR | T_LST);

    if (stack_get(idf->val) != NULL) {
        printf("buildin const: overwriting const values is not allowed: %s\n", idf->val);
        exit(1);
    }

    stack_push(idf->val, const_node);
    return new_node(T_UND, "");
}

Node *buildin_if(List* args) {
    int types[] = {
        T_INT | T_STR | T_IDF | T_APPL | T_UND,
        T_INT | T_STR | T_IDF | T_APPL | T_UND,
        T_INT | T_STR | T_IDF | T_APPL | T_UND,
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

Node *buildin_def(Node *appl) {
    Node *name, *params, *locals = NULL;
    List *args = appl->children;

    if (args->length == 3) {
        int types[] = { T_IDF, T_APPL, T_APPL };
        assert_args("def", args, 3, types);
        name = list_get(args, 0);
        params = list_get(args, 1);
    }
    else if (args->length == 4) {
        int types[] = { T_IDF, T_APPL, T_APPL, T_APPL };
        assert_args("def", args, 4, types);
        name = list_get(args, 0);
        params = list_get(args, 1);
        locals = list_get(args, 2);
    }
    else {
        printf("def takes 3 or 4 arguments got %d\n", args->length);
        exit(1);
    }

    if (strcmp(params->val, "params") != 0) {
        printf("second argument to def must be a parameter list: %s", name->val);
        exit(1);
    }

    for (int i = 0; i < params->children->length; i++) {
        Node *param = list_get(params->children, i);
        if (param->type != T_IDF) {
            printf("def: each parameter must be an identifier: %s\n", name->val);
            exit(1);
        }
    }

    if (locals) {
        if (strcmp(locals->val, "locals") != 0) {
            printf("third parameter to def must be a locals list if 4 arguments were passed, got %s\n", locals->val);
            exit(1);
        }

        if (locals->children->length % 2 != 0) {
            printf("number of arguments to locals must be even\n");
            exit(1);
        }

        for (int i = 0; i < locals->children->length; i +=2) {
            Node *key = list_get(locals->children, i);
            if (key->type != T_IDF) {
                printf("expected an identifier at position %d in the locals list to %s but got %d\n",
                    i, name->val, key->type);
                exit(1);
            }
        }
    }

    if (stack_get(name->val) != NULL) {
        printf("def: redefine function is not allowed: %s\n", name->val);
        exit(1);
    }

    stack_push(name->val, appl);
    return new_node(T_UND, "");
}
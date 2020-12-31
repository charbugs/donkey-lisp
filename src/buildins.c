#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "buildins.h"
#include "parse.h"
#include "resolve.h"
#include "stack.h"


static int argtoi(List *args, int pos) {
    return atoi(((Node*)list_get(args, pos))->val);
}

static char* itos(int i) {
    char *s = malloc(sizeof(char) * 20);
    sprintf(s, "%d", i);
    return s;
}

static int to_boolean(Node *node) {
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

static void assert_arg_type(char* func, Node *arg, int pos, int valid_type) {
    if ((arg->type & valid_type) == 0) {
        printf("assert args: expected typ<e %d for argument %d of function %s but got type %d\n",
                valid_type, pos, func, arg->type);
        exit(1);
    }
}

static void assert_args_len(char* func, List *args, int len) {
    if (len != args->length) {
        printf("assert args: expected %d arguments for function %s but got %d\n",
            len, func, args->length);
        exit(1);
    }
}

static void assert_args(char *func, List* args, int len, int valid_types[]) {
    assert_args_len(func, args, len);

    for (int i = 0; i < len; i++) {
        Node *arg = list_get(args, i);
        assert_arg_type(func, arg, i + 1, valid_types[i]);
    }
}

static int _buildin_add (int x, int y) {
    return x + y;
}

Node *buildin_add(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args("+", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_add(arg1, arg2));
    return new_node(T_INT, res); 
}


static int _buildin_sub(int x, int y) {
    return x - y;
}

Node *buildin_sub(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args("-", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_sub(arg1, arg2));   
    return new_node(T_INT, res);
}

static int _buildin_mul(int x, int y) {
    return x * y;
}

Node *buildin_mul(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args("*", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_mul(arg1, arg2));   
    return new_node(T_INT, res);
}

static int _buildin_div(int x, int y) {
    return x / y;
}

Node *buildin_div(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args("/", args, 2, types);
    int arg1 = argtoi(args, 0);
    int arg2 = argtoi(args, 1);
    char *res = itos(_buildin_div(arg1, arg2));   
    return new_node(T_INT, res);
}

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

static char *list_to_string(List *items) {
    int repr_bytes = 3;
    char *repr = malloc(sizeof(char) * repr_bytes);
    strcat(repr, "(");

    for (int i = 0; i < items->length; i++) {
        Node *node = list_get(items, i);
        char *val = node->val;

        if (node->type == T_STR) {
            repr_bytes += strlen(val) + 4;
            repr = realloc(repr, repr_bytes);
            strcat(repr, "\"");
            strcat(repr, val);
            strcat(repr, "\"");
        } else {
            repr_bytes += strlen(val) + 2;
            repr = realloc(repr, repr_bytes);
            strcat(repr, val);
        }

        if (i < (items->length - 1)) {
            strcat(repr, ", ");
        }
    }
    
    strcat(repr, ")");
    return repr;
}

Node *buildin_list(List *args) {
    args = resolve_all(args);
    Node* list = new_node(T_LST, list_to_string(args));
    list->children = args;
    return list;
}

Node *buildin_head(List *args) {
    args = resolve_all(args);
    int types[] = { T_LST | T_STR };
    assert_args("head", args, 1, types);
    Node *list = list_get(args, 0);
    
    if (list->type == T_LST) {
        Node *item = list_get(list->children, 0);
        return item ? item : new_node(T_UND, "");
    } 
    else { // T_STR
        if (strlen(list->val) == 0) {
            return new_node(T_UND, "");
        } else {
            char* val = malloc(sizeof(char) * 2);
            strncpy(val, list->val, 1);
            return new_node(T_STR, val);    
        }
    }
}

Node *buildin_tail(List *args) {
    args = resolve_all(args);
    int types[] = { T_LST | T_STR };
    assert_args("tail", args, 1, types);
    Node *list = list_get(args, 0);

    if (list->type == T_LST) {
        List *items = list->children;
        List *tail_items = list_create();

        for (int i = 1; i < items->length; i++) {
            list_push(tail_items, list_get(items, i));
        }

        Node *new_list = new_node(T_LST, list_to_string(tail_items));
        new_list->children = tail_items;
        return new_list;
    }
    else { // T_STR
        int len = strlen(list->val);
        if (len < 2) {
            return new_node(T_STR, "");
        } else {
            char* tail_string = malloc(sizeof(char) * len);
            strcpy(tail_string, ++list->val);
            return new_node(T_STR, tail_string);
        }
    }
}

Node *buildin_init(List *args) {
    args = resolve_all(args);
    int types[] = { T_LST | T_STR };
    assert_args("init", args, 1, types);
    Node *list = list_get(args, 0);

    if (list->type == T_LST) {
        List *items = list->children;
        List *init_items = list_create();

        for (int i = 0; i < items->length - 1; i++) {
            list_push(init_items, list_get(items, i));
        }

        Node *new_list = new_node(T_LST, list_to_string(init_items));
        new_list->children = init_items;
        return new_list;
    }
    else {
        int len = strlen(list->val);
        if (len < 2) {
            return new_node(T_STR, "");
        } else {
            char* init_string = malloc(sizeof(char) * len);
            strncpy(init_string, list->val, len - 1);
            init_string[len - 1] = '\0';
            return new_node(T_STR, init_string);
        }
    }
}

Node *buildin_last(List *args) {
    args = resolve_all(args);
    int types[] = { T_LST | T_STR };
    assert_args("last", args, 1, types);
    Node *list = list_get(args, 0);


    if (list->type == T_LST) {
        Node *item = list_last(list->children);
        return item ? item : new_node(T_UND, "");
    }
    else { // T_STR
        if (strlen(list->val) == 0) {
            return new_node(T_UND, "");
        } else {
            char* val = malloc(sizeof(char) * 2);
            int off = strlen(list->val) - 1;
            strncpy(val, list->val + off, 1);
            return new_node(T_STR, val);    
        }
    }
}

Node *buildin_empty(List *args) {
    args = resolve_all(args);
    int types[] = { T_LST | T_STR };
    assert_args("empty", args, 1, types);
    Node *list = list_get(args, 0);

    if (list->type == T_LST) {
        return new_node(T_INT,
            list->children->length == 0 ? "1" : "0");
    }
    else { // T_STR
        return new_node(T_INT,
            strlen(list->val) == 0 ? "1": "0");
    }
}

// 0: =
// 1: <
// 2: >
// 3: <=
// 4: >=
static Node *compare(int op, List *args) {
    args = resolve_all(args);
    int types[] = {
        T_INT | T_STR,
        T_INT | T_STR,
    };
    assert_args("compare", args, 2, types);
    Node *left = list_get(args, 0);
    Node *right = list_get(args, 1);
    int res;

    if (left->type != right->type) {
        printf(">: can only compare values of same type\n");
        exit(1);
    }

    if (left->type == T_INT) {
        int left_val = atoi(left->val);
        int right_val = atoi(right->val);

        switch (op) {
            case 0: res = left_val == right_val; break;
            case 1: res = left_val < right_val; break;
            case 2: res = left_val > right_val; break;
            case 3: res = left_val <= right_val; break;
            case 4: res = left_val >= right_val; break;

        }
    }
    else { // T_STR
        switch (op) {
            case 0: res = strcmp(left->val, right->val) == 0; break;
            case 1: res = strcmp(left->val, right->val) < 0; break;
            case 2: res = strcmp(left->val, right->val) > 0; break;
            case 3: res = strcmp(left->val, right->val) <= 0; break;
            case 4: res = strcmp(left->val, right->val) >= 0; break;
        }
    }

    return res
        ? new_node(T_INT, "1")
        : new_node(T_INT, "0");
}

Node *buildin_eq(List *args) {
    return compare(0, args);
}

Node *buildin_lt(List *args) {
    return compare(1, args);
}

Node *buildin_gt(List *args) {
    return compare(2, args);
}

Node *buildin_le(List *args) {
    return compare(3, args);
}

Node *buildin_ge(List *args) {
    return compare(4, args);
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

Node *buildin_cons(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT | T_STR | T_LST , T_LST | T_STR };
    assert_args("cons", args, 2, types);
    Node *left = list_get(args, 0);
    Node *right = list_get(args, 1);
    
    if (right->type == T_LST) {
        List *items = list_create();
        list_push(items, left);

        for (int i = 0; i < right->children->length; i++) {
            list_push(items, list_get(right->children, i));
        }

        Node *list = new_node(T_LST, list_to_string(items));
        list->children = items;
        return list;
    }
    else { // right->type == T_STR
        if (left->type != T_STR) {
            printf("cons: left must be string if right is a string, got:%d\n", left->type);
            exit(1);
        }

        char* string = malloc(sizeof(char) * (strlen(left->val) + strlen(right->val) + 1));
        strcat(string, left->val);
        strcat(string, right->val);
        return new_node(T_STR, string);
    }
}

Node *buildin_println(List *args) {
    args = resolve_all(args);
    int types[] = { T_STR | T_INT | T_LST | T_APPL | T_UND };
    assert_args("println", args, 1, types);
    Node *node = list_get(args, 0);
    printf("%s\n", node->val);
    return node;
}

Node *buildin_printstack(List *args) {
    args = resolve_all(args);
    int types[] = { T_STR | T_INT | T_LST | T_APPL | T_UND };
    assert_args("printstack", args, 1, types);
    Node *node = list_get(args, 0);
    print_stack();
    return node;
}

Node *buildin_isint(List *args) {
    args = resolve_all(args);
    assert_args_len("int?", args, 1);
    Node *node = list_get(args, 0);
    return new_node(T_INT, node->type == T_INT ? "1" : "0");   
}

Node *buildin_isstr(List *args) {
    args = resolve_all(args);
    assert_args_len("str?", args, 1);
    Node *node = list_get(args, 0);
    return new_node(T_INT, node->type == T_STR ? "1" : "0");   
}

Node *buildin_islist(List *args) {
    args = resolve_all(args);
    assert_args_len("list?", args, 1);
    Node *node = list_get(args, 0);
    return new_node(T_INT, node->type == T_LST ? "1" : "0");   
}

Node *buildin_isundefined(List *args) {
    args = resolve_all(args);
    assert_args_len("undefined?", args, 1);
    Node *node = list_get(args, 0);
    return new_node(T_INT, node->type == T_UND ? "1" : "0");   
}

Node *buildin_mod(List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args("mod", args, 2, types);
    Node *left = list_get(args, 0);
    Node *right = list_get(args, 1);
    return new_node(T_INT, itos(atoi(left->val) % atoi(right->val)));
}
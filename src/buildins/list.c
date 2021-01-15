#include "buildins.h"


static char *list_to_string(List *items) {
    Node *node;
    
    long size = items->length * 2 + 1;
    for (int i = 0; i < items->length; i++) {
        node = list_get(items, i);
        size += strlen(node->val);
        if (node->type == T_STR) {
            size += 2;
        }
    }

    char *repr = malloc(sizeof(char) * size);
    strcpy(repr, "(");    


    for (int i = 0; i < items->length; i++) {
        Node *node = list_get(items, i);
        char *val = node->val;

        if (node->type == T_STR) {
            strcat(repr, "\"");
            strcat(repr, val);
            strcat(repr, "\"");
        } else {
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
        return item ? item : new_node(T_NONE, "<None>");
    } 
    else { // T_STR
        if (strlen(list->val) == 0) {
            return new_node(T_NONE, "<None>");
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
        return item ? item : new_node(T_NONE, "<None>");
    }
    else { // T_STR
        if (strlen(list->val) == 0) {
            return new_node(T_NONE, "<None>");
        } else {
            char* val = malloc(sizeof(char) * 2);
            int off = strlen(list->val) - 1;
            strncpy(val, list->val + off, 1);
            return new_node(T_STR, val);    
        }
    }
}

Node *buildin_isempty(List *args) {
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

Node *buildin_cons(List *args) {
    args = resolve_all(args);
    int types[] = {
        T_INT | T_STR | T_LST | T_NONE | T_FUN,
        T_LST | T_STR
    };
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
            printf("function cons: left side must be of type string if the right side is a string, got: %s\n",
                type_to_string(left->type));
            exit(1);
        }

        char* string = malloc(sizeof(char) * (strlen(left->val) + strlen(right->val) + 1));
        strcpy(string, left->val);
        strcat(string, right->val);
        return new_node(T_STR, string);
    }
}

Node *buildin_append(List *args) {
    args = resolve_all(args);
    int types[] = {
        T_LST | T_STR,
        T_INT | T_STR | T_LST | T_NONE | T_FUN
    };
    assert_args("append", args, 2, types);
    Node *left = list_get(args, 0);
    Node *right = list_get(args, 1);
    
    if (left->type == T_LST) {
        List *items = list_create();

        for (int i = 0; i < left->children->length; i++) {
            list_push(items, list_get(left->children, i));
        }

        list_push(items, right);
        Node *list = new_node(T_LST, list_to_string(items));
        list->children = items;
        return list;
    }
    else { // left->type == T_STR
        if (right->type != T_STR) {
            printf("function append: right side must be of type string if the left side is a string, got: %s\n",
                type_to_string(left->type));
            exit(1);
        }

        char* string = malloc(sizeof(char) * (strlen(left->val) + strlen(right->val) + 1));
        strcpy(string, left->val);
        strcat(string, right->val);
        return new_node(T_STR, string);
    }
}
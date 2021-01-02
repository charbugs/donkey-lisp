#include "buildins.h"

static Node *istype(int type, List *args) {
    args = resolve_all(args);
    assert_args_len("istype?", args, 1);
    Node *node = list_get(args, 0);
    return new_node(T_INT, node->type == type ? "1" : "0");   
}

Node *buildin_isint(List *args) {
    return istype(T_INT, args);
}

Node *buildin_isstr(List *args) {
    return istype(T_STR, args);
}

Node *buildin_islist(List *args) {
    return istype(T_LST, args);
}

Node *buildin_isundefined(List *args) {
    return istype(T_UND, args);
}
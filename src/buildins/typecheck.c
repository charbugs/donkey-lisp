#include "buildins.h"

static Node *istype(char *fname, int type, List *args) {
    args = resolve_all(args);
    assert_args_len(fname, args, 1);
    Node *node = list_get(args, 0);
    return new_node(T_INT, node->type == type ? "1" : "0");   
}

Node *buildin_isint(List *args) {
    return istype("int?", T_INT, args);
}

Node *buildin_isstr(List *args) {
    return istype("str?", T_STR, args);
}

Node *buildin_islist(List *args) {
    return istype("list?", T_LST, args);
}

Node *buildin_isundefined(List *args) {
    return istype("undefined?", T_UND, args);
}

Node *buildin_isfunction(List *args) {
    return istype("function?", T_FUN, args);
}
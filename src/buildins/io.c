#include "buildins.h"


Node *buildin_print(List *args) {
    args = resolve_all(args);
    Node *cur;
    int type = T_STR | T_INT | T_LST | T_APPL | T_NONE | T_FUN;
    
    for (int i = 0; i < args->length; i++) {
        cur = list_get(args, i);
        assert_arg_type("print", cur, i, type);
        printf("%s", cur->val);
    }

    cur = list_get(args, 0);
    return cur ? cur : new_node(T_NONE, "<None>");
}
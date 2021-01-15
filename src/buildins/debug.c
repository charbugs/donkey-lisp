#include "buildins.h"


Node *buildin_printstack(List *args) {
    args = resolve_all(args);
    int types[] = { T_STR | T_INT | T_LST | T_APPL | T_NONE | T_FUN };
    assert_args("printstack", args, 1, types);
    Node *node = list_get(args, 0);
    print_stack();
    return node;
}
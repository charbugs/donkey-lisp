#include "buildins.h"


Node *buildin_print(List *args) {
    args = resolve_all(args);
    int types[] = { T_STR | T_INT | T_LST | T_APPL | T_UND | T_FUN };
    assert_args("println", args, 1, types);
    Node *node = list_get(args, 0);
    printf("%s\n", node->val);
    return node;
}
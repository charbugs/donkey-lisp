#include "buildins.h"


static int argtoi(List *args, int pos) {
    return atoi(((Node*)list_get(args, pos))->val);
}

static char* itos(int i) {
    char *s = malloc(sizeof(char) * 20);
    sprintf(s, "%d", i);
    return s;
}

// 0: +
// 1: -
// 2: *
// 3: /
// 4: %
static Node *calc(char* fname, int op, List *args) {
    args = resolve_all(args);
    int types[] = { T_INT, T_INT };
    assert_args(fname, args, 2, types);
    int val1 = argtoi(args, 0);
    int val2 = argtoi(args, 1);
    int res;

    switch (op) {
        case 0: res = val1 + val2; break;
        case 1: res = val1 - val2; break;
        case 2: res = val1 * val2; break;
        case 3: res = val1 / val2; break;
        case 4: res = val1 % val2; break;
    }

    return new_node(T_INT, itos(res));
}

Node *buildin_add(List *args) {
    return calc("+", 0, args);
}

Node *buildin_sub(List *args) {
    return calc("-", 1, args);
}

Node *buildin_mul(List *args) {
    return calc("*", 2, args);
}

Node *buildin_div(List *args) {
    return calc("/", 3, args);
}

Node *buildin_mod(List *args) {
    return calc("mod", 4, args);
}
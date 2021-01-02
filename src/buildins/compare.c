#include "buildins.h"


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
        printf("compare: can only compare values of same type\n");
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
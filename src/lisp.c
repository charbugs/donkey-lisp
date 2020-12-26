#include <stdio.h>
#include "tokenize.h"
#include "parse.h"
#include "exec.h"
#include "stack.h"

#define MAX_TEXT_LEN 10000

int get_text(char *buf, int max, FILE *f) {
    char c; int i;
    for (i = 0; (i < max - 1) && ((c = getc(f)) != EOF); i++) {
        buf[i] = c;
    }
    buf[i] = '\0';
    return i - 1;
}

void print_result(Node *root) {
    Node *child;
    for (int i = 0; i < root->children->length; i++) {
        child = list_get(root->children, i);
        printf("%s\n", child->val);
    }
}

int main(int argc, char *argv[]) {
    FILE *f;
    char text[MAX_TEXT_LEN];

    if (argc > 1) {
        f = fopen(argv[1], "r");
        if (!f) {
            printf("could not open file %s\n", argv[1]);
            return 0;
        }
    } else {
        f = stdin;
    }


    get_text(text, MAX_TEXT_LEN, f);
    List *tokens = tokenize(text);
    Node *root = parse(tokens);
    print_ast(root);
    Node *executed_root = exec(root);
    print_stack();
    print_result(executed_root);

    return 0;
}
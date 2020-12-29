#include <stdio.h>
#include <string.h>
#include "tokenize.h"
#include "parse.h"
#include "resolve.h"
#include "stack.h"

#define MAX_TEXT_LEN 100000


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
        if (child->type != T_UND) {
            printf("%s\n", child->val);
        }
    }
}

int main(int argc, char *argv[]) {
    int arg_print_stack = 0;
    int arg_print_ast = 0;
    char *arg_fname = NULL;
    char text[MAX_TEXT_LEN];
    FILE *f;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-s") == 0) {
            arg_print_stack = 1;
        }
        else if (strcmp(argv[i], "-a") == 0) {
            arg_print_ast = 1;
        }
        else {
            arg_fname = argv[i];
        }
    }

    if (arg_fname) {
        f = fopen(argv[1], "r");
        if (!f) {
            printf("could not open file %s\n", arg_fname);
            return 0;
        }
    } else {
        f = stdin;
    }

    get_text(text, MAX_TEXT_LEN, f);
    List *tokens = tokenize(text);
    Node *root = parse(tokens);
    
    if (arg_print_ast) {
        printf("======== AST ========\n");
        print_ast(root);
        printf("\n");
    }
    
    Node *resolved_root = resolve(root);
    
    if (arg_print_stack) {
        printf("======== STACK ========\n");
        print_stack();
        printf("\n");
    }
    
    print_result(resolved_root);

    return 0;
}
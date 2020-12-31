#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tokenize.h"
#include "parse.h"
#include "resolve.h"
#include "stack.h"

#define MAX_FILE_SIZE 100000


int read_file(char *buf, int max, FILE *f) {
    int i = 0,
        in_comment = 0;
    char c; 

    while ((i < max) && ((c = getc(f)) != EOF)) {
        if (in_comment) {
            if (c == '\n') {
                buf[i++] = c;
                in_comment = 0;
            }
        } else { 
            if (c == ';') {
                in_comment = 1;       
            } else {
                buf[i++] = c;
            }

        }
    }

    if (i >= max) {
        printf("reached file size limit\n");
        exit(1);
    }

    return i;
}

char *get_text(char *include[]) {
    int i = 0, text_size = 0; 
    char *text = NULL, *cur, *fname;
    FILE *f;

    while ((fname = include[i++]) != NULL) {
        f = fopen(fname, "r");
        
        if (!f) {
            printf("can not open file %s\n", fname);
            exit(1);
        }

        text = realloc(text, text_size + MAX_FILE_SIZE + 1);
        cur = text + text_size;
        text_size += read_file(cur, MAX_FILE_SIZE, f);
        fclose(f);
    }

    if (text) {
        text[text_size] = '\0';
    }

    return text;
}

int main(int argc, char *argv[]) {
    int arg_print_ast = 0;
    char *arg_fname = NULL;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            arg_print_ast = 1;
        }
        else {
            arg_fname = argv[i];
        }
    }

    if (!arg_fname) {
        printf("usage: lisp [-a] FILE\n");
        exit(1);
    }

    char *include[] = {
        "lib/stdlib.dl",
        arg_fname,
        NULL // terminate list
    };

    char *text = get_text(include);
    List *tokens = tokenize(text);
    Node *root = parse(tokens);

    if (arg_print_ast) {
        printf("======== AST ========\n");
        print_ast(root);
        printf("\n");
    }
    
    resolve(root);
    return 0;
}
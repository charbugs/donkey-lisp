#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tokenize.h"
#include "parse.h"
#include "resolve.h"
#include "read.h"


int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("usage: donkey FILE [FILE ...]\n");
        exit(1);
    }

    char *text = get_text(++argv, argc - 1);
    List *tokens = tokenize(text);
    Node *root = parse(tokens);
    resolve(root);
    return 0;
}
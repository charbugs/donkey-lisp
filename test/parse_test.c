#include <stdio.h>
#include "parse.h"
#include "tokenize.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("need a line of source code as first argument\n");
        return 1;
    }

    char *source = argv[1];

    List *tokens = tokenize(source);
    Root *root = parse(tokens);
    print_ast((Node*)root);

    return 0;
} 
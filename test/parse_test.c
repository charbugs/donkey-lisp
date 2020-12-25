#include <stdio.h>
#include "../src/parse.h"
#include "../src/tokenize.h"


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("need a line of source code as first argument\n");
        return 1;
    }

    char *source = argv[1];

    List *tokens = tokenize(source);
    Node *root = parse(tokens);
    print_ast(root);

    return 0;
} 
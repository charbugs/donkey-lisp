#include <stdio.h>
#include "tokenize.h"


int main(int argc, char* argv[]) {
    char *text = argv[1];
    List *tokens = tokenize(text);
    Token *token;

    for (int i = 0; i < tokens->length; i++) {
        token = list_get(tokens, i);
        if (token->type == TOK_INT) {
            printf("%d: %d\n", token->type, token->int_val);
        } else {
            printf("%d: %s\n", token->type, token->str_val);
        }
    }
}
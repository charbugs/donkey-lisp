#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "tokenize.h"

static int is_valid_token_neighbor(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == ')' || c == '(' || c == '\0';
}

static int is_valid_identifier_start(char c) {
    return isalpha(c) ||
        c == '_' || c == '?' ||
        c == '+' || c == '-' || c == '*' || c == '/' ||
        c == '=' || c == '<' || c == '>';
}

static int is_valid_identifier_rest(char c) {
    return is_valid_identifier_start(c) || isdigit(c);
}

static char *substring(char *s, int n) {
    char *sub = malloc(sizeof(char) * n + 1);
    strncpy(sub, s, n);
    sub[n] = '\0';
    return sub;
}

List *tokenize(char* text) {
    List *tokens = list_create();
    Token *token;

    while (*text != '\0') {
        // Opening bracket token
        if (*text == '(') {
            token = malloc(sizeof(Token));
            token->type = TOK_OPN;
            token->val = "(";
            list_push(tokens, token);
            text++;
        }
        // Closing bracket token
        else if (*text == ')') {
            token = malloc(sizeof(Token));
            token->type = TOK_CLS;
            token->val = ")";
            list_push(tokens, token);
            text++;
        }
        // Identifier token:
        else if (is_valid_identifier_start(*text)) {
            char *start = text;
            
            while (is_valid_identifier_rest(*text))
                text++;
            
            char *form = substring(start, text - start);

            if (!is_valid_token_neighbor(*text)) {
                printf("tokenizer: malformed token: %s%c\n", form, *text);
                exit(1);
            }

            token = malloc(sizeof(Token));
            token->type = TOK_IDF;
            token->val = form;
            list_push(tokens, token);
        }
        // String token:
        // Any character between "" is considered a string.
        else if (*text == '"') {
            char *start = (text + 1);
            
            while(*(++text) != '"')
                ;
            
            char *form = substring(start, text - start);

            text++;
            if (!is_valid_token_neighbor(*text)) {
                printf("tokenizer: malformed token: \"%s\"%c\n", form, *text);
                exit(1);
            }

            token = malloc(sizeof(Token));
            token->type = TOK_STR;
            token->val = form;
            list_push(tokens, token);
        } 
        else if (isdigit(*text)) {
            char *start = text;

            while(isdigit(*(++text)))
                ;

            char *form = substring(start, text - start);

            if (!is_valid_token_neighbor(*text)) {
                printf("tokenizer: malformed token: %s%c\n", form, *text);
                exit(1);
            }

            token = malloc(sizeof(Token));
            token->type = TOK_INT;
            token->val = form;
            list_push(tokens, token);
        }
        else if (is_valid_token_neighbor(*text)) {
            text++;
        }
        else {
            printf("tokenizer: non valid character: %c\n", *text);
            exit(1);
        }
    }

    return tokens;
}
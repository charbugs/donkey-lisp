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

static char esc_seq_to_ascii(char c) {
    switch (c) {
        case 'n': return '\n'; break;
        case 't': return '\t'; break;
        case '\\': return '\\'; break;
        default: return c; break;
    }
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
        else if (isdigit(*text) || (*text == '-' && isdigit(*(text + 1)))) {
            char *start = text;

            while(isdigit(*(++text)))
                ;

            char *form = substring(start, text - start);

            if (!is_valid_token_neighbor(*text)) {
                printf("tokenize: malformed token: %s%c\n", form, *text);
                exit(1);
            }

            token = malloc(sizeof(Token));
            token->type = TOK_INT;
            token->val = form;
            list_push(tokens, token);
        }
        // Identifier token:
        else if (is_valid_identifier_start(*text)) {
            char *start = text;
            
            while (is_valid_identifier_rest(*text))
                text++;
            
            char *form = substring(start, text - start);

            if (!is_valid_token_neighbor(*text)) {
                printf("tokenize: malformed token: %s%c\n", form, *text);
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
            int esc_len = 0; // length of escape sequences
            
            // Forward the text until we get a closing quote
            // and count the escape sequences by counting the backslashes.
            // But dont count the backslash if the previous char is a backslash.
            while(*(++text) != '"') {
                if (*text == '\\' && *(text - 1) != '\\') {
                    esc_len++;
                }
            }

            // The string that we are going to extract will be as long
            // as the characters between the quotes minus the backslashes
            // plus one for the terminating null byte.
            char *form = malloc(sizeof(char) * (text - start - esc_len + 1));
            char *form_ptr = form;
            int esc_on = 0;

            // Copy the text chunk into the form string.
            // If there is a backslash then skip copying the backslash
            // and interpret the next character as an escape character.
            for (int i = 0; i < text - start; i++) {
                if (start[i] == '\\' && start[i - 1] != '\\') {
                    esc_on = 1;
                }
                else if (esc_on) {
                    *(form_ptr++) = esc_seq_to_ascii(start[i]);
                    esc_on = 0;
                }
                else {
                    *(form_ptr++) = start[i];
                }
            }

            *form_ptr = '\0';

            text++;
            if (!is_valid_token_neighbor(*text)) {
                printf("tokenize: malformed token: \"%s\"%c\n", form, *text);
                exit(1);
            }

            token = malloc(sizeof(Token));
            token->type = TOK_STR;
            token->val = form;
            list_push(tokens, token);
        } 
        else if (is_valid_token_neighbor(*text)) {
            text++;
        }
        else {
            printf("tokenize: non valid character: %c\n", *text);
            exit(1);
        }
    }

    return tokens;
}
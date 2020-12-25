#ifndef TOKENIZE_H
#define TOKENIZE_H

#include "list.h"

#define TOK_OPN 0
#define TOK_CLS 1
#define TOK_FUN 2
#define TOK_INT 3
#define TOK_STR 4


typedef struct Token {
    int type;
    union {
        int int_val;
        char* str_val;
    };
} Token;


List *tokenize(char *source);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "log.h"

#define MAX_LINE 1000
#define MAX_TOKENS 100

#define EXP 0
#define IDF 1
#define STR 2
#define NUM 3
#define OPN 4
#define CLS 5

typedef struct node_t {
  struct node_t *next;
  struct node_t *first;
  int type;
  char *value;
} node_t;

node_t *new_node(int type, char *value) {
  node_t *new = malloc(sizeof(node_t));
  new->next = NULL;
  new->first = NULL;
  new->type = type;
  new->value = value;
  return new;
}


int get_line(char **lineptr) {
    size_t max = MAX_LINE;
    *lineptr = malloc(sizeof(char *) * max);
    return getline(lineptr, &max, stdin);
}

int tokenize(char *line, node_t *tokens, int max) {
    char *pattern = "\\(|\\)|[0-9]+|[a-z-]+|\"[a-zA-Z0-9?!\\.-\\(\\) ]*\"";
    regex_t re;
    regmatch_t match;
    char *token;
    int toklen, ntok = 0;

    if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
        logs("failed to compile regex");
        exit(1);
    }

    while (regexec(&re, line, 1, &match, 0) == 0 && ntok < max) {
        toklen = match.rm_eo - match.rm_so;
        token = malloc(sizeof(char) * toklen + 1);
        strncpy(token, line + match.rm_so, toklen);
        token[toklen] = '\0';
        tokens[ntok].value = token;
        line = line + match.rm_eo;
        ntok++;
    }

    return ntok;
}

int is_string_value(char *value) {
    return value[0] == '"' && value[strlen(value) - 1] == '"';
}

int is_number_value(char *value) {
    char *p = value;
    while (*p != '\0') {
        if (!isdigit(*p++))
            return 0;
    }
    return 1;
}

void lex(node_t *tokens, int len) {
    for (int i = 0; i < len; i++) {
        if (is_string_value(tokens[i].value))
            tokens[i].type = STR;
        else if (is_number_value(tokens[i].value))
            tokens[i].type = NUM;
        else if (*tokens[i].value == '(')
            tokens[i].type = OPN;
        else if (*tokens[i].value == ')')
            tokens[i].type = CLS;
        else 
            tokens[i].type = IDF;
    }
}

int main() {
    char *line;
    int ntoks;
    node_t *tokens = malloc(sizeof(node_t) * MAX_TOKENS);

    while (get_line(&line) != EOF) {
        ntoks = tokenize(line, tokens, MAX_TOKENS);
        lex(tokens, ntoks);
        for (int i = 0; i < ntoks; i++) {
            printf("%d: %s\n", tokens[i].type, tokens[i].value);
        }
    }

    return 0;
}
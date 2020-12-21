#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>

#define MAX_LINE 1000
#define MAX_TERMINALS 100

#define UND 0
#define EXP 1
#define IDF 2
#define STR 3
#define NUM 4
#define OPN 5
#define CLS 6

char *type_to_str[7] = { 
    "UND", "EXP", "IDF", "STR", "NUM", "OPN", "CLS"
};

typedef struct node_t {
  struct node_t *next;
  struct node_t *children;
  int type;
  char *value;
} node_t;

node_t *new_node() {
  node_t *new = malloc(sizeof(node_t));
  new->next = NULL;
  new->children = NULL;
  new->type = UND;
  new->value = NULL;
  return new;
}


int get_line(char **lineptr) {
    size_t max = MAX_LINE;
    *lineptr = malloc(sizeof(char *) * max);
    return getline(lineptr, &max, stdin);
}

int tokenize(char *line, node_t *terminals[], int max) {
    char *pattern = "\\(|\\)|[0-9]+|[a-z-]+|\"[a-zA-Z0-9?!\\.-\\(\\) ]*\"";
    regex_t re;
    regmatch_t match;
    char *token;
    int toklen, nterms = 0;

    if (regcomp(&re, pattern, REG_EXTENDED) != 0) {
        printf("failed to compile regex\n");
        exit(1);
    }

    while (regexec(&re, line, 1, &match, 0) == 0 && nterms < max) {
        toklen = match.rm_eo - match.rm_so;
        token = malloc(sizeof(char) * toklen + 1);
        strncpy(token, line + match.rm_so, toklen);
        token[toklen] = '\0';
        terminals[nterms]->value = token;
        line = line + match.rm_eo;
        nterms++;
    }

    return nterms;
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

void lex(node_t *terminals[], int len) {
    node_t *term;
    for (int i = 0; i < len; i++) {
        term = terminals[i];
        if (is_string_value(term->value))
            term->type = STR;
        else if (is_number_value(term->value))
            term->type = NUM;
        else if (*term->value == '(')
            term->type = OPN;
        else if (*term->value == ')')
            term->type = CLS;
        else 
            term->type = IDF;
    }
}

void print_tree(node_t *node, int level) {
    for (int i = 0; i < level; i++)
        printf("   ");
    
    printf("%s: %s\n", type_to_str[node->type], node->value);
    
    node_t *curr_child = node->children;
    while (curr_child != NULL) {
        print_tree(curr_child, level + 1);
        curr_child = curr_child->next;
    }
}

node_t *get_last_child(node_t *node) {
    node_t *child = node->children;
    if (!child)
        return NULL;
    while (child->next != NULL)
        child = child->next;
    return child;
}

node_t *parse(node_t *terminals[], int nterms) {
    node_t *curr_working_node = NULL;
    node_t *prev_working_node = NULL;
    node_t *last_child = NULL;

    for (int i = 0; i < nterms; i++) {
        switch (terminals[i]->type) {
            case OPN:
                prev_working_node = curr_working_node;
                curr_working_node = new_node();
                curr_working_node->type = EXP;
                if (prev_working_node) {
                    if ((last_child = get_last_child(prev_working_node)))
                        last_child->next = curr_working_node;
                    else
                        prev_working_node->children = curr_working_node;
                }
                break;
            case CLS:
                if (prev_working_node)
                    curr_working_node = prev_working_node;
                break;
            case IDF:
            case STR:
            case NUM:
                if ((last_child = get_last_child(curr_working_node)))
                    last_child->next = terminals[i];
                else
                    curr_working_node->children = terminals[i];
                break;
        }
    }
    return curr_working_node;
}

int main() {
    char *line;
    int nterms;
    node_t *terminals[MAX_TERMINALS];
    node_t *ast;

    while (get_line(&line) != EOF) {
        for (int i = 0; i < MAX_TERMINALS; i++) {
            terminals[i] = new_node();
        }
        nterms = tokenize(line, terminals, MAX_TERMINALS);
        lex(terminals, nterms);
        ast = parse(terminals, nterms);
        print_tree(ast, 0);
    }

    return 0;
}
#include <stdlib.h>
#include <stdio.h>
#include "parse.h"


Node *new_node(int type, char *val) {
    Node *node = malloc(sizeof(Node));
    node->children = list_create();
    node->type = type;
    node->val = val;
    return node;
}

static int is_wrapped_by_brackets(List *tokens) {
    Token *first = list_get(tokens, 0);
    Token *last = list_last(tokens);
    return first->type == TOK_OPN && last->type == TOK_CLS; 
}

static int has_matching_brackets(List *tokens) {
    Token *token;
    int count = 0;
    for (int i = 0; i < tokens->length; i++) {
        token = list_get(tokens, i);
        if (token->type == TOK_OPN)
            count++;
        if (token->type == TOK_CLS)
            count--;
    }
    return count == 0;
}

static int each_opening_bracket_is_followed_by_identifier(List *tokens) {
    Token *left, *right;
    for (int i = 0; i < tokens->length - 1; i++) {
        left = list_get(tokens, i);
        right = list_get(tokens, i + 1);
        if (left->type == TOK_OPN && right->type != TOK_IDF) {
            return 0;
        }
    }
    return 1;
}

static void check_token_syntax(List *tokens) {
    if (!is_wrapped_by_brackets(tokens)) {
        printf("parser: start or end bracket missing\n");
        exit(1);
    }
    if (!has_matching_brackets(tokens)) {
        printf("parser: opening and closing brackets do not match\n");
        exit(1);
    }
    if (!each_opening_bracket_is_followed_by_identifier(tokens)) {
        printf("parser: each opening bracket must be followed by an identifier\n");
        exit(1);
    }
}

// List<Token> *tokens
Node *parse(List *tokens) {
    
    check_token_syntax(tokens);

    // List<Node> *stack
    List *stack = list_create(); 
    Node *cur = new_node(T_ROOT, "root");

    for (int i = 0; i < tokens->length; i++) {

        Token *token = list_get(tokens, i);

        if (token->type == TOK_OPN) {
            // Get the identifier token that follows the opening token
            // and interpret it's value as a function name.
            Token *tok_idf = list_get(tokens, ++i);
            Node *appl = new_node(T_APPL, tok_idf->val);
            list_push(cur->children, appl);
            list_push(stack, cur);
            cur = appl;
        }
        else if (token->type == TOK_CLS) {
            cur = list_pop(stack);

        }
        else if (token->type == TOK_IDF) {
            list_push(cur->children, new_node(T_IDF, token->val));
        } 
        else if (token->type == TOK_STR) {
            list_push(cur->children, new_node(T_STR, token->val));
        }
        else if (token->type == TOK_INT) {
            list_push(cur->children, new_node(T_INT, token->val));
        }
    }

    return cur;
}

static void _print_ast(Node* node, int level) {
    for (int i = 0; i < level; i++) {
        printf("   ");
    }

    printf("%s: %s\n", type_to_string(node->type), node->val);
   
    for (int i = 0; i < node->children->length; i++) {
        Node *child = list_get(node->children, i);
        _print_ast(child, level + 1);
    }
}

void print_ast(Node *node) {
 _print_ast(node, 0);   
}

char* type_to_string(int type) {
    switch(type) {
        case 1: return "Root";
        case 2: return "Int";
        case 4: return "Str";
        case 8: return "Ident";
        case 16: return "Appl";
        case 32: return "List";
        case 64: return "Undefined";
        default: return "Unknown";
    }
}
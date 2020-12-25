#include <stdlib.h>
#include <stdio.h>
#include "parse.h"


Root *new_root(char *label) {
    Root *root = malloc(sizeof(Root));
    root->node.type = NODE_ROOT;
    root->node.children = list_create();
    root->label = label;
    return root;
}

Appl *new_appl(char *func) {
    Appl *appl = malloc(sizeof(Appl));
    appl->node.type = NODE_APPL;
    appl->node.children = list_create();
    appl->func = func;
    return appl;
}

Str *new_str(char *val) {
    Str *str = malloc(sizeof(Str));
    str->node.type = NODE_STR;
    str->node.children = list_create();
    str->val = val;
    return str;
}

Int *new_int(int val) {
    Int *_int = malloc(sizeof(Int));
    _int->node.type = NODE_INT;
    _int->node.children = list_create();
    _int->val = val;
    return _int;
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

static int each_opening_bracket_is_followed_by_function_token(List *tokens) {
    Token *left, *right;
    for (int i = 0; i < tokens->length - 1; i++) {
        left = list_get(tokens, i);
        right = list_get(tokens, i + 1);
        if (left->type == TOK_OPN && right->type != TOK_FUN) {
            return 0;
        }
    }
    return 1;
}

static int function_name_can_only_appear_after_opening_bracket(List *tokens) {
    Token *left, *right;
    for (int i = 0; i < tokens->length - 1; i++) {
        left = list_get(tokens, i);
        right = list_get(tokens, i + 1);
        if (right->type == TOK_FUN && left->type != TOK_OPN) {
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
    if (!each_opening_bracket_is_followed_by_function_token(tokens)) {
        printf("parser: each opening bracket must be followed by a function name\n");
        exit(1);
    }
    if (!function_name_can_only_appear_after_opening_bracket(tokens)) {
        printf("parser: function names must be placed directly after opening bracket\n");
        exit(1);
    }
}

// List<Token> *tokens
Root *parse(List *tokens) {
    
    check_token_syntax(tokens);

    // List<Node> *stack
    List *stack = list_create(); 
    Node *cur = (Node*)new_root("root");

    for (int i = 0; i < tokens->length; i++) {

        Token *token = list_get(tokens, i);

        if (token->type == TOK_OPN) {
            // Get the function name token that follows the opening token.
            Token *tok_fun = list_get(tokens, ++i);
            Appl *appl = new_appl(tok_fun->str_val);
            list_push(cur->children, appl);
            list_push(stack, cur);
            cur = (Node*)appl;
        }
        else if (token->type == TOK_CLS) {
            cur = list_pop(stack);

        } 
        else if (token->type == TOK_STR) {
            list_push(cur->children, new_str(token->str_val));
        }
        else if (token->type == TOK_INT) {
            list_push(cur->children, new_int(token->int_val));
        }
    }

    return (Root*)cur;
}

static void _print_ast(Node* node, int level) {

    for (int i = 0; i < level; i++) {
        printf("   ");
    }

    if (node->type == NODE_ROOT) {
        printf("Root: %s\n", ((Root*)node)->label);
    }
    else if (node->type == NODE_APPL) {
        printf("Appl: %s\n", ((Appl*)node)->func);
    }
    else if (node->type == NODE_STR) {
        printf("Str: %s\n", ((Str*)node)->val);
    }
    else if (node->type == NODE_INT) {
        printf("Int: %d\n", ((Int*)node)->val);
    }
   
    for (int i = 0; i < node->children->length; i++) {
        Node *child = list_get(node->children, i);
        _print_ast(child, level + 1);
    }
}

void print_ast(Node *node) {
 _print_ast(node, 0);   
}

char* node_type_to_string(int type) {
    switch(type) {
        case 0: return "Root";
        case 1: return "Appl";
        case 2: return "Str";
        case 3: return "Int";
        default: return "Unknown";
    }
}
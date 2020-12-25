#include <stddef.h>
#include <string.h>
#include "buildins.h"
#include "parse.h"


static Signature sigs[] = {
    { "add", 2, { NODE_INT, NODE_INT }, NODE_INT },
    { "sub", 2, { NODE_INT, NODE_INT }, NODE_INT },
    { "strlen", 1, { NODE_STR }, NODE_INT },
    { "_list_end" , 0, {}, 0}
};

Signature *get_signature(char *name) {
    for (int i = 0; strcmp(sigs[i].name, "_list_end") != 0  ; i++) {
        if (strcmp(sigs[i].name, name) == 0) {
            return &sigs[i];
        }
    }
    return NULL;
}

int buildin_add (int x, int y) {
    return x + y;
}

int buildin_sub(int x, int y) {
    return x - y;
}

int buildin_strlen(char* s) {
    return strlen(s);
}
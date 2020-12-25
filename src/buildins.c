#include <stddef.h>
#include <string.h>
#include "buildins.h"
#include "parse.h"


static Signature sigs[] = {
    { "add", 2, { T_INT, T_INT }, T_INT },
    { "sub", 2, { T_INT, T_INT }, T_INT },
    { "strlen", 1, { T_STR }, T_INT },
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
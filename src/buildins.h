#ifndef BUILDINS_H
#define BUILDINS_H


typedef struct {
    char *name;
    int arg_count;
    int arg_types[10];
    int ret_type;
} Signature;

Signature *get_signature(char* name);

int buildin_add(int x, int y);
int buildin_sub(int x, int y);
int buildin_strlen(char* s);

#endif
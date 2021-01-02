#include <stdio.h>
#include <stdlib.h>
#include "read.h"

#define MAX_FILE_SIZE 100000


static int read_file(char *buf, int max, FILE *f) {
    int i = 0,
        in_comment = 0;
    char c; 

    while ((i < max) && ((c = getc(f)) != EOF)) {
        if (in_comment) {
            if (c == '\n') {
                buf[i++] = c;
                in_comment = 0;
            }
        } else { 
            if (c == ';') {
                in_comment = 1;       
            } else {
                buf[i++] = c;
            }

        }
    }

    if (i >= max) {
        printf("reached file size limit\n");
        exit(1);
    }

    return i;
}

char *get_text(char *files[], int len) {
    int text_size = 0; 
    char *text = NULL, *cur;
    FILE *f;

    for (int i = 0; i < len; i++) {

        f = fopen(files[i], "r");
        
        if (!f) {
            printf("can not open file %s\n", files[i]);
            exit(1);
        }

        text = realloc(text, text_size + MAX_FILE_SIZE + 1);
        cur = text + text_size;
        text_size += read_file(cur, MAX_FILE_SIZE, f);
        fclose(f);
    }

    if (text) {
        text[text_size] = '\0';
    }

    return text;
}
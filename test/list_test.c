#include <stdio.h>
#include "list.h"


int main() {

    char *foo = "foo", *bar = "bar", *baz = "baz", *bubu = "bubu";
    List *list = list_create();
    
    list_push(list, foo);
    list_push(list, bar);
    list_push(list, baz);

    for (int i = 0; i < list->length; i++)
        printf("%s\n", (char*)list_get(list, i));
    
    char *replaced = list_replace(list, 0, bubu);

    for (int i = 0; i < list->length; i++)
        printf("%s\n", (char*)list_get(list, i));

    printf("replaced: %s\n", replaced);

    return 0;
}
#ifndef LIST_H
#define LIST_H


typedef struct _Item {
    struct _Item *next;
    void *value;
} Item;

typedef struct {
    int length;
    Item *items;
} List;


List *list_create(void);
int list_push(List *list, void *value);
void *list_get(List* list, int pos);
void *list_last(List* list);
void *list_pop(List *list);
// Replace the `pos` element and return the replaced element
// or NULL when nothing was replaced.
void *list_replace(List *list, int pos, void *value);
// void *list_remove(List *list, int pos);

#endif
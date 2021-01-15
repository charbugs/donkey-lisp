#include <stdlib.h>
#include "list.h"


List *list_create() {
    List *list = malloc(sizeof(List));
    list->length = 0;
    list->items = NULL;
    return list;
}

int list_push(List *list, void *value) {
    Item *new = malloc(sizeof(Item));
    new->value = value;
    new->next = NULL;
    
    Item *cur = list->items;

    if (!cur) {
        list->items = new;
    } 
    else {
        while (cur->next != NULL) {
            cur = cur->next;
        }
        cur->next = new;
    }

    return ++list->length;
}

void *list_get(List *list, int pos) {
    if (pos >= list->length || pos < 0) {
        return NULL;
    }

    Item *cur = list->items;

    for (int i = 0; i < pos; i++) {
        cur = cur->next;
    }

    return cur->value;
}

void *list_last(List *list) {
    return list_get(list, list->length - 1);
}

void *list_pop(List *list) {
    Item *cur = list->items;

    if (list->length == 0) {
        return NULL;
    }

    if (list->length == 1) {
        list->items = NULL;
        list->length = 0;
        return cur->value;
    }

    // get second last item
    while(cur->next->next != NULL) {
        cur = cur->next;
    }

    Item *last = cur->next;
    cur->next = NULL;
    list->length--;

    return last->value;
}

void *list_replace(List *list, int pos, void* value) {
    if (pos >= list->length) {
        return NULL;
    }

    Item *cur = list->items;

    while (pos-- != 0) {
        cur = cur->next;
    }

    void *old = cur->value;
    cur->value = value;
    return old;      
}
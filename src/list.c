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
    if (pos >= list->length) {
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

/* void *list_remove(List *list, int pos) {
    if (pos >= list->length) {
        return NULL;
    }

    // if the item to remove is the only one in the list:
    if (pos == 0 && list->length == 1) {
        Item *target = list->items;
        list->items = NULL;
        list->length = 0;
        return target->value;
    }

    // if the item to remove is the first item and there are further items:
    if (pos == 0 && list->length > 1) {
        Item *target = list->items;
        list->items = target->next;
        list->length--;
        return target->value;
    }

    // if the item to remove is on the last position:
    if (pos == list->length - 1) {
        return list_pop(list);       
    }

    // else if the item to remove is in between:
    
    // get predecessor 
    Item *pred = list->items;
    for (int i = 0; i < pos - 1; i++) {
        pred = pred->next;
    }

    Item *target = pred->next;
    Item *succ = target->next;
    pred->next = succ;
    list->length--;
    return target->value;
} */
#include "dlist.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

void init(dlist* list) {
    list->head = NULL;
    list->tail = NULL;
}

node* xor_ptrs(node* a, node* b) {
    return (node*)((uintptr_t)a ^ (uintptr_t)b);
}

void insert(dlist* list, item* thing, bool atTail) {
    node* new = malloc(sizeof(node));
    new->thing = thing;

    /* if the list is empty */
    if(list->head == NULL && list->tail == NULL) {
        new->ptr = 0;

        list->head = new;
        list->tail = new;
    /* if there's at least one item */
    } else {
        node** firstp = atTail ? &list->tail : &list->head;
        node* first = *firstp;

        new->ptr = first;

        first->ptr = xor_ptrs(first->ptr, new);
        *firstp = new;
    }
}

item* extract(dlist* list, bool atTail) {
    /* if the list is empty */
    if(list->head == NULL && list->tail == NULL) {
        return NULL;
    /* if there's exactly one item */
    } else if(list->head == list->tail) {
        node* head = list->head;
        item* thing = head->thing;
        list->head = NULL;
        list->tail = NULL;
        free(head);
        return thing;
    /* if there's more than one item */
    } else {
        node** firstp = atTail ? &list->tail : &list->head;
        node* first = *firstp;
        node* next = first->ptr;
        item* thing = first->thing;
        next->ptr = xor_ptrs(next->ptr, first);
        *firstp = next;
        free(first);
        return thing;
    }
}

void reverse(dlist* list) {
    node* tmp;
    tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;
}

item* search(dlist* list, bool(*matches)(item*)) {
    node* next = list->head;
    node* prev = NULL;
    node* tmp;
    while(next != NULL) {
        if(matches(next->thing)) {
            return next->thing;
        }
        tmp = next;
        next = xor_ptrs(next->ptr, prev);
        prev = tmp;
    }
    return NULL;
}

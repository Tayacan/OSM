#ifndef DLIST
#define DLIST
typedef int bool;
typedef void item;

typedef struct node_ {
    item         *thing;
    struct node_ *ptr;
} node;

typedef struct dlist_ {
    node *head, *tail;
} dlist;

/* initialize a new (empty) list */
void init(dlist* list);

/* Insert in O(1). atTail determines whether to insert at the
 * head or tail of the list. */
void insert(dlist* list, item* thing, bool atTail);

/* Extract in O(1). atTail determines whether to extract at the
 * head or tail of the list. */
item* extract(dlist* list, bool atTail);

/* Reverse the list in O(1). */
void reverse(dlist* list);

/* Find the first item in the list for which matches returns true in O(n) */
item* search(dlist* list, bool(*matches)(item*));
#endif

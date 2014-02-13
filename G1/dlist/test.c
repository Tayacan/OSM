#include "dlist.h"
#include <stdio.h>
#define false 0
#define true  1

bool even(item* n) {
    return *(int*)n % 2 == 0;
}

int main(void) {
    // Create and initalize a list
    dlist l;
    init(&l);

    // Put some things at the beginning of the list
    int v0 = 3;
    int v1 = 5;
    int v2 = 8;
    insert(&l, &v0, false);
    insert(&l, &v1, true);

    // See if we can extract correctly
    item* p;
    p = extract(&l, true);

    printf("Last item in the list should be 5. It is %d.\n", *(int*)p);

    p = extract(&l, false);
    printf("First (only) item in the list should be 3. It is %d.\n", *(int*)p);

    p = extract(&l, false);
    printf("Extract should return NULL, and it ");
    if(p == NULL) {
        printf("did!\n");
    } else {
        printf("didn't!\n");
    }

    // insert things again
    insert(&l, &v0, false);
    insert(&l, &v1, false);
    insert(&l, &v2, false);
    insert(&l, &v1, false);
    // List should now be [5, 8, 5, 3]

    p = search(&l, &even);
    printf("Item found with search should be 8, and it is %d.\n", *(int*)p);

    reverse(&l);
    // List should now be [3, 5, 8, 5]

    p = extract(&l, false);
    printf("First item in the list should be 3. It is %d.\n", *(int*)p);

    p = extract(&l, true);
    printf("Last item in the list should be 5. It is %d.\n", *(int*)p);
}

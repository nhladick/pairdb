#include <stdlib.h>

#include "hashtable.h"

#define HVAL_INIT 0x811c9dc5
#define FNV_PRIME 0x01000193

struct node {
    char *key;
    char *val;
    struct node *next;
};

struct hashtbl_obj {
    struct node **arr;
    size_t arrsize;
};

hashtbl init_hashtbl(size_t tblsize)
{
    hashtbl ptr = malloc(sizeof(struct hashtbl_obj));
    if (!ptr) {
        return NULL;
    }

    ptr->arr = malloc(sizeof(struct node *) * tblsize);
    if (!ptr->arr) {
        free(ptr);
        return NULL;
    }

    ptr->arrsize = tblsize;

    return ptr;
}

size_t get_tbl_size(hashtbl ht)
{
    return ht->arrsize;
}


static unsigned int fnv_hash(void *p_in)
{
    unsigned int hval = HVAL_INIT;
    unsigned char *p = (unsigned char *) p_in;

    while (*p) {
        hval ^= (unsigned int) *p++;
        hval *= FNV_PRIME;
    }

    return hval;
}

















#include <stdlib.h>

#include "hashtable.h"
#include "buffsizes.h"

/*
 *
 * Values for Fowler/Noll/Vo hash function
 * In public domain - see links
 * https://github.com/lcn2/fnv/tree/master
 * https://github.com/lcn2/fnv/blob/master/LICENSE
 * https://github.com/lcn2/fnv/blob/master/hash_32a.c
 *
 */

#define HVAL_INIT 0x811c9dc5 // FNV hash
#define FNV_PRIME 0x01000193 // FNV hash


// Data structures
struct node {
    char *key;
    char *val;
    struct node *next;
};

struct hashtbl_obj {
    struct node **arr;
    size_t arrsize;
    size_t numentries;
};


// Hash table functions
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
    ptr->numentries = 0;

    return ptr;
}

void put(char *key, char *val)
{
    struct node *np = calloc(1, sizeof(struct node));
}

size_t get_tbl_size(hashtbl ht)
{
    return ht->arrsize;
}


/*
 *
 ********* Start - static/internal functions ********
 *
 */

static double get_load_factor(hashtbl ht)
{
    return ht->numentries / ht->arrsize;
}


/*
 *
 * Fowler/Noll/Vo hash function
 * In public domain - see links
 * https://github.com/lcn2/fnv/tree/master
 * https://github.com/lcn2/fnv/blob/master/LICENSE
 * https://github.com/lcn2/fnv/blob/master/hash_32a.c
 *
 */

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

/*
 *
 ********* End - static/internal functions ********
 *
 */

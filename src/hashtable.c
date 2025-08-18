#include <stdlib.h>
#include <string.h>

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
struct hashtbl_obj {
    struct node **arr;
    size_t arrsize;
    size_t numentries;
};

struct node {
    char *key;
    char *val;
    unsigned int hashval;
    size_t tblpos;
};

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

static void arr_insert(struct node *arr[], struct node *np)
{
    return;
}

/*
 *
 ********* End - static/internal functions ********
 *
 */


// Hash table functions
hashtbl init_hashtbl(size_t tblsize)
{
    hashtbl ptr = calloc(1, sizeof(struct hashtbl_obj));
    if (!ptr) {
        return NULL;
    }

    ptr->arr = calloc(tblsize, sizeof(struct node *));
    if (!ptr->arr) {
        free(ptr);
        return NULL;
    }

    ptr->arrsize = tblsize;
    ptr->numentries = 0;

    return ptr;
}

/*
 *
 * put function
 * input - two strings, key and val, to be copied to table
 * (not dependent on original char buffers after returning)
 * output - -1 on failure, 1 on success
 * memory must be freed after successful call
 * (node string elements created with strndup, must be freed,
 * node itself must be freed)
 *
 */

int put(hashtbl tbl, char *key, char *val)
{
    struct node *np = calloc(1, sizeof(struct node));
    if (!np) {
        return -1;
    }

    np->key = strndup(key, KEY_MAX);
    if (!np->key) {
        free(np);
        return -1;
    }

    np->val = strndup(val, VAL_MAX);
    if (!np->val) {
        free(np->key);
        free(np);
        return -1;
    }

    np->hashval = fnv_hash(np->key);
    np->tblpos = np->hashval % tbl->arrsize;
}

size_t get_tbl_size(hashtbl ht)
{
    return ht->arrsize;
}


#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "hashtable.h"
#include "keydbstring.h"

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


/*
 *
 **************** Data structures ****************
 *
 */

// all hashtable operations performed using pointer
// to hashtbl_obj - pointer defined in header file
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
************* Start - static/internal functions ************
*
*/

static double get_load_factor(hashtbl ht)
{
    return ht->numentries / ht->arrsize;
}

// Fowler/Noll/Vo hash function
// In public domain - see links
// https://github.com/lcn2/fnv/tree/master
// https://github.com/lcn2/fnv/blob/master/LICENSE
// https://github.com/lcn2/fnv/blob/master/hash_32a.c
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

// insert to hash table array using
// linear probing for collisions
static void arr_insert(hashtbl tbl, struct node *np)
{
    // start with initial expected index
    // using hash function
    size_t i = np->hashval % tbl->arrsize;

    // find first NULL element of arr
    // starting at expected index
    while (tbl->arr[i]) {
        i = (i + 1) % tbl->arrsize;
    }

    // insert pointer to array
    tbl->arr[i] = np;

    // cache table position for faster
    // loading from disk
    np->tblpos = i;
}

static void free_node(struct node *np)
{
    if (np->key) {
        free(np->key);
    }

    if (np->val) {
        free(np->val);
    }

    free(np);
}

// find hash table array index by key string
// returns -1 if key not found
static ssize_t get_index_by_key(hashtbl tbl, char *key)
{
    struct node **arr = tbl->arr;
    ssize_t i = fnv_hash(key) % tbl->arrsize;

    for (size_t j = 0; j < tbl->arrsize; i = (i + 1) % tbl->arrsize, j++) {
        if (!arr[i]) {
            continue;
        }

        if (strcmp(key, arr[i]->key) == 0) {
            return i;
        }
    }

    return -1;
}

/*
 *
 ************* End - static/internal functions ************
 *
 */



/*
 *
 *************** Hash table public functions ***************
 *
 */


// returns handle to hash table object allocated on heap
// returns NULL on failure
// on success, all array elements (node pointers) set to NULL
// empty array elements will always be NULL when using
// hash table functions
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

// frees all heap allocated components of table
// and frees table
void destroy_hashtbl(hashtbl tbl)
{
    for (size_t i = 0; i < tbl->arrsize; i++) {
        if (tbl->arr[i]) {
            free_node(tbl->arr[i]);
        }
    }

    free(tbl->arr);
    free(tbl);
}


// put function
//
// input: two strings, key and val, to be copied to table
// (no reference to original char buffers after returning)
//
// output: -1 if key already exists,
//         -2 on memory allocation failure,
//          1 on success
//
// memory must be freed with delete or destroy functions
// after successful call
//
// (node string elements created with strndup, must be freed,
// node itself must be freed)
int put(hashtbl tbl, char *key, char *val)
{
    // stop if key already exists
    if (get_index_by_key(tbl, key) >= 0) {
        return -1;
    }

    struct node *np = calloc(1, sizeof(struct node));
    if (!np) {
        return -2;
    }

    np->key = strndup(key, KEY_MAX - 1);
    if (!np->key) {
        free(np);
        return -2;
    }

    np->val = strndup(val, VAL_MAX - 1);
    if (!np->val) {
        free(np->key);
        free(np);
        return -2;
    }

    // hash value stored within node for quicker
    // loading from disk and quicker execution
    // of array expansion when necessary
    np->hashval = fnv_hash(np->key);

    arr_insert(tbl, np);
    return 1;
}

size_t find(char *dst, size_t dsize, hashtbl tbl, char *key)
{
    struct node **arr = tbl->arr;
    ssize_t i = get_index_by_key(tbl, key);

    if (i < 0) {
        return 0;
    }

    ssize_t cpy = strtcpy(dst, arr[i]->val, dsize);

    return (cpy < 0) ? dsize - 1 : cpy;
}

// removes node (key, value, hash value, arr position)
// from hash table and frees allocated memory
// idempotent - running multiple times on the same
// key has no effect
void delete(hashtbl tbl, char *key)
{
    ssize_t i = get_index_by_key(tbl, key);
    if (i < 0) {
        return;
    }

    free_node(tbl->arr[i]);
    tbl->arr[i] = NULL;
}

size_t get_tbl_size(hashtbl tbl)
{
    return tbl->arrsize;
}

// prints node info to stdout for debugging
void print_tbl(hashtbl tbl)
{
    for (size_t i = 0; i < tbl->arrsize; i++) {
        printf("node %lu\n", i);
        struct node *np = tbl->arr[i];
        if (np) {
            printf("key: %s\n", np->key);
            printf("val: %s\n", np->val);
            printf("hval: %u\n", np->hashval);
            printf("tblpos: %lu\n", np->tblpos);
        }
        else {
            printf("NULL\n");
        }
        printf("\n");
    }
}






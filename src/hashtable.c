/*
 * Pairdb - a command line key-value database
 * --------------------------------------------------
 * Copyright (C) 2025 Nikolai Hladick
 * SPDX-License-Identifier: MIT
 * https://github.com/nhladick/pairdb
 * nhladick@gmail.com
 * --------------------------------------------------
 *
 * Hashtable implementation using quadratic probing.
 * The table tracks the maximum number of times a
 * table insertion (using the put function) had to
 * probe for an open bucket. When subsequently
 * searching for a key, this number is used as the
 * limit to stop searching if a key is not found.
 * This prevents false negatives and stops the search
 * function from degrading to linear time.
 *
 */


#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "hashtable.h"
#include "stringutil.h"

/*
 *
 * Values for Fowler/Noll/Vo hash function
 * In public domain - see links
 * https://github.com/lcn2/fnv/tree/master
 * https://github.com/lcn2/fnv/blob/master/LICENSE
 * https://github.com/lcn2/fnv/blob/master/hash_32a.c
 *
 */

static const unsigned int HVAL_INIT = 0x811c9dc5; // FNV hash
static const unsigned int FNV_PRIME = 0x01000193; // FNV hash

/*
 *
 * Values for managing and resizing hash table array
 *
 */

static const unsigned int RESIZE_FACTOR = 2;
static const double LOAD_FACT_LIM = 0.60;


/*------------------ Data structures -----------------*/

// all hashtable operations performed using pointer
// to hashtbl_obj - pointer defined in header file
struct hashtbl_obj {
    struct node **arr;
    size_t arrsize;     // Full table size including empty and used buckets
    size_t numentries;  // Number of occupied buckets
    size_t maxprobe;    // Max number of probes performed during data insert
};

struct node {
    char *key;
    char *val;
    unsigned int hashval;   // hashval stored to avoid repeated hashing
    size_t tblpos;          // Table position stored for efficient loading from file
};

/*---------------- Start - static/internal functions --------------*/

static double get_load_factor(hashtbl ht)
{
    if (!ht) {
        return (double) 0.0;
    }

    return (double) ht->numentries / ht->arrsize;
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

// insert to hash table array using quadratic
// probing for collisions.
static void arr_insert(hashtbl tbl, struct node *np)
{
    if (!tbl || !np) {
        return;
    }

    // start with initial expected index
    // using hash function
    size_t hv = np->hashval;

    // Probe value - quadratic probing
    // probe = hash value + (i * i + 1) / 2
    // i starts at 0
    size_t probe = hv;

    // Probe count to track maxprobe
    size_t i = 0;

    // Find first open bucket with quadratic probing
    while (tbl->arr[probe % tbl->arrsize] != NULL) {
        i++;
        probe = hv + ((i * i + i) / 2);
    }

    if (i > tbl->maxprobe) {
        tbl->maxprobe = i;
    }

    // insert pointer to array
    tbl->arr[probe % tbl->arrsize] = np;

    // cache table position for faster
    // loading from disk
    np->tblpos = probe % tbl->arrsize;
}

static void free_node(struct node *np)
{
    if (!np) {
        return;
    }

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
    if (!tbl) {
        return -1;
    }

    struct node **arr = tbl->arr;
    ssize_t hv = fnv_hash(key);
    size_t probe = hv;

    // Loop exits if key has not been found within maxprobe iterations
    for (size_t i = 0; i <= tbl->maxprobe; i++) {
        probe = hv + ((i * i + i) / 2);
        // Start at hashvalue % table size
        if (arr[probe % tbl->arrsize]) {
            if (strcmp(key, arr[probe % tbl->arrsize]->key) == 0) {
                return probe % tbl->arrsize;
            }
        }
    }

    return -1;
}

// resize tbl array when load factor reaches LOAD_FACT_LIM
// returns 1 if successful
// returns -1 on memory allocation failure or invalid table
static int resize(hashtbl tbl)
{
    if (!tbl) {
        return -1;
    }

    struct node **prevarr = tbl->arr;
    tbl->arr = NULL;
    size_t prevsize = tbl->arrsize;
    tbl->arrsize *= RESIZE_FACTOR;
    tbl->arr = calloc(tbl->arrsize, sizeof(struct node *));
    if (!tbl->arr) {
        return -1;
    }

    for (size_t i = 0; i < prevsize; i++) {
        if (prevarr[i]) {
            arr_insert(tbl, prevarr[i]);
        }
    }

    free(prevarr);

    return 1;
}

// Input: unsigned integer a
// Returns: the nearest power of 2 that is
// greater than a
static size_t topower2(size_t a)
{
    // Shift a right (divide by 2) until a = 2.
    // Track whether a is odd at any point.
    // If no intermediate value was ever odd
    // when a == 2 with the loop, a was originally
    // a power of 2. If an intermediate value was
    // odd, a was not a power of 2. Left shift a
    // (multiply by 2) count + 1 times for next
    // greatest power of 2.
    //
    // Bit level:
    //      00001100 - shift right until a == 2
    //      00000001 - a was not divisible by 2 at step 2
    //      00010000 - a is the power of 2 that is greater
    //                 than the original value

    if (a < 2) {
        return 2;
    }

    size_t b = a;

    size_t count = 0;
    bool is_odd = false;
    while (a > 2) {
        if (a % 2 == 1) {
            is_odd = true;
        }
        a = a >> 1;
        count++;
    }

    if (is_odd) {
        return a << (count + 1);
    }
    else {
        return b;
    }
}

/*--------------- End - static/internal functions --------------*/



/*---------------- Hash table public functions -----------------*/


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

    tblsize = topower2(tblsize);

    ptr->arr = calloc(tblsize, sizeof(struct node *));
    if (!ptr->arr) {
        free(ptr);
        return NULL;
    }

    ptr->arrsize = tblsize;
    ptr->numentries = 0;
    ptr->maxprobe = 0;

    return ptr;
}

// frees all heap allocated components of table
// and frees table
void destroy_hashtbl(hashtbl tbl)
{
    if (!tbl) {
        return;
    }

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
    if (!tbl) {
        return -2;
    }

    // stop if key already exists
    if (get_index_by_key(tbl, key) >= 0) {
        return -1;
    }

    // resize table array if load factor > LOAD_FACT_LIM
    if (get_load_factor(tbl) > LOAD_FACT_LIM) {
        if (resize(tbl) < 0) {
            return -2;
        }
    }

    struct node *np = calloc(1, sizeof(struct node));
    if (!np) {
        return -2;
    }

    np->key = strndup(key, HT_KEY_MAX - 1);
    if (!np->key) {
        free(np);
        return -2;
    }

    np->val = strndup(val, HT_VAL_MAX - 1);
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
    tbl->numentries++;
    return 1;
}

size_t find(char *dst, size_t dsize, hashtbl tbl, char *key)
{
    if (!tbl) {
        return 0;
    }

    struct node **arr = tbl->arr;
    ssize_t i = get_index_by_key(tbl, key);

    if (i < 0) {
        return 0;
    }

    ssize_t cpy = strtcpy(dst, arr[i]->val, dsize);

    return (cpy < 0) ? dsize - 1 : (size_t) cpy;
}

bool exists(hashtbl tbl, char *key)
{
    if (!tbl) {
        return false;
    }

    if (get_index_by_key(tbl, key) < 0) {
        return false;
    }
    return true;
}

// removes node (key, value, hash value, arr position)
// from hash table and frees allocated memory
// idempotent - running multiple times on the same
// key has no effect
void delete(hashtbl tbl, char *key)
{
    if (!tbl) {
        return;
    }

    ssize_t i = get_index_by_key(tbl, key);
    if (i < 0) {
        return;
    }

    free_node(tbl->arr[i]);
    tbl->arr[i] = NULL;
    tbl->numentries--;
}

size_t get_tbl_size(hashtbl tbl)
{
    if (!tbl) {
        return 0;
    }

    return tbl->arrsize;
}

size_t get_numentries(hashtbl tbl)
{
    if (!tbl) {
        return 0;
    }

    return tbl->numentries;
}

// Returns pointer to heap-allocated array of
// key strings. Caller is responsible for
// freeing returned pointer.
char **get_keys(hashtbl tbl)
{
    if (!tbl) {
        return NULL;
    }

    char **keyarr = calloc(tbl->numentries, sizeof(char *));

    size_t k_index = 0;
    struct node *nptr;
    for (size_t i = 0; i < tbl->arrsize; i++) {
        if (tbl->arr[i]) {
            nptr = tbl->arr[i];
            keyarr[k_index] = nptr->key;
            k_index++;
        }
    }

    return keyarr;
}

// Returns pointer to heap-allocated array of
// val strings. Caller is responsible for
// freeing returned pointer.
char **get_vals(hashtbl tbl)
{
    if (!tbl) {
        return NULL;
    }

    char **valarr = calloc(tbl->numentries, sizeof(char *));

    size_t v_index = 0;
    struct node *nptr;
    for (size_t i = 0; i < tbl->arrsize; i++) {
        if (tbl->arr[i]) {
            nptr = tbl->arr[i];
            valarr[v_index] = nptr->val;
            v_index++;
        }
    }

    return valarr;
}

// Write (binary) all key-val pairs and
// metadata to file stream provided.
// Writes starting at location pointed
// to by stream. Input stream should
// be set to write ("w") mode.
// Returns number of items written.
size_t hashtbl_to_file(hashtbl tbl, FILE *outf)
{
    if (!tbl || !outf) {
        return 0;
    }

    // File format:
    // arrsize          (sizeof(size_t)) bytes
    // numentries       (sizeof(size_t)) bytes
    // maxprobe         (sizeof(size_t)) bytes
    // list of nodes with no separation:
    //      key len     (sizeof(size_t)) bytes
    //      key         (strlen(key)) bytes
    //      val len     (sizeof(size_t)) bytes
    //      val         (strlen(val)) bytes
    //      hashval     (sizeof(unsigned int)) bytes
    //      tblpos      (sizeof(size_t)) bytes

    size_t writecnt = 0;

    // Write arrsize
    writecnt += fwrite(&tbl->arrsize, sizeof(size_t), 1, outf);

    // Write numentries
    writecnt += fwrite(&tbl->numentries, sizeof(size_t), 1, outf);

    // Write maxprobe
    writecnt += fwrite(&tbl->maxprobe, sizeof(size_t), 1, outf);

    size_t tbllen = get_tbl_size(tbl);
    size_t keylen;
    size_t vallen;
    struct node *nptr = NULL;
    for (size_t i = 0; i < tbllen; i++) {
        if (tbl->arr[i]) {
            nptr = tbl->arr[i];

            // key
            keylen = strlen(nptr->key) + 1;
            // Write keylen
            writecnt += fwrite(&keylen, sizeof(size_t), 1, outf);
            // Write key
            writecnt += fwrite(nptr->key, keylen, 1, outf);

            // val
            vallen = strlen(nptr->val) + 1;
            // Write vallen
            writecnt += fwrite(&vallen, sizeof(size_t), 1, outf);
            // Write val
            writecnt += fwrite(nptr->val, vallen, 1, outf);

            // Write hashval
            writecnt += fwrite(&nptr->hashval, sizeof(unsigned int), 1, outf);

            // Write tblpos
            writecnt += fwrite(&nptr->tblpos, sizeof(size_t), 1, outf);
        }
    }
    return writecnt;
}

// Load hashtable from file - expects
// format provided by hashtbl_to_file.
// Input - FILE pointer to open file.
// Input stream should
// be set to read ("r") mode.
// Returns - pointer to hashtable
// allocated on heap.
hashtbl load_hashtbl_from_file(FILE *inf)
{
    if (!inf) {
        return NULL;
    }

    // Read arrsize
    size_t arrsize;
    fread(&arrsize, sizeof(size_t), 1, inf);

    // Read numentries
    size_t numentries;
    fread(&numentries, sizeof(size_t), 1, inf);

    // Read maxprobe
    size_t maxprobe;
    fread(&maxprobe, sizeof(size_t), 1, inf);

    hashtbl tbl = init_hashtbl(arrsize);
    tbl->maxprobe = maxprobe;

    char keybuff[HT_KEY_MAX] = {0};
    char valbuff[HT_VAL_MAX] = {0};
    size_t keylen;
    size_t vallen;
    struct node *nptr = NULL;
    for (size_t i = 0; i < numentries; i++) {
        nptr = calloc(1, sizeof(struct node));

        // Read keylen
        fread(&keylen, sizeof(size_t), 1, inf);

        // Read key
        fread(keybuff, keylen, 1, inf);
        nptr->key = strndup(keybuff, HT_KEY_MAX - 1);

        // Read vallen
        fread(&vallen, sizeof(size_t), 1, inf);

        // Read val
        fread(valbuff, vallen, 1, inf);
        nptr->val = strndup(valbuff, HT_VAL_MAX - 1);

        // Read hashval
        fread(&nptr->hashval, sizeof(unsigned int), 1, inf);

        // Read tblpos
        fread(&nptr->tblpos, sizeof(size_t), 1, inf);

        // Add to array
        tbl->arr[nptr->tblpos] = nptr;
        tbl->numentries++;
    }

    return tbl;
}




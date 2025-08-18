#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

// hashtable object handle
typedef struct hashtbl_obj *hashtbl;


// create table with init_hashtbl function
// hash table and all data allocated on heap
// free with destroy_hashtbl function

// returns handle to hash table object allocated on heap
// returns NULL on failure
hashtbl init_hashtbl(size_t tblsize);

void destroy_hashtbl(hashtbl tbl);

size_t get_tbl_size(hashtbl tbl);

// put
// input: two strings, key and val, to be added to table
// output: -1 on failure, 1 on success
// attempt to add key that already exists results in failure
int put(hashtbl tbl, char *key, char *val);

// key and value removed
// running multiple times on same key has no effect
void delete(hashtbl tbl, char *key);

// print key and value pairs stored in table
void print_tbl(hashtbl tbl);

#endif // HASHTABLE_H

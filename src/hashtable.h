#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

// hashtable object handle
typedef struct hashtbl_obj *hashtbl;


// Create table with init_hashtbl function.
// Hash table and all data allocated on heap.
// Free with destroy_hashtbl function.

// Returns handle to hash table object allocated on heap.
// Returns NULL on failure.
hashtbl init_hashtbl(size_t tblsize);

void destroy_hashtbl(hashtbl tbl);

size_t get_tbl_size(hashtbl tbl);

// put
// Input: two strings, key and val, to be added to table.
// Output: -1 if key already exists,
//         -2 on memory allocation failure,
//          1 on success.
// Attempt to add key that already exists results in failure.
int put(hashtbl tbl, char *key, char *val);

// Searches for value associated with key.
// If found, copies value to dst.
// Caller is responsible for allocating
// and managing dst.
// Returns length of value string copied to dst.
// Returns 0 on error or if value not found.
size_t find(char *dst, size_t dsize, hashtbl tbl, char *key);

// key and value removed
// running multiple times on same key has no effect
void delete(hashtbl tbl, char *key);

// print key and value pairs stored in table
void print_tbl(hashtbl tbl);

#endif // HASHTABLE_H

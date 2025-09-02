#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "hashtable.h"

typedef struct db_manager *db_mgr;
typedef struct db_object *db_obj;

db_mgr init_db_mgr();
void destroy_db_mgr(db_mgr dbm);

// Check whether db_mgr object has a
// current db table
bool has_curr_tbl(db_mgr dbm);

// Get new empty db_obj for use with db_mgr.
// Input: valid db_mgr handle, new tbl name string
// Returns:
//      -1 if table with tblname already exists
//      -2 on memory allocation error
//      1 on success
int get_new_tbl(db_mgr dbm, char *tblname);
db_obj use_tbl(db_mgr dbm, char *tblname);

// Writes db to file.
// Returns -1 on failure,
// returns 1 on success.
int save_curr_tbl(db_mgr dbm);

db_obj init_db_obj(char *tblname);
void destroy_db_obj(db_obj dbo);

// Input: two strings, key and val, to be added to database.
// Output: -1 if key already exists,
//         -2 on memory allocation failure,
//          1 on success.
// Attempt to add key that already exists results in failure.
int add(db_obj dbo, char *key, char *val);

// Searches for value associated with key.
// If found, copies value to dst.
// Caller is responsible for allocating
// and managing dst.
// Returns length of value string copied to dst.
// Returns 0 on error or if value not found.
size_t get(char *dst, size_t dsize, db_obj dbo, char *key);

// key and value removed
// running multiple times on same key has no effect
void db_remove(db_obj dbo, char *key);

// Copies tblname to dst.
// Returns length of value string copied to dst.
// Returns 0 on error.
size_t get_tblname(db_obj dbo, char *dst, size_t dsize);

// Returns hashtbl (pointer to hash table object)
hashtbl get_hashtbl(db_obj dbo);

#endif // DB_MANAGER_H

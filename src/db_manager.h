#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "hashtable.h"

typedef struct db_manager *db_mgr;

db_mgr init_db_mgr();
void destroy_db_mgr(db_mgr dbm);

// Check whether db_mgr object has a
// current db table
bool has_curr_tbl(db_mgr dbm);

// Get new empty table for use with db_mgr.
// Use save_curr_tbl to keep current table
// data, as this function overwrites current data.
// Input: valid db_mgr handle, new table name string
// Returns:
//      -1 if table with tblname already exists
//      -2 on memory allocation error
//      1 on success
int get_new_tbl(db_mgr dbm, char *tblname);

// Use a previously saved table within db_mgr.
// Use save_curr_tbl to keep current table
// data, as this function overwrites current data.
// Input: valid db_mgr handle, table name string
// Returns:
//      -1 if table with tblname does not exist
//      -2 on memory allocation error
//      1 on success
int use_tbl(db_mgr dbm, char *tblname);

// Writes db to file.
// Returns -1 on failure,
// returns 1 on success.
int save_curr_tbl(db_mgr dbm);

// Deletes all table data in memory and on disk
// Input: db_mgr object and string table name
// Returns: 1 on success,
//          -1 if table not found
//          -2 on file access error
int drop_tbl(db_mgr dbm, char *tblname);

// Input: valid db_mgr handle,
//        two strings, key and val, to be added to database.
// Output: -1 if key already exists,
//         -2 on memory allocation failure,
//          1 on success.
// Attempt to add key that already exists results in failure.
int add(db_mgr dbm, char *key, char *val);

// Searches for value associated with key.
// If found, copies value to dst.
// Caller is responsible for allocating
// and managing dst.
// Returns length of value string copied to dst.
// Returns 0 on error or if value not found.
size_t get(char *dst, size_t dsize, db_mgr dbm, char *key);

// key and value removed
// running multiple times on same key has no effect
void db_remove(db_mgr dbm, char *key);

// Returns number of key-val pairs
// saved in current table.
size_t get_num_tbl_entries(db_mgr dbm);

// Returns pointer to heap-allocated array of
// key strings. Caller is responsible for
// freeing returned pointer.
char **get_tbl_keys(db_mgr dbm);

// Returns pointer to heap-allocated array of
// val strings. Caller is responsible for
// freeing returned pointer.
char **get_tbl_vals(db_mgr dbm);

// Get number of tables saved in file
size_t get_numtbls(db_mgr dbm);

// Returns pointer to heap-allocated array of
// table name strings. Caller is responsible for
// freeing returned pointer.
char **get_tbls(db_mgr dbm);

#endif // DB_MANAGER_H

/*
 * --------------------------------------------------
 * Copyright (C) 2025 Nikolai Hladick
 * SPDX-License-Identifier: MIT
 * https://github.com/nhladick/pairdb
 * nhladick@gmail.com
 * --------------------------------------------------
 *
 * Database Manager - db_manager
 *
 * The database manager is an object that can be used
 * to get, modify, and save database tables. All tables
 * save key-value pairs. Use the db_mgr handle to
 * interact with a database containing multiple tables.
 * The db_mgr object can work with at most 1 current
 * database table. A table can be set to current with
 * the get_new_tbl or use_tbl functions. Use the
 * save_curr_tbl function to write the current table to
 * disk. The user is responsible for freeing the db_mgr
 * with the destroy_db_mgr function.
 *
 */

#ifndef DB_MANAGER_H
#define DB_MANAGER_H

// Use handle to db_mgr to interact
// with database tables and files
typedef struct db_manager *db_mgr;

// Init function allocates db_mgr object
// on the heap. Must be freed with destroy
// function. User is responsible for freeing.
db_mgr init_db_mgr();
void destroy_db_mgr(db_mgr dbm);

// Check whether db_mgr object has a
// current db table to perform
// operations
bool has_curr_tbl(db_mgr dbm);

// Get new empty table for use with db_mgr.
// Use save_curr_tbl to keep current table
// data, as this function overwrites current data.
// Input: valid db_mgr handle, new table name string
// Returns:
//      -1 if table with tblname already exists
//      -2 on memory allocation error
//      1 on success
// New table returned exists only in memory and
// is not written to disk until save_curr_tbl
// is called.
int get_new_tbl(db_mgr dbm, char *tblname);

// Use a previously saved table within db_mgr.
// Use save_curr_tbl to keep current table
// data, as this function overwrites current data.
// Input: valid db_mgr handle, table name string
// Returns:
//      -1 if table with tblname does not exist
//      -2 on memory allocation error
//      1 on success
// Modifications to the table are not saved
// to disk until save_curr_tbl is called.
int use_tbl(db_mgr dbm, char *tblname);

// Writes db to file and keeps table
// as current table in db_mgr.
// Returns -1 on failure,
// returns 1 on success.
int save_curr_tbl(db_mgr dbm);

// Drop table
// Input: db_mgr object and string name
// of table to be deleted.
//
// If tblname has an associated file on disk,
// the file is deleted.
//
// If tblname refers to the current, active
// table set by use_tbl or get_new_tbl,
// the table is cleared from memory and the
// db_mgr active table is set to NULL.
//
// Returns: 1 if file is successfully deleted,
//          -1 if no associated file is found,
//          -2 on file access or memory error.
//
// If file access error occurs, the table
// specified by tblname will remain accessible
// on disk and through the db_mgr.
//
// Note - if tblname refers to current table
// in db_mgr, the data in memory is always
// cleared regardless of whether a file exists
// on disk or whether it is accessed/deleted
// successfully. This is not recorded in a
// return value.
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

// Key and value removed from current table.
// Running multiple times on the same key has no effect.
// Returns 1 on success, 0 on failure.
int db_remove(db_mgr dbm, char *key);

// Returns number of key-val pairs
// saved in current table.
// Returns -1 on error.
ssize_t get_num_tbl_entries(db_mgr dbm);

// Returns pointer to heap-allocated array of
// key strings. Caller is responsible for
// freeing returned pointer.
char **get_tbl_keys(db_mgr dbm);

// Returns pointer to heap-allocated array of
// val strings. Caller is responsible for
// freeing returned pointer.
char **get_tbl_vals(db_mgr dbm);

// Get number of tables saved in file.
// Returns -1 on error.
ssize_t get_numtbls(db_mgr dbm);

// Returns pointer to heap-allocated array of
// table name strings. Caller is responsible for
// freeing returned pointer.
char **get_tbls(db_mgr dbm);

#endif // DB_MANAGER_H

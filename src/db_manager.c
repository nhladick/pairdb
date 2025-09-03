#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#include "pairdbconst.h"
#include "db_manager.h"
#include "hashtable.h"
#include "stringutil.h"

// String constants
static const char *PDB_ENV_VAR = "PAIRDB_DIR";
static const char *TBL_LIST_FNAME = "tbl_list";
static const char *PDB_FILE_EXT = ".pairdb";

enum {
    INIT_HASHTBL_SIZE = 32,
    TBL_FNAME_LEN = 11
};

struct db_manager {
    char *curr_tbl_name;
    hashtbl curr_tbl;
    hashtbl active_tbls;
    char *active_tbls_fname;
};

// Input: file name without ".pairdb" file extension.
// Allocates string on heap:
//     absolute_pairdb_dir_path + input_file_name + ".pairdb"
// Use return value to write to and read from file.
// User is responsible for freeing allocated string.
static char *get_full_path(const char *fname)
{
    char *PDB_PATH = getenv(PDB_ENV_VAR);
    size_t buffsize = strlen(PDB_PATH) +
                      strlen(fname) +
                      strlen(PDB_FILE_EXT) + 1;
    char *fullpath = calloc(1, buffsize);
    strcat(fullpath, PDB_PATH);
    strcat(fullpath, fname);
    strcat(fullpath, PDB_FILE_EXT);
    return fullpath;
}

db_mgr init_db_mgr()
{
    db_mgr ptr = calloc(1, sizeof(struct db_manager));
    if (!ptr) {
        return NULL;
    }

    ptr->active_tbls_fname = get_full_path(TBL_LIST_FNAME);

    // Check whether tbl_list file exists
    if (access(ptr->active_tbls_fname, F_OK) == 0) {
        // If exists, load tbl from file
        FILE *inf = fopen(ptr->active_tbls_fname, "r");
        if (!inf) {
            free(ptr);
            return NULL;
        }
        ptr->active_tbls = load_hashtbl_from_file(inf);
        fclose(inf);
    }
    else {
        // If it doesn't exist, create new hashtable
        // to be written when db_mgr object is freed
        ptr->active_tbls = init_hashtbl(INIT_HASHTBL_SIZE);
    }

    if (!ptr->active_tbls) {
        free(ptr);
        return NULL;
    }

    ptr->curr_tbl = NULL;
    ptr->curr_tbl_name = NULL;

    return ptr;
}

void destroy_db_mgr(db_mgr dbm)
{
    if (!dbm) {
        return;
    }

    // Write active_tbls to file
    FILE *outf = fopen(dbm->active_tbls_fname, "w");

    hashtbl_to_file(dbm->active_tbls, outf);
    fclose(outf);

    destroy_hashtbl(dbm->active_tbls);

    if (dbm->curr_tbl) {
        destroy_hashtbl(dbm->curr_tbl);
    }

    if (dbm->curr_tbl_name) {
        free(dbm->curr_tbl_name);
    }

    free(dbm->active_tbls_fname);
    free(dbm);
}

bool has_curr_tbl(db_mgr dbm)
{
    return (dbm->curr_tbl);
}

// Get new empty table for use with db_mgr.
// Input: valid db_mgr handle, new table name string
// Returns:
//      -1 if table with tblname already exists
//      -2 on memory allocation error
//      1 on success
int get_new_tbl(db_mgr dbm, char *tblname)
{
    if (exists(dbm->active_tbls, tblname)) {
        return -1;
    }

    if (dbm->curr_tbl) {
        destroy_hashtbl(dbm->curr_tbl);
    }

    if (dbm->curr_tbl_name) {
        free(dbm->curr_tbl_name);
    }

    dbm->curr_tbl = init_hashtbl(INIT_HASHTBL_SIZE);
    if (!dbm->curr_tbl) {
        return -2;
    }

    dbm->curr_tbl_name = strndup(tblname, TBL_NAME_MAX);
    if (!dbm->curr_tbl_name) {
        return -2;
    }

    return 1;
}

// Use a previously saved table within db_mgr.
// Input: valid db_mgr handle, table name string
// Returns:
//      -1 if table with tblname does not exist
//      -2 on memory allocation error
//      1 on success
int use_tbl(db_mgr dbm, char *tblname)
{
    if (!exists(dbm->active_tbls, tblname)) {
        return -1;
    }

    if (dbm->curr_tbl) {
        destroy_hashtbl(dbm->curr_tbl);
    }

    if (dbm->curr_tbl_name) {
        free(dbm->curr_tbl_name);
    }

    char fname[TBL_FNAME_LEN];
    find(fname, TBL_FNAME_LEN, dbm->active_tbls, tblname);

    char *tbl_fname = get_full_path(fname);

    FILE *inf = fopen(tbl_fname, "r");
    if (!inf) {
        return -2;
    }

    dbm->curr_tbl = load_hashtbl_from_file(inf);
    fclose(inf);

    if (!dbm->curr_tbl) {
        return -2;
    }

    dbm->curr_tbl_name = strndup(tblname, TBL_NAME_MAX);
    if (!dbm->curr_tbl_name) {
        return -2;
    }

    free(tbl_fname);

    return 1;

}

// Returns -1 on failure,
// returns 1 on success.
int save_curr_tbl(db_mgr dbm)
{
    if (!dbm) {
        return -1;
    }

    char fname[TBL_FNAME_LEN];

    // if db exists - get file name from active_tbls
    // else - create new file name
    if (exists(dbm->active_tbls, dbm->curr_tbl_name)) {
        find(fname, TBL_FNAME_LEN, dbm->active_tbls, dbm->curr_tbl_name);
    }
    else {
        getrandstr(fname, TBL_FNAME_LEN);
        put(dbm->active_tbls, dbm->curr_tbl_name, fname);
    }

    char *tbl_fname = get_full_path(fname);

    FILE *outf = fopen(tbl_fname, "w");
    if (!outf) {
        return -1;
    }
    size_t result = hashtbl_to_file(dbm->curr_tbl, outf);
    fclose(outf);

    free(tbl_fname);

    return (result == 0) ? -1 : 1;
}

// Input: valid db_mgr handle,
//        two strings, key and val, to be added to database.
// Output: -1 if key already exists,
//         -2 on memory allocation failure,
//          1 on success.
// Attempt to add key that already exists results in failure.
int add(db_mgr dbm, char *key, char *val)
{
    return put(dbm->curr_tbl, key, val);
}

// Searches for value associated with key.
// If found, copies value to dst.
// Caller is responsible for allocating
// and managing dst.
// Returns length of value string copied to dst.
// Returns 0 on error or if value not found.
size_t get(char *dst, size_t dsize, db_mgr dbm, char *key)
{
    return find(dst, dsize, dbm->curr_tbl, key);
}

// key and value removed
// running multiple times on same key has no effect
void db_remove(db_mgr dbm, char *key)
{
    delete(dbm->curr_tbl, key);
}

// Returns pointer to heap-allocated array of
// key strings. Caller is responsible for
// freeing returned pointer.
char **get_tbl_keys(db_mgr dbm)
{
    return get_keys(dbm->curr_tbl);
}

// Returns pointer to heap-allocated array of
// val strings. Caller is responsible for
// freeing returned pointer.
char **get_tbl_vals(db_mgr dbm)
{
    return get_vals(dbm->curr_tbl);
}

// Get number of tables saved in file
size_t get_numtbls(db_mgr dbm)
{
    return get_numentries(dbm->active_tbls);
}

// Returns pointer to heap-allocated array of
// table name strings. Caller is responsible for
// freeing returned pointer.
char **get_tbls(db_mgr dbm)
{
    return get_keys(dbm->active_tbls);
}


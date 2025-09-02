#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "db_manager.h"
#include "hashtable.h"
#include "keydbstring.h"

// Update to absolute path
static const char *TBL_LIST_FNAME = "tbl_list";

enum {
    INIT_HASHTBL_SIZE = 32,
    FNAME_LEN = 11
};

struct db_manager {
    char * curr_tbl_name;
    hashtbl curr_tbl;
    hashtbl active_tbls;
};

db_mgr init_db_mgr()
{
    db_mgr ptr = calloc(1, sizeof(struct db_manager));
    if (!ptr) {
        return NULL;
    }

    // Check whether tbl_list file exists
    if (access(TBL_LIST_FNAME, F_OK) == 0) {
        // If exists, load tbl from file
        FILE *inf = fopen(TBL_LIST_FNAME, "r");
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

    destroy_hashtbl(dbm->active_tbls);

    if (dbm->curr_tbl) {
        destroy_hashtbl(dbm->curr_tbl);
    }

    if (dbm->curr_tbl_name) {
        free(dbm->curr_tbl_name);
    }

    free(dbm);
}


// Get new empty db_obj for use with db_mgr.
// Input: valid db_mgr handle, new tbl name string
// Returns:
//      -1 if table with tblname already exists
//      -2 on memory allocation error
//      1 on success
int get_new_tbl(db_mgr dbm, char *tblname)
{
    if (exists(dbm->active_tbls, tblname)) {
        return -1;
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

// Get new empty db_obj for use with db_mgr.
// Input: valid db_mgr handle, new tbl name string
// Returns:
//      -1 if table with tblname does not exist
//      -2 on memory allocation or file error
//      1 on success
int use_tbl(db_mgr dbm, char *tblname)
{
    if (!exists(dbm->active_tbls, tblname)) {
        return -1;
    }

    char fname[FNAME_LEN];
    find(fname, FNAME_LEN, dbm->active_tbls, tblname);
    FILE *inf = fopen(fname, "r");
    if (!inf) {
        return -2;
    }

    dbm->curr_tbl = load_hashtbl_from_file(inf);
    fclose(inf);

    if (!dbm->curr_dbo) {
        return -2;
    }

    dbm->curr_tbl_name = strndup(tblname, TBL_NAME_MAX);
    if (!dbm->curr_tbl_name) {
        return -2;
    }

    return 1;

}

// Returns -1 on failure,
// returns 1 on success.
int save_curr_tbl(db_mgr dbm)
{
    if (!dbm) {
        return -1;
    }

    char fname[FNAME_LEN];

    // if db exists - get file name from active_tbls
    // else - create new file name
    if (exists(dbm->active_tbls, tblname)) {
        find(fname, FNAME_LEN, dbm->active_tbls, tblname);
    }
    else {
        getrandstr(fname, FNAME_LEN);
        put(dbm->active_tbls, tblname, fname);
    }

    FILE *outf = fopen(fname, "w");
    if (!outf) {
        return -1;
    }
    size_t result = hashtbl_to_file(dbm->curr_tbl, outf);
    fclose(outf);

    return (result == 0) ? -1 : 1;
}

// Input: two strings, key and val, to be added to database.
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

// Copies tblname to dst.
// Returns length of value string copied to dst.
// Returns 0 on error.
size_t get_tblname(db_mgr dbm, char *dst, size_t dsize)
{
    ssize_t cpy = strtcpy(dst, dbm->curr_tbl_name, dsize);
    return (cpy < 0) ? dsize - 1 : cpy;
}


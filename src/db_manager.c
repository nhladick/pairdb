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
    db_obj curr_dbo;
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

    ptr->curr_dbo = NULL;

    return ptr;
}

void destroy_db_mgr(db_mgr dbm)
{
    free(dbm->active_tbls);
    free(dbm);
}

db_obj get_new_tbl(db_mgr dbm, char *tblname)
{
    if (exists(dbm->active_tbls, tblname)) {
        return NULL;
    }

    return init_db_obj(tblname);
}

db_obj use_tbl(db_mgr dbm, char *tblname)
{
    if (!exists(dbm->active_tbls, tblname)) {
        return NULL;
    }

    char fname[FNAME_LEN];
    find(fname, FNAME_LEN, dbm->active_tbls, tblname);
    FILE *inf = fopen(fname, "r");
    hashtbl tbl = load_hashtbl_from_file(inf);
    fclose(inf);

    db_obj dbo = init_db_obj(tblname);
    return dbo;

}

// Returns -1 on failure,
// returns 1 on success.
int save_db_obj(db_mgr dbm, db_obj dbo)
{
    if (!dbm || !dbo) {
        return -1;
    }

    char fname[FNAME_LEN];

    char tblname[TBL_NAME_MAX];
    get_tblname(dbo, tblname, TBL_NAME_MAX);

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
    size_t result = hashtbl_to_file(get_hashtbl(dbo), outf);
    fclose(outf);

    return (result == 0) ? -1 : 1;
}

static const size_t INIT_TBL_SIZE = 32;

struct db_object {
    hashtbl tbl;
    char *tblname;
};

db_obj init_db_obj(char *tblname)
{
    db_obj ptr = calloc(1, sizeof(struct db_object));
    if (!ptr) {
        return NULL;
    }

    ptr->tbl = init_hashtbl(INIT_TBL_SIZE);
    if (!ptr->tbl) {
        free(ptr);
        return NULL;
    }

    ptr->tblname = calloc(1, TBL_NAME_MAX);
    if (!ptr->tblname) {
        free(ptr);
        return NULL;
    }

    strtcpy(ptr->tblname, tblname, TBL_NAME_MAX);

    return ptr;
}

void destroy_db_obj(db_obj dbo)
{
    destroy_hashtbl(dbo->tbl);
    free(dbo->tblname);
    free(dbo);
}

// Input: two strings, key and val, to be added to database.
// Output: -1 if key already exists,
//         -2 on memory allocation failure,
//          1 on success.
// Attempt to add key that already exists results in failure.
int add(db_obj dbo, char *key, char *val)
{
    return put(dbo->tbl, key, val);
}

// Searches for value associated with key.
// If found, copies value to dst.
// Caller is responsible for allocating
// and managing dst.
// Returns length of value string copied to dst.
// Returns 0 on error or if value not found.
size_t get(char *dst, size_t dsize, db_obj dbo, char *key)
{
    return find(dst, dsize, dbo->tbl, key);
}

// key and value removed
// running multiple times on same key has no effect
void db_remove(db_obj dbo, char *key)
{
    delete(dbo->tbl, key);
}

// Copies tblname to dst.
// Returns length of value string copied to dst.
// Returns 0 on error.
size_t get_tblname(db_obj dbo, char *dst, size_t dsize)
{
    ssize_t cpy = strtcpy(dst, dbo->tblname, dsize);
    return (cpy < 0) ? dsize - 1 : cpy;
}

hashtbl get_hashtbl(db_obj dbo)
{
    return dbo->tbl;
}






#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "db_manager.h"
#include "hashtable.h"
#include "memorydb.h"
#include "keydbstring.h"

// Update to absolute path
static const char *TBL_LIST_FNAME = "tbl_list";

enum {
    INIT_HASHTBL_SIZE = 32,
    FNAME_LEN = 11
};

struct db_manager {
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







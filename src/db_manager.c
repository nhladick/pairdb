#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

/*
 *
 ******** Start - static / internal functions ********
 *
 *
 */

// Read table list file from disk.
//      file format:
//          line 1: int file_length
//          line 2...line n: file names, one per line
// Populate hashtable with file name strings.
// Returns 1 on success.
// Returns -1 on failure.
int load_tbl_list(const char *fname, hashtbl tbl)
{
    FILE *inf = fopen(fname, "r");
    if (!inf) {
        return -1;
    }

    char buff[TBL_NAME_MAX];

    // Get length of file list
    // from first line of file
    fgets(buff, TBL_NAME_MAX, inf);
    int len = atoi(buff);

    for (int i = 0; i < len; i++) {
        fgets(buff, TBL_NAME_MAX, inf);
        buff[strlen(buff) - 1] = '\0';
        // Leave val field empty
        put(tbl, buff, "");
    }

    fclose(inf);
    return 1;
}

/*
 *
 ******** End - static / internal functions ********
 *
 *
 */

db_mgr init_db_mgr()
{
    db_mgr ptr = calloc(1, sizeof(struct db_manager));
    if (!ptr) {
        return NULL;
    }

    ptr->active_tbls = init_hashtbl(INIT_HASHTBL_SIZE);
    if (!ptr->active_tbls) {
        free(ptr);
        return NULL;
    }

    if (load_tbl_list(TBL_LIST_FNAME, ptr->active_tbls) < 0) {
        free(ptr->active_tbls);
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







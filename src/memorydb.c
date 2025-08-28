#include <stdlib.h>

#include "memorydb.h"
#include "fileio.h"
#include "hashtable.h"
#include "keydbstring.h"

#define INIT_TBL_SIZE 32

struct db_object {
    hashtbl tbl;
    char *tblname;
};

db_obj init_db_obj()
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
    int result = put(dbo->tbl, key, val);
    return result;
}

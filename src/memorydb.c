#include <stdlib.h>

#include "memorydb.h"
#include "hashtable.h"
#include "keydbstring.h"

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

#include <stdlib.h>

#include "memorydb.h"
#include "fileio.h"
#include "hashtable.h"
#include "keydbstring.h"

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

    ptr->tblname = calloc(1, TBL_NAME_MAX);
    if (!ptr->tblname) {
        free(ptr);
        return NULL;
    }

    return ptr;
}

void destroy_db_obj(db_obj dbo)
{
    free(dbo->tblname);
    free(dbo);
}

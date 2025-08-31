#ifndef DB_MANAGER_H
#define DB_MANAGER_H

#include "memorydb.h"

typedef struct db_manager *db_mgr;

db_mgr init_db_mgr();
void destroy_db_mgr(db_mgr dbm);

db_obj get_new_tbl(db_mgr dbm, char *tblname);

// Writes db to file.
// Returns -1 on failure,
// returns 1 on success.
int save_db_obj(db_mgr dbm, db_obj dbo);

#endif // DB_MANAGER_H

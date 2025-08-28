#ifndef MEMORYDB_H
#define MEMORYDB_H

typedef struct db_object *db_obj;

db_obj init_db_obj();
void destroy_db_obj(db_obj dbo);

#endif // MEMORYDB_H

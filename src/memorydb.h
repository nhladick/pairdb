#ifndef MEMORYDB_H
#define MEMORYDB_H

typedef struct db_object *db_obj;

db_obj init_db_obj();
void destroy_db_obj(db_obj dbo);

// Input: two strings, key and val, to be added to database.
// Output: -1 if key already exists,
//         -2 on memory allocation failure,
//          1 on success.
// Attempt to add key that already exists results in failure.
int add(db_obj dbo, char *key, char *val);

#endif // MEMORYDB_H

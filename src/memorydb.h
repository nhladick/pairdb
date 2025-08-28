#ifndef MEMORYDB_H
#define MEMORYDB_H

typedef struct db_object *db_obj;

db_obj init_db_obj(char *tblname);
void destroy_db_obj(db_obj dbo);

// Input: two strings, key and val, to be added to database.
// Output: -1 if key already exists,
//         -2 on memory allocation failure,
//          1 on success.
// Attempt to add key that already exists results in failure.
int add(db_obj dbo, char *key, char *val);

// Searches for value associated with key.
// If found, copies value to dst.
// Caller is responsible for allocating
// and managing dst.
// Returns length of value string copied to dst.
// Returns 0 on error or if value not found.
size_t get(char *dst, size_t dsize, db_obj dbo, char *key);

#endif // MEMORYDB_H

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

typedef struct hashtbl_obj *hashtbl;

hashtbl init_hashtbl(size_t tblsize);
void destroy_hashtbl(hashtbl tbl);
size_t get_tbl_size(hashtbl tbl);
int put(hashtbl tbl, char *key, char *val);
void remove(hashtbl tbl, char *key);

void print_tbl(hashtbl tbl);

#endif // HASHTABLE_H

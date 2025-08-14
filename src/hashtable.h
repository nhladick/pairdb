#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stddef.h>

typedef struct hashtbl_obj *hashtbl;

hashtbl init_hashtbl(size_t tblsize);

size_t get_tbl_size(hashtbl ht);

#endif // HASHTABLE_H

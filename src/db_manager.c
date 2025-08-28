#include <stdio.h>

#include "db_manager.h"
#include "hashtable.h"
#include "memorydb.h"
#include "fileio.h"

struct db_manager {
    hashtbl active_tbls;

};

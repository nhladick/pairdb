#include "memorydb.h"
#include "fileio.h"
#include "hashtable.h"

char *db_stub()
{
    fileio_stub();
    hash_stub();
    return "from db stub";
}
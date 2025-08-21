#include "../src/hashtable.h"

int main(void)
{
    hashtbl tbl = init_hashtbl(16);

    put(tbl, "key1", "val1");
    put(tbl, "key2", "val2");
    put(tbl, "key3", "val3");
    put(tbl, "key4", "val4");
    put(tbl, "key5", "val5");
    put(tbl, "key6", "val6");

    char str1[6];
    find(str1, 6, tbl, "key1");

    delete(tbl, "key1");
    delete(tbl, "key2");

    destroy_hashtbl(tbl);
}

#include <stdlib.h>

#include "parse.h"

#define TBL_NAME_MAX 32

enum parse_cmd {
    FAIL,
    NEWTABLE,
    USETABLE,
    ADD,
    DELETE
};

struct parse_object {
    enum parse_cmd cmd;
    char *tbl_name;
};

parse_data init_parse_data()
{
    parse_data ptr = malloc(sizeof(struct parse_object));
    ptr->tbl_name = malloc(TBL_NAME_MAX);
    return ptr;
}

void destroy_parse_data(parse_data ptr)
{
    free(ptr->tbl_name);
    free(ptr);
}

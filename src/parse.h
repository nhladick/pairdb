#ifndef PARSE_H
#define PARSE_H

#include <stddef.h>

#include "keydbstring.h"

// data types
enum CMD {
    FAIL,
    NEWTABLE,
    USETABLE,
    ADD,
    DELETE,
    QUIT
};

struct parse_object {
    enum CMD cmd;
    char tbl_name[TBL_NAME_MAX];
    char key[KEY_MAX];
    char val[VAL_MAX];
};

// object use functions
void parse_input(char *inbuff, struct parse_object *prs_data);
void clear_kv_buffs(struct parse_object *prs_data);

#endif // PARSE_H

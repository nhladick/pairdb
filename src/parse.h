#ifndef PARSE_H
#define PARSE_H

#include <stddef.h>

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
    char *tbl_name;
    char *key;
    char *val;
};

typedef struct parse_object *parse_data;

// object lifecycle functions
parse_data init_parse_data();
void destroy_parse_data(parse_data ptr);

// object use functions
void parse_input(char *inbuff, parse_data prs_data);
void clear_kv_buffs(parse_data prs_data);

#endif // PARSE_H

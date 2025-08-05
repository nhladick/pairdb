#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parse.h"

#define TBL_NAME_MAX 32
#define KEY_MAX 100
#define VAL_MAX 100

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
    char *key;
    char *val;
};

static void preprocess(char *cp)
{
    char *end = &cp[strlen(cp) - 1];
    *end = '\0';
    bool inquote = false;

    for (; cp < end; cp++) {
        if (*cp == ' ') {
            *cp = '\t';
        }
    }
}

parse_data init_parse_data()
{
    parse_data ptr = malloc(sizeof(struct parse_object));
    ptr->tbl_name = malloc(TBL_NAME_MAX);
    ptr->key = malloc(KEY_MAX);
    ptr->val = malloc(VAL_MAX);
    return ptr;
}

void destroy_parse_data(parse_data ptr)
{
    free(ptr->tbl_name);
    free(ptr->key);
    free(ptr->val);
    free(ptr);
}

void parse_input(char *inbuff, parse_data prs_data)
{
    preprocess(inbuff);
}














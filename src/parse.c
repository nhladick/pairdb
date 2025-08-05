#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// remove after testing
#include <stdio.h>

#include "parse.h"

#define TBL_NAME_MAX 32
#define KEY_MAX 100
#define VAL_MAX 100
#define MAX_ARGS 5

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
    // remove newline char from fgets
    // end points to null char
    char *end = &cp[strlen(cp) - 1];
    *end = '\0';

    bool inquote = false;

    for (; cp < end; cp++) {
        // determine whether cp is within a quoted section
        // check for single and double quote chars
        if (*cp == '"' || *cp == '\'') {
            if (inquote == false) {
                inquote = true;
            }
            else {
                inquote = false;
            }
        }

        // if not in a quoted section, replace space with tab
        if (!inquote && *cp == ' ') {
            *cp = '\t';
        }
    }
}

static void tokenize(char *inbuff, char *argv[])
{
    char *delim = "\t\'\"";
    char *temp = strtok(inbuff, delim);
    argv[0] = temp;

    for (int i = 1; i < MAX_ARGS && temp; i++) {
        temp = strtok(NULL, delim);
        argv[i] = temp;
    }
}

parse_data init_parse_data()
{
    parse_data ptr = calloc(1, sizeof(struct parse_object));
    ptr->tbl_name = calloc(1, TBL_NAME_MAX);
    ptr->key = calloc(1, KEY_MAX);
    ptr->val = calloc(1, VAL_MAX);
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

    char *argv[MAX_ARGS];

    for (int i = 0; i < MAX_ARGS; i++) {
        argv[i] = NULL;
    }

    tokenize(inbuff, argv);


}














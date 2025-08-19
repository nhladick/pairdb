#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parse.h"
#include "keydbstring.h"

#define MAX_ARGS 5

struct parse_object {
    enum CMD cmd;
    char *tbl_name;
    char *key;
    char *val;
};

/*
 *
 ********** start - static/internal functions **********
 *
 */

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

static enum CMD parse_cmd(char *str_cmd)
{
    if (str_cmd == NULL) {
        return FAIL;
    }

    if (strcmp(str_cmd, "newtbl") == 0) {
        return NEWTABLE;
    }
    else if (strcmp(str_cmd, "use") == 0) {
        return USETABLE;
    }
    else if (strcmp(str_cmd, "add") == 0) {
        return ADD;
    }
    else if (strcmp(str_cmd, "del") == 0) {
        return DELETE;
    }
    else if (strcmp(str_cmd, "quit") == 0) {
        return QUIT;
    }
    else {
        return FAIL;
    }
}

static void parse_args(char *argv[], parse_data prs_data)
{
    switch (prs_data->cmd) {
        case FAIL:
        case QUIT:
            break;
        case NEWTABLE:
        case USETABLE:
            if (argv[1] == NULL) {
                prs_data->cmd = FAIL;
                return;
            }
            strtcpy(prs_data->tbl_name, argv[1], TBL_NAME_MAX);
            break;
        case ADD:
            if (argv[1] == NULL || argv[2] == NULL) {
                prs_data->cmd = FAIL;
                return;
            }
            strtcpy(prs_data->key, argv[1], KEY_MAX);
            strtcpy(prs_data->val, argv[2], VAL_MAX);
            break;
        case DELETE:
            if (argv[1] == NULL) {
                prs_data->cmd = FAIL;
                return;
            }
            strtcpy(prs_data->key, argv[1], KEY_MAX);
            break;
    }
}

/*
 *
 ********** end - static/internal functions **********
 *
 */

parse_data init_parse_data()
{
    parse_data ptr = calloc(1, sizeof(struct parse_object));
    if (!ptr) {
        return NULL;
    }

    ptr->tbl_name = calloc(1, TBL_NAME_MAX);
    if (!ptr->tbl_name) {
        free(ptr);
        return NULL;
    }

    ptr->key = calloc(1, KEY_MAX);
    if (!ptr->key) {
        free(ptr->tbl_name);
        free(ptr);
        return NULL;
    }

    ptr->val = calloc(1, VAL_MAX);
    if (!ptr->val) {
        free(ptr->key);
        free(ptr->tbl_name);
        free(ptr);
        return NULL;
    }

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

    for (size_t i = 0; i < MAX_ARGS; i++) {
        argv[i] = NULL;
    }

    tokenize(inbuff, argv);

    prs_data->cmd = parse_cmd(argv[0]);

    parse_args(argv, prs_data);
}

enum CMD get_input_cmd(parse_data prs_data)
{
    return prs_data->cmd;
}

const char *get_tbl_name(parse_data prs_data)
{
    return (const char *) prs_data->tbl_name;
}

void clear_kv_buffs(parse_data prs_data)
{
    memset(prs_data->key, 0, KEY_MAX);
    memset(prs_data->val, 0, VAL_MAX);
}

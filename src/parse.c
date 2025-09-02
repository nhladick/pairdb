#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parse.h"
#include "keydbstring.h"

enum {
    MAX_ARGS = 5
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

    for (size_t i = 1; i < MAX_ARGS && temp; i++) {
        temp = strtok(NULL, delim);
        argv[i] = temp;
    }
}

static enum CMD parse_cmd(char *str_cmd)
{
    if (str_cmd == NULL) {
        return FAIL;
    }

    if (strcmp(str_cmd, "lstbls") == 0) {
        return LSTABLES;
    }
    else if (strcmp(str_cmd, "newtbl") == 0) {
        return NEWTABLE;
    }
    else if (strcmp(str_cmd, "use") == 0) {
        return USETABLE;
    }
    else if (strcmp(str_cmd, "add") == 0) {
        return ADD;
    }
    else if (strcmp(str_cmd, "get") == 0) {
        return GET;
    }
    else if (strcmp(str_cmd, "del") == 0) {
        return DELETE;
    }
    else if (strcmp(str_cmd, "save") == 0) {
        return SAVE;
    }
    else if (strcmp(str_cmd, "help") == 0) {
        return HELP;
    }
    else if (strcmp(str_cmd, "quit") == 0) {
        return QUIT;
    }
    else {
        return FAIL;
    }
}

static void parse_args(char *argv[], struct parse_object *prs_data)
{
    switch (prs_data->cmd) {
        case FAIL:
        case QUIT:
        case SAVE:
        case HELP:
        case LSTABLES:
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
        case GET:
            if (argv[1] == NULL) {
                prs_data->cmd = FAIL;
                return;
            }
            strtcpy(prs_data->key, argv[1], KEY_MAX);
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

void parse_input(char *inbuff, struct parse_object *prs_data)
{
    preprocess(inbuff);
    char *argv[MAX_ARGS] = {NULL};
    tokenize(inbuff, argv);
    prs_data->cmd = parse_cmd(argv[0]);
    parse_args(argv, prs_data);
}

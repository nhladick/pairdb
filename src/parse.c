/*
 * --------------------------------------------------
 * Copyright (C) 2025 Nikolai Hladick
 * SPDX-License-Identifier: MIT
 * https://github.com/nhladick/pairdb
 * nhladick@gmail.com
 * --------------------------------------------------
 *
 * The parse_input function reads a string buffer and returns
 * a parse object. The parse object has a CMD and three
 * string values. If the first argument is a valid command and
 * the correct number of arguments is provided for that
 * command, the CMD field is set to the proper command value,
 * and the key, value, and table name fields are set accordingly.
 * If a command is not recognized or an incorrect number of
 * arguments is provided for a command, then the CMD field
 * is set to FAIL.
 *
 * Input is parsed in 4 steps:
 *
 *  1. Preprocess -     The newline character left in the input
 *                      buffer by fgets is replaced with the NUL
 *                      character. The input buffer is scanned.
 *                      All spaces outside of sections enclosed
 *                      in single or double quotation marks are
 *                      replaced with a marker character for
 *                      further processing.
 *
 *  2. Tokenize -       The preprocessed input buffer is broken
 *                      into separate strings based on the new
 *                      marker character and quotation mark
 *                      characters.
 *
 *  3. Command Parse -  The initial string is parsed to determine
 *                      whether it is a valid command. If it is
 *                      not valid, the enum CMD element of the
 *                      parse object is set to FAIL.
 *
 *  4. Argument Parse - The rest of the strings are parsed. The
 *                      argument parse function determines whether
 *                      the correct number of arguments are present
 *                      for each command. Extra arguments are ignored.
 *
 */


#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pairdbconst.h"
#include "parse.h"
#include "stringutil.h"

enum {
    MAX_ARGS = 3
};

/*---------- start - static/internal functions ------------*/

static void preprocess(char *cp)
{
    // fgets leaves newline char in
    // buffer as last char of string.
    // Replace newline char with NUL
    // char to get NUL terminated
    // input string.
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

static void tokenize(char *inbuff, char *argv[], size_t len)
{
    char *delim = "\t\'\"";
    char *temp = strtok(inbuff, delim);
    argv[0] = temp;

    for (size_t i = 1; i < len && temp; i++) {
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
    else if (strcmp(str_cmd, "drop") == 0) {
        return DROPTABLE;
    }
    else if (strcmp(str_cmd, "lsdata") == 0) {
        return LSDATA;
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
        case LSDATA:
            break;
        case NEWTABLE:
        case USETABLE:
        case DROPTABLE:
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

/*-------------- end - static/internal functions --------------*/

void parse_input(char *inbuff, struct parse_object *prs_data)
{
    preprocess(inbuff);
    char *argv[MAX_ARGS] = {NULL};
    tokenize(inbuff, argv, MAX_ARGS);
    prs_data->cmd = parse_cmd(argv[0]);
    parse_args(argv, prs_data);
}

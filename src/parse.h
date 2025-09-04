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
 */


#ifndef PARSE_H
#define PARSE_H

#include <stddef.h>

#include "pairdbconst.h"

enum CMD {
    FAIL,
    LSTABLES,
    NEWTABLE,
    USETABLE,
    ADD,
    GET,
    DELETE,
    SAVE,
    DROPTABLE,
    LSDATA,
    HELP,
    QUIT
};

struct parse_object {
    enum CMD cmd;
    char tbl_name[TBL_NAME_MAX];
    char key[KEY_MAX];
    char val[VAL_MAX];
};

void parse_input(char *inbuff, struct parse_object *prs_data);

#endif // PARSE_H

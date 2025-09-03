/*
 * Pairdb - a command line key-value database
 * --------------------------------------------------
 * Copyright (C) Nikolai Hladick - 2025
 * https://github.com/nhladick/pairdb
 * nhladick@gmail.com
 * --------------------------------------------------
 *
 * Pairdb is an interactive key-value database for use
 * at the command line - usage: 'pairdb'. This program
 * also accepts input files at stdin to execute batch
 * commands - usage 'pairdb < <input_file>' (see example
 * below).
 *
 * Pairdb can manage multiple named database tables. Each
 * table contains key-value pairs of strings. Upon starting
 * the program, a table must be created or a previously
 * saved table must be chosen to work with. Users can add
 * entries to a table, delete entries from a table, find
 * entries within a table, and save a table to disk. Pairdb
 * manages database files with its own file format. Syntax
 * for database operations is listed below.
 *
 * ------------------- Commands -----------------------
 *         SYNTAX                   ACTIONS
 * newtbl <table_name>        Creates a new table in
 *                            memory and sets as current
 *                            table to be used for
 *                            subsequent commands.
 *                            Command fails if a table
 *                            with <table_name> already
 *                            exists. If used when another
 *                            table is already set as
 *                            current table, that table
 *                            is saved before switching
 *                            to the new table.
 *
 * use <table_name>           Loads a previously saved
 *                            table into memory and sets
 *                            as current table to be used
 *                            for subsequent commands.
 *                            Command fails if a table
 *                            with <table_name> does not
 *                            exist. If used when another
 *                            table is already set as
 *                            current table, that table is
 *                            saved before switching to the
 *                            table specified.
 *
 * save                       Saves current table to disk.
 *
 * lstbls                     Prints list of all saved tables.
 *
 * add <key> <val>            Adds key-value pair to current
 *                            table. Command fails if <key>
 *                            already exists in current table.
 *
 * get <key>                  Returns <val> associated with
 *                            previously added <key> in current
 *                            table.
 *
 * del <key>                  Deletes <key> <val> pair from
 *                            current table.
 *
 * lsdata                     Lists all key-value pairs in current
 *                            table.
 *
 * help                       Prints information on commands.
 *
 * quit                       Quit interactive program and save
 *                            current table to disk.
 *
 * Any of the above commands can be placed in an input file with
 * one command per line (separated by the newline character). Use
 * the quit command on the final line. All data will be saved
 * to disk in the table(s) specified.
 *
 * Example:
 *
 *   file: input.txt
 *      newtbl table1
 *      add key1 val1
 *      add key2 val2
 *      quit
 *
 *   command line:
 *      pairdb < input.txt
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "parse.h"
#include "db_manager.h"
#include "messages.h"

enum {
    INBUFF_SIZE = 256
};

int main(int argc, char *argv[])
{
    // Print program info if any command line
    // argument is provided
    if (argc > 1) {
        printf("%s", long_help_msg());
        exit(EXIT_FAILURE);
    }

    struct parse_object parse_data = {0};
    char inbuff[INBUFF_SIZE];

    db_mgr dbmgr = init_db_mgr();
    if (!dbmgr) {
        exit(EXIT_FAILURE);
    }

    printf("%s", intro_msg());

    bool run_loop = true;
    while (run_loop) {
        // clear input buffer
        memset(inbuff, 0, INBUFF_SIZE);

        printf("pairdb>> ");

        fgets(inbuff, INBUFF_SIZE, stdin);
        parse_input(inbuff, &parse_data);

        // Commands 'use <tbl_name>' or 'newtbl <tbl_name>'
        // will set the table name that will be used until
        // another 'use' or 'newtbl' command is received.
        // FAIL, QUIT, HELP are bypassed - no table needed
        // for these commands.
        if ((parse_data.cmd == ADD ||
            parse_data.cmd == GET ||
            parse_data.cmd == DELETE ||
            parse_data.cmd == SAVE ||
            parse_data.cmd == LSDATA) &&
            parse_data.tbl_name[0] == '\0') {
                printf("No table selected: 'use <tbl_name>' or 'newtbl <tbl_name>'\n");
                printf("Use 'lstbls' to see all tables\n");
                continue;
            }

        switch (parse_data.cmd) {
            case FAIL:
                printf("%s", short_help_msg());
                break;
            case LSTABLES:
                size_t numtbls = get_numtbls(dbmgr);
                char **tbls = get_tbls(dbmgr);
                for (size_t i = 0; i < numtbls; i++) {
                    printf("%s\n", tbls[i]);
                }
                free(tbls);
                break;
            case NEWTABLE:
                if (has_curr_tbl(dbmgr)) {
                    save_curr_tbl(dbmgr);
                }
                int newtbl_stat = get_new_tbl(dbmgr, parse_data.tbl_name);
                if (newtbl_stat == -1) {
                    printf("Table already exists\n");
                    // Reset table name field in parse_object
                    parse_data.tbl_name[0] = '\0';
                }
                else if (newtbl_stat == -2) {
                    fprintf(stderr, "Memory allocation error\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case USETABLE:
                if (has_curr_tbl(dbmgr)) {
                    save_curr_tbl(dbmgr);
                }
                int usetbl_stat = use_tbl(dbmgr, parse_data.tbl_name);
                if (usetbl_stat == -1) {
                    printf("Table does not exist\n");
                    // Reset table name field in parse_object
                    parse_data.tbl_name[0] = '\0';
                }
                else if (usetbl_stat == -2) {
                    fprintf(stderr, "Memory allocation error\n");
                    exit(EXIT_FAILURE);
                }
                break;
            case ADD:
                int result = add(dbmgr, parse_data.key, parse_data.val);
                if (result == -1) {
                    printf("Key %s already exists\n", parse_data.key);
                }
                else if (result == -2) {
                    printf("Memory allocation error\n");
                }
                break;
            case GET:
                char buff[VAL_MAX];
                if (get(buff, VAL_MAX, dbmgr, parse_data.key) == 0) {
                    printf("Value not found\n");
                }
                else {
                    printf("%s\n", buff);
                }
                break;
            case DELETE:
                db_remove(dbmgr, parse_data.key);
                break;
            case SAVE:
                if (has_curr_tbl(dbmgr)) {
                    save_curr_tbl(dbmgr);
                }
                break;
            case LSDATA:
                printf("KEY\t\t\t-\tVAL\n");
                printf("--------------------------------------\n");
                size_t numentries = get_num_tbl_entries(dbmgr);
                char **keys = get_tbl_keys(dbmgr);
                char **vals = get_tbl_vals(dbmgr);
                for (size_t i = 0; i < numentries; i++) {
                    printf("%s\t\t\t-\t%s\n", keys[i], vals[i]);
                }
                free(keys);
                free(vals);
                break;
            case HELP:
                printf("%s", long_help_msg());
                break;
            case QUIT:
                if (has_curr_tbl(dbmgr)) {
                    save_curr_tbl(dbmgr);
                }
                run_loop = false;
                break;
        }
    }
    destroy_db_mgr(dbmgr);
}

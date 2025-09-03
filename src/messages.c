#include "messages.h"

const char *intro_msg()
{
    const char *msg =
                "Pairdb - a command line key-value database\n"
                "Copyright (C) 2025 Nikolai Hladick\n"
                "SPDX-License-Identifier: MIT\n"
                "https://github.com/nhladick/pairdb\n"
                "nhladick@gmail.com\n"
                "--------------------------------------------------\n"
                "Use help command for info\n";
    return msg;
}

const char *short_help_msg()
{
    const char *msg =
                "syntax error\n"
                "commands: newtbl <tbl_name>\n"
                "          use <tbl_name>\n"
                "          save\n"
                "          lstbls\n"
                "          drop <tbl_name>\n"
                "          add <key> <val>\n"
                "          get <key>\n"
                "          del <key>\n"
                "          lsdata\n"
                "          help\n"
                "          quit\n"
                "use help command for more info\n";
    return msg;
}

const char *long_help_msg()
{
    const char *msg =
            "Pairdb is an interactive key-value database for use\n"
            "at the command line - usage: 'pairdb'. This program\n"
            "also accepts input files at stdin to execute batch\n"
            "commands - usage 'pairdb < <input_file>' (see example\n"
            "below).\n\n"
            "Pairdb can manage multiple named database tables. Each\n"
            "table contains key-value pairs of strings. Upon starting\n"
            "the program, a table must be created or a previously\n"
            "saved table must be chosen to work with. Users can add\n"
            "entries to a table, delete entries from a table, find\n"
            "entries within a table, and save a table to disk. Pairdb\n"
            "manages database files with its own file format. Syntax\n"
            "for database operations is listed below.\n\n"
            "------------------- Commands -----------------------\n"
            "         SYNTAX                   ACTIONS           \n"
            " newtbl <table_name>        Creates a new table in\n"
            "                            memory and sets as current\n"
            "                            table to be used for\n"
            "                            subsequent commands.\n"
            "                            Command fails if a table\n"
            "                            with <table_name> already\n"
            "                            exists. If used when another\n"
            "                            table is already set as\n"
            "                            current table, that table\n"
            "                            is saved before switching\n"
            "                            to the new table.\n\n"
            " use <table_name>           Loads a previously saved\n"
            "                            table into memory and sets\n"
            "                            as current table to be used\n"
            "                            for subsequent commands.\n"
            "                            Command fails if a table\n"
            "                            with <table_name> does not\n"
            "                            exist. If used when another\n"
            "                            table is already set as\n"
            "                            current table, that table is\n"
            "                            saved before switching to the\n"
            "                            table specified.\n\n"
            " save                       Saves current table to disk.\n\n"
            " lstbls                     Prints list of all saved tables.\n\n"
            " drop <table_name>          Drops table by deleting file\n"
            "                            associated with <table_name> and\n"
            "                            removing <table_name> from database\n"
            "                            table list. If <table_name> refers\n"
            "                            to the current active table, all\n"
            "                            table data is cleared from memory,\n"
            "                            and another table must be created\n"
            "                            or selected to perform any table\n"
            "                            operations.\n\n"
            " add <key> <val>            Adds key-value pair to current\n"
            "                            table. Command fails if <key>\n"
            "                            already exists in current table.\n\n"
            " get <key>                  Returns <val> associated with\n"
            "                            previously added <key> in current\n"
            "                            table.\n\n"
            " del <key>                  Deletes <key> <val> pair from\n"
            "                            current table.\n\n"
            " lsdata                     Lists all key-value pairs in current\n"
            "                            table.\n\n"
            " help                       Prints information on commands.\n\n"
            " quit                       Quit interactive program and save\n"
            "                            current table to disk.\n\n"
            " Any of the above commands can be placed in an input file with\n"
            " one command per line (separated by the newline character). Use\n"
            " the quit command on the final line. All data will be saved\n"
            " to disk in the table(s) specified.\n\n"
            " Example:\n\n"
            "   file: input.txt\n"
            "      newtbl table1\n"
            "      add key1 val1\n"
            "      add key2 val2\n"
            "      quit\n\n"
            "   command line:\n"
            "      pairdb < input.txt\n";

    return msg;
}

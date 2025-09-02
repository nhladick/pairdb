#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "parse.h"
#include "db_manager.h"

static const size_t INBUFF_SIZE = 256;

int main(int argc, char *argv[])
{
    struct parse_object parse_data = {0};
    char inbuff[INBUFF_SIZE];

    db_mgr dbmgr = init_db_mgr();
    if (!dbmgr) {
        exit(EXIT_FAILURE);
    }

    bool run_loop = true;
    while (run_loop) {
        // clear input buffer
        memset(inbuff, 0, INBUFF_SIZE);

        printf("keydb>> ");

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
            parse_data.cmd == SAVE) &&
            parse_data.tbl_name[0] == '\0') {
                printf("No table selected: 'use <tbl_name>' or 'newtbl <tbl_name>'\n");
                continue;
            }

        switch (parse_data.cmd) {
            case FAIL:
                printf("fail\n");
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
                printf("usetable\n");
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
                printf("save\n");
                break;
            case HELP:
                printf("help\n");
                break;
            case QUIT:
                if (has_curr_tbl(dbmgr)) {
                    save_curr_tbl(dbmgr);
                }
                printf("quit\n");
                run_loop = false;
                break;
        }
    }
    destroy_db_mgr(dbmgr);
}

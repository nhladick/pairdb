#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "parse.h"
#include "memorydb.h"

#define INBUFF_SIZE 256

int main(int argc, char *argv[])
{
    struct parse_object parse_data = {0};
    char inbuff[INBUFF_SIZE];

    db_obj dbo = NULL;

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
        if (parse_data.cmd != FAIL &&
            parse_data.cmd != QUIT &&
            parse_data.cmd != HELP &&
            parse_data.tbl_name[0] == '\0') {
                printf("No table selected: 'use <tbl_name>' or 'newtbl <tbl_name>'\n");
                continue;
            }

        switch (parse_data.cmd) {
            case FAIL:
                printf("fail\n");
                break;
            case NEWTABLE:
                dbo = init_db_obj(parse_data.tbl_name);
                break;
            case USETABLE:
                printf("usetable\n");
                break;
            case ADD:
                int result = add(dbo, parse_data.key, parse_data.val);
                if (result == -1) {
                    printf("Key %s already exists\n", parse_data.key);
                }
                else if (result == -2) {
                    printf("Memory allocation error\n");
                }
                break;
            case GET:
                char buff[VAL_MAX];
                if (get(buff, VAL_MAX, dbo, parse_data.key) == 0) {
                    printf("Value not found\n");
                }
                else {
                    printf("%s\n", buff);
                }
                break;
            case DELETE:
                db_remove(dbo, parse_data.key);
                break;
            case SAVE:
                printf("save\n");
                break;
            case HELP:
                printf("help\n");
                break;
            case QUIT:
                printf("quit\n");
                run_loop = false;
                break;
        }
    }
}

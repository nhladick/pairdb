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
        // Include FAIL to go to syntax error handle.
        if (parse_data.cmd != FAIL &&
            parse_data.cmd != QUIT &&
            parse_data.tbl_name[0] == '\0') {
                printf("No table selected: 'use <tbl_name>' or 'newtbl <tbl_name>'\n");
                continue;
            }

        switch (parse_data.cmd) {
            case FAIL:
                printf("fail\n");
                break;
            case NEWTABLE:
                printf("newtbl\n");
                break;
            case USETABLE:
                printf("usetable\n");
                break;
            case ADD:
                printf("add\n");
                break;
            case DELETE:
                printf("del\n");
                break;
            case QUIT:
                printf("quit\n");
                run_loop = false;
                break;
        }
    }
}

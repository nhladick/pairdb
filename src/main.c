#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "parse.h"
#include "memorydb.h"

#define INBUFF_SIZE 256

int main(int argc, char *argv[])
{
    parse_data prs_data = init_parse_data();
    char inbuff[INBUFF_SIZE];

    bool run_loop = true;

    while (run_loop) {
        // clear input buffer
        memset(inbuff, 0, INBUFF_SIZE);

        printf("keydb>> ");

        fgets(inbuff, INBUFF_SIZE, stdin);
        parse_input(inbuff, prs_data);
        enum CMD input_cmd = get_input_cmd(prs_data);

        if (input_cmd != NEWTABLE &&
            input_cmd != USETABLE &&
            input_cmd != FAIL &&
            (get_tbl_name(prs_data))[0] == '\0') {
                printf("No table selected: 'use <tbl_name>' or 'newtbl <tbl_name>'\n");
                clear_kv_buffs(prs_data);
                continue;
            }

        switch (input_cmd) {
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
        // clear key and val buffers in prs_data
        clear_kv_buffs(prs_data);

        // keep table name in prs_data - handle table name
        // logic in main switch loop
    }

    destroy_parse_data(prs_data);
}

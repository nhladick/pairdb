#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"
#include "memorydb.h"

#define INBUFF_SIZE 256

int main(int argc, char *argv[])
{
    parse_data prs_data = init_parse_data();
    char inbuff[INBUFF_SIZE];

    bool run_loop = true;

    while (run_loop) {
        printf("keydb>> ");

        fgets(inbuff, INBUFF_SIZE, stdin);
        parse_input(inbuff, prs_data);

        switch (get_input_cmd(prs_data)) {
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
        }

        run_loop = false;
    }

    destroy_parse_data(prs_data);
}

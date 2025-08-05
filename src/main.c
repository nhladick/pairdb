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
        printf("%s\n", inbuff);
        run_loop = false;
    }

    destroy_parse_data(prs_data);
}

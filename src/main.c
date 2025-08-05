#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"
#include "memorydb.h"

int main(int argc, char *argv[])
{
    parse_data prs_data = init_parse_data();

    bool run_loop = true;

    while (run_loop) {
        printf("keydb>> ");
    }

    destroy_parse_data(prs_data);
}

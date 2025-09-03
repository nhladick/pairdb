#include <stdio.h>

#include "messages.h"

void intro_msg()
{
    char *msg =
"Pairdb - a command line key-value database\n\
Copyright (C) Nikolai Hladick - 2025\n\
https://github.com/nhladick/pairdb\n\
nhladick@gmail.com\n\
--------------------------------------------------\n\
Use help command for info\n";
    printf("%s", msg);
}

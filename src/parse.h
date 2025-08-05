#ifndef PARSE_H
#define PARSE_H

// data types
enum CMD {
    FAIL,
    NEWTABLE,
    USETABLE,
    ADD,
    DELETE
};

typedef struct parse_object *parse_data;

// object lifecycle functions
parse_data init_parse_data();
void destroy_parse_data(parse_data ptr);

// object use functions
void parse_input(char *inbuff, parse_data prs_data);
enum CMD get_input_cmd(parse_data prs_data);

#endif // PARSE_H

#ifndef PARSE_H
#define PARSE_H

enum CMD;
typedef struct parse_object *parse_data;

parse_data init_parse_data();
void destroy_parse_data(parse_data ptr);

void parse_input(char *inbuff, parse_data prs_data);

#endif // PARSE_H

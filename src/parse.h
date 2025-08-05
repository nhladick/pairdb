#ifndef PARSE_H
#define PARSE_H

enum parse_cmd;
typedef struct parse_object *parse_data;

parse_data init_parse_data();
void destroy_parse_data(parse_data ptr);

#endif // PARSE_H

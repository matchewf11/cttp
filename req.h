#ifndef REQ_H
#define REQ_H

#define max_headers 10

typedef enum {
    Get,
    Post,
} Method;

typedef struct {
    const char *name;
    int name_len;
    const char *value;
    int value_len;
} Header;

typedef struct {
    Method method;
    const char *path;
    int path_len;
    char version[9];
    Header headers[max_headers];
    int header_count;
} Request;

int compare_req(const Request *a, const Request *b);

Request parse_req(const char *input);

#endif

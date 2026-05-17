#ifndef REQ_H
#define REQ_H

int run_server();

typedef enum {
    Get,
    Post,
} Method;

typedef struct {
    Method method;
    const char *path;
    int path_len;
    char version[9];
} Request;

Request parse_req(const char *input);

int compare_req(const Request *a, const Request *b);

#endif

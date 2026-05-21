#include <string.h>
#include <stdio.h>

#include "req.h"
#include "comp.h"
#include "log.h"

static inline int is_space(char c) {
    return c == ' ' || c == '\t' || c == '\n';
}

static inline void eat(const char **cursor) {
    (*cursor)++;
}

static inline void eat_n(const char **cursor, int n) {
    *cursor += n;
}

static inline void eat_spaces(const char **cursor) {
    while (is_space(**cursor)) {
        eat(cursor);
    }
}

const char* display_method(Method method) {
    switch (method) {
        case Get:
            return "Get";
        case Post:
            return "Post";
    }
    unreachable();
}

void display_req(const Request *req) {
    printf("Method: %s\nPath: ", display_method(req->method));
    for (int i = 0; i < req->path_len; i++) {
        printf("%c", req->path[i]);
    }
    printf("\nVersion: %s\n", req->version);
}

int compare_req(const Request *a, const Request *b) {
    int cmp;
    if ((cmp = a->method - b->method) != 0) {
        return cmp;
    }
    if ((cmp = a->path_len - b->path_len) != 0) {
        return cmp;
    }
    if ((cmp = strcmp(a->version, b->version)) != 0) {
        return cmp;
    }
    if ((cmp = a->header_count - b->header_count) != 0) {
        return cmp;
    }
    for (int i = 0; i < a->header_count; i++) {
        Header ha = a->headers[i];
        Header hb = b->headers[i];
        cmp = ha.name_len - hb.name_len;
        if (cmp != 0) {
            return cmp;
        }
        cmp = ha.value_len - hb.value_len;
        if (cmp != 0) {
            return cmp;
        }
        cmp = ha.value_len - hb.value_len;
        if (cmp != 0) {
            return cmp;
        }
        cmp = strncmp(ha.name, hb.name, ha.name_len);
        if (cmp != 0) {
            return cmp;
        }
        cmp = strncmp(ha.value, hb.value, ha.value_len);
        if (cmp != 0) {
            return cmp;
        }
    }
    return strncmp(a->path, b->path, a->path_len);
}

Method parse_method(const char **cursor) {
    if (strncmp(*cursor, "GET", 3) == 0) {
        eat_n(cursor, 3);
        return Get;
    }

    if (strncmp(*cursor, "POST", 4) == 0) {
        eat_n(cursor, 4);
        return Post;
    }

    unreachable();
}

void get_version(const char **cursor, Request *req) {
    int i = 0;
    while (**cursor != ' ' && **cursor != '\r') {
        req->version[i++] = **cursor;
        eat_n(cursor, 1);
    }
    req->version[i] = '\0';
}

void get_headers(const char **cursor, Request *req) {
    // TODO: finish me!!!
    // read a thing until we get a ':'
    // eat the space
    // then go until we hit a carige (if we see two then end the thing)
    unused(cursor);
    unused(req);
}

Request parse_req(const char *input) {
    logi("Req: %s", input);
    Request req;

    req.method = parse_method(&input);

    eat_spaces(&input);

    int i = 0;
    while (input[i] != ' ') {
        i++;
    }
    req.path = input;
    req.path_len = i;
    eat_n(&input, i);
    eat_spaces(&input);
    get_version(&input, &req);

    // TODO: end the program early if
    // we see doulble return (no flags)
    eat_spaces(&input);

    get_headers(&input, &req);

    // parse body if it is there

    return req;
}

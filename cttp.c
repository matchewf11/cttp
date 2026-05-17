#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "cttp.h"

#define LOG_ERROR 0
#define LOG_WARN 1
#define LOG_INFO 2

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_INFO
#endif

#define todo() exit(1)
#define unreachable() exit(1)
#define unused(v) (void)(v)

#if LOG_LEVEL <= LOG_WARN
#define logw(...) log_warn(__VA_ARGS__)
#else
#define logw(...)
#endif

#if LOG_LEVEL <= LOG_INFO
#define logi(...) log_info(__VA_ARGS__)
#else
#define logi(...)
#endif

#if LOG_LEVEL <= LOG_ERROR
#define loge(...) log_error(__VA_ARGS__)
#else
#define loge(...)
#endif

static inline void log_error(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[ERROR] ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

static inline void log_warn(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[WARN] ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

static inline void log_info(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf("[INFO] ");
    vprintf(fmt, ap);
    printf("\n");
    va_end(ap);
}

int compare_req(const Request *a, const Request *b) {
    if (a->method != b->method) {
        return a->method - b->method;
    }
    if (a->path_len != b->path_len) {
        return a->path_len - b->path_len;
    }

    int cmp = strcmp(a->version, b->version);
    if (cmp != 0) {
        return cmp;
    }

    return strncmp(a->path, b->path, a->path_len);
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

void eat_n(const char **cursor, int n) {
    *cursor += n;
}

void eat_spaces(const char **cursor) {
    while (**cursor == ' ') {
        eat_n(cursor, 1);
    }
}

Method parse_method(const char **cursor) {
    if (strncmp(*cursor, "GET", 3) == 0) {
        eat_n(cursor, 3);
        return Get;
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

    // then eat the headers
    // ends with two \r\n

    return req;
}

int run_server() {
    logi("trying to open socket");
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        printf("failed to make socket\n");
        return 1;
    }
    logi("socket opened");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = htonl(INADDR_ANY);

    logi("attemping socket bind");
    if (bind(socketfd, (const struct sockaddr*) &address, sizeof(address)) != 0) {
        printf("failed to bind socket\n");
        return 1;
    }
    logi("socket bound");

    logi("attempting to listen");
    if (listen(socketfd, 0) != 0) {
        printf("failed to listen\n");
        return 1;
    }
    logi("starting to listen");

    while (1) {
        logi("waiting");
        int client = accept(socketfd, NULL, NULL);
        printf("accepted");

        logi("read req");
        char buffer[1024];
        int rec_res = recv(client, buffer, sizeof(buffer), 0);
        logi("done reading req");

        switch (rec_res) {
            case 0:
            case -1:
                printf("Rec error or done\n");
                break;
            default:
                buffer[rec_res] = '\0';
                parse_req(buffer);
        }

        char *msg =
            "HTTP/1.1 200 OK\r\n"
            "\r\n"
            "OK";

        logi("attemping to send msg");
        send(client, msg, strlen(msg), 0);
        logi("sent msg");

        close(client);
        logi("closed");
    }
}

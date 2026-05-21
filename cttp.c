#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "req.h"
#include "log.h"
#include "comp.h"

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
        recv(client, buffer, sizeof(buffer), 0);
        logi("done reading req");

        Request req = parse_req(buffer);
        unused(req);

        char *msg =
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "\r\n"
            "<h1>hello from c</h1>";
        send(client, msg, strlen(msg), 0);

        close(client);
        logi("closed");
    }
}

#include "test.h"
#include "req.h"

int test_parse_req_2() {
    const char *input =
        "GET / HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/8.20.0\r\n"
        "Accept: */*\r\n"
        "\r\n";

    Request req = parse_req(input);
    Request exp = (Request){
        .method = Get,
        .path = "/",
        .version = "HTTP/1.1",
        .path_len = 1,
        .headers = {
            (Header){
                .name = "Host",
                .name_len = 4,
                .value = "localhost:8080",
                .value_len = 14,
            },
            (Header){
                .name = "User-Agent",
                .name_len = 10,
                .value = "curl/8.20.0",
                .value_len = 11,
            },
            (Header){
                .name = "Accept",
                .name_len = 6,
                .value = "*/*",
                .value_len = 3,
            },
        },
        .header_count = 3,
    };

    if (compare_req(&req, &exp) != 0) {
        return 1;
    }
    return 0;
}

int test_parse_req_1() {
    const char *input =
        "GET /foo HTTP/1.1\r\n"
        "Host: localhost:8080\r\n"
        "User-Agent: curl/8.20.0\r\n"
        "Accept: */*\r\n"
        "\r\n";

    Request req = parse_req(input);
    Request exp = (Request){
        .method = Get,
        .path = "/foo",
        .path_len = 4,
        .version = "HTTP/1.1",
        .headers = {
            (Header){
                .name = "Host",
                .name_len = 4,
                .value = "localhost:8080",
                .value_len = 14,
            },
            (Header){
                .name = "User-Agent",
                .name_len = 10,
                .value = "curl/8.20.0",
                .value_len = 11,
            },
            (Header){
                .name = "Accept",
                .name_len = 6,
                .value = "*/*",
                .value_len = 3,
            },
        },
        .header_count = 3,
    };

    if (compare_req(&req, &exp) != 0) {
        return 1;
    }
    return 0;
}

int main() {
    REG_TEST(test_parse_req_1);
    REG_TEST(test_parse_req_2);
    run_tests();
    return 0;
}

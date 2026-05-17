#include <stdio.h>

#include "cttp.h"

#define REG_TEST(test) reg_test(test, #test)

typedef int test_fn(void);

typedef struct {
    test_fn *test;
    const char *name;
} Test;

static Test tests[128];

static int test_i = 0;
static int passed = 0;
static int failed = 0;

void reg_test(test_fn test, const char *name) {
    tests[test_i++] = (Test){
        .test = test,
        .name = name,
    };
}

void run_tests() {
    printf("Starting Tests...\n");

    for (int i = 0; i < test_i; i++) {
        Test curr = tests[i];
        if (curr.test() == 0) {
            passed++;
            printf("Test '%s' passed\n", curr.name);
        } else {
            failed++;
            printf("Test '%s' failed\n", curr.name);
        }

    }

    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
}

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

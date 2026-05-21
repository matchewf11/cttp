#ifndef TEST_H
#define TEST_H

#include <stdio.h>

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

static inline void reg_test(test_fn test, const char *name) {
    tests[test_i++] = (Test){
        .test = test,
        .name = name,
    };
}

static inline void run_tests() {
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

#endif

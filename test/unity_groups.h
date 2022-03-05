#include <cstdio>

#include "unity.h"

#define TEST_SETUP(group) \
    void test_##group##_setup()

#define TEST_TEARDOWN(group) \
    void test_##group##_teardown()

#define TEST(group, name) \
    void group##_##name()

// This goes at the bottom of each test file
#define TEST_GROUP_RUNNER(group) \
    void test_##group##_group_runner()

// Call this for each test from the group runner
#define TEST_RUN(group, name) \
    test_##group##_setup(); \
    RUN_TEST(group##_##name); \
    test_##group##_teardown()

// Call this from main
#define TEST_RUN_GROUP(group)\
    { \
        void test_##group##_group_runner(); \
        test_##group##_group_runner(); \
    }

#define TEST_PRINT(msg) UnityPrint(msg)
#define TEST_PRINTF(fmt, ...) { \
    char msg[256]; \
    snprintf(msg, sizeof(msg), fmt, __VA_ARGS__); \
    TEST_PRINT(msg); \
}

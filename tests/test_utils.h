/*
test_utils.h - Testing utilities for p5.h library
Provides TEST_ASSERT_TRUE macro and golden image testing functionality
*/

#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Test statistics
static int test_count = 0;
static int test_passed = 0;
static int test_failed = 0;

// Simple TEST_ASSERT_TRUE implementation (based on Unity framework)
#define TEST_ASSERT_TRUE(condition) \
    do { \
        test_count++; \
        if (condition) { \
            test_passed++; \
            printf("PASS: %s:%d - %s\n", __FILE__, __LINE__, #condition); \
        } else { \
            test_failed++; \
            printf("FAIL: %s:%d - Expected TRUE Was FALSE: %s\n", __FILE__, __LINE__, #condition); \
        } \
    } while(0)

#define TEST_ASSERT_FALSE(condition) \
    do { \
        test_count++; \
        if (!(condition)) { \
            test_passed++; \
            printf("PASS: %s:%d - %s\n", __FILE__, __LINE__, #condition); \
        } else { \
            test_failed++; \
            printf("FAIL: %s:%d - Expected FALSE Was TRUE: %s\n", __FILE__, __LINE__, #condition); \
        } \
    } while(0)

// Test result reporting
static void print_test_results(void) {
    printf("\n=== Test Results ===\n");
    printf("Total: %d, Passed: %d, Failed: %d\n", test_count, test_passed, test_failed);
    if (test_failed == 0) {
        printf("ALL TESTS PASSED!\n");
    } else {
        printf("SOME TESTS FAILED!\n");
    }
}

// Image comparison function declarations
bool save_framebuffer_as_png(const char* filename, int width, int height);
bool compare_images(const char* test_image, const char* golden_image);
bool file_exists(const char* filename);

// Test runner macros
#define RUN_TEST(test_func) \
    do { \
        printf("\n--- Running %s ---\n", #test_func); \
        test_func(); \
    } while(0)

#define TEST_RUNNER_START() \
    do { \
        printf("Starting test suite...\n"); \
        test_count = test_passed = test_failed = 0; \
    } while(0)

#define TEST_RUNNER_END() \
    do { \
        print_test_results(); \
        return (test_failed == 0) ? 0 : 1; \
    } while(0)

#endif // TEST_UTILS_H
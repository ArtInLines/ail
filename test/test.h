#ifndef TEST_H_
#define TEST_H_

#include <stdio.h>
#define TEST(func, ...) do { \
        if (func(__VA_ARGS__)) printf("\033[32m" #func "() succesful :)\033[0m\n"); \
        else printf("\033[31m" #func "() failed :)\033[0m\n");                       \
    } while(0)

#define TEST_COMMON_ASSERT(expr, msg) do { if (!(expr)) {                                        \
        printf("  \033[031mFailed assert at " __FILE__ ":" AIL_STR_LINE " " msg "\033[0m\n"); \
        return false;                                                                       \
    } } while(0)

#define TEST_ASSERT_MSG(expr, msg) TEST_COMMON_ASSERT(expr, "with message '" msg "'")

#define TEST_ASSERT(expr) TEST_COMMON_ASSERT(expr, " at expression '" #expr "'")

#endif // TEST_H_

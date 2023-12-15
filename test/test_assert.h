#ifndef TEST_ASSERT_H_
#define TEST_ASSERT_H_

#define COMMON_ASSERT(expr, msg) if (!(expr)) {                                             \
        printf("\033[031mFailed assert at " __FILE__ ":" AIL_STR_LINE " " msg "\033[0m\n"); \
        return false;				                                                        \
    }

#define ASSERT_MSG(expr, msg) COMMON_ASSERT(expr, "with message '" msg "'")

#define ASSERT(expr) COMMON_ASSERT(expr, " at expression '" #expr "'")

#endif // TEST_ASSERT_H_
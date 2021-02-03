#ifndef __ELECTIONGUARD_CPP_TEST_UTILS_H_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_UTILS_H_INCLUDED__

#include <stdbool.h>

static bool strings_are_equal(char *expected, char *actual)
{
    while (*expected == *actual) {
        if (*expected == '\0' || *actual == '\0') {
            break;
        }
        expected++;
        actual++;
    }

    if (*expected == '\0' && *actual == '\0') {
        return true;
    } else {
        return false;
    }
}
#endif /* __ELECTIONGUARD_CPP_TEST_UTILS_H_INCLUDED__ */

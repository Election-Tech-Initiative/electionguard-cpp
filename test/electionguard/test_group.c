
#include "utils/utils.h"

#include <assert.h>
#include <electionguard/constants.h>
#include <electionguard/group.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

static bool test_g_pow_p_with_random(void);

bool test_group(void)
{
    printf("\n -------- test_group.c --------- \n");
    return test_g_pow_p_with_random();
}

bool test_g_pow_p_with_random(void)
{
    printf("\n -------- test_g_pow_p_with_random -------- \n");

    // Arrange
    eg_element_mod_p_t *g = NULL;
    if (eg_element_mod_p_constant_g(&g)) {
        assert(false);
    }

    eg_element_mod_q_t *e = NULL;
    if (eg_element_mod_q_rand_q_new(&e)) {
        assert(false);
    }

    // Act
    eg_element_mod_p_t *actual = NULL;
    if (eg_element_mod_q_pow_mod_p(g, e, &actual)) {
        assert(false);
    }

    // Assert
    // TODO:

    // Clean Up
    eg_element_mod_p_free(actual);
    eg_element_mod_q_free(e);

    return true;
}

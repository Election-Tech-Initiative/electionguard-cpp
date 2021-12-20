#include "utils/utils.h"

#include <assert.h>
#include <electionguard/collections.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

static bool test_can_create_lined_list(void);

bool test_collections(void)
{
    printf("\n -------- test_collections.c --------- \n");
    return test_can_create_lined_list();
}

bool test_can_create_lined_list(void)
{
    printf("\n -------- test_can_create_lined_list --------- \n");

    eg_linked_list_t *list = NULL;
    if (eg_linked_list_new(&list)) {
        assert(false);
    }

    if (eg_linked_list_append(list, "some", "value")) {
        assert(false);
    }

    assert(strings_are_equal(list->head->key, "some") == true);
    assert(strings_are_equal(list->head->value, "value") == true);

    eg_linked_list_free(list);

    return true;
}

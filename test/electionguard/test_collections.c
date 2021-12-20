#include "utils/utils.h"

#include <assert.h>
#include <electionguard/collections.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

static bool test_can_create_linked_list(void);

bool test_collections(void)
{
    printf("\n -------- test_collections.c --------- \n");
    return test_can_create_linked_list();
}

bool test_can_create_linked_list(void)
{
    printf("\n -------- test_can_create_lined_list --------- \n");

    // Arrange

    eg_linked_list_t *list = NULL;
    if (eg_linked_list_new(&list)) {
        assert(false);
    }

    // Act
    if (eg_linked_list_append(list, "some", "value")) {
        assert(false);
    }

    if (eg_linked_list_append(list, "another", "thing")) {
        assert(false);
    }

    char *first_value = NULL;
    if (eg_linked_list_get_value_at(list, 0, &first_value)) {
        assert(false);
    }

    char *second_value = NULL;
    if (eg_linked_list_get_value_at(list, 1, &second_value)) {
        assert(false);
    }

    // Assert
    assert(strings_are_equal(list->head->key, "some") == true);
    assert(strings_are_equal(list->head->value, "value") == true);
    assert(strings_are_equal(list->tail->key, "another") == true);
    assert(strings_are_equal(list->tail->value, "thing") == true);

    assert(strings_are_equal(list->head->value, first_value) == true);
    assert(strings_are_equal(list->tail->value, second_value) == true);

    eg_linked_list_free(list);

    return true;
}

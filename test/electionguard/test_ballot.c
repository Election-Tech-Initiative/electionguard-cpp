#include "utils/utils.h"

#include <assert.h>
#include <electionguard/ballot.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

static bool test_ballot_property_getters(void);
static bool test_ballot_serialization(void);
static bool test_plaintext_ballot_selection_is_valid(void);
static bool test_plaintext_ballot_selection_is_invalid(void);

bool test_ballot(void)
{
    printf("\n -------- test_ballot.c --------- \n");
    return test_ballot_property_getters() && test_ballot_serialization() &&
           test_plaintext_ballot_selection_is_valid() &
             test_plaintext_ballot_selection_is_invalid();
}

bool test_ballot_property_getters()
{
    // Arrange
    char *data =
      "{\"ballot_style\":\"ballot-style-1\",\"contests\":[{\"ballot_selections\":[{\"object_id\":"
      "\"contest-1-selection-1-id\",\"vote\":1},{\"object_id\":\"contest-1-selection-2-id\","
      "\"vote\":0},{\"object_id\":\"contest-1-selection-3-id\",\"vote\":0}],\"object_id\":"
      "\"contest-1-id\"},{\"ballot_selections\":[{\"object_id\":\"contest-2-selection-1-id\","
      "\"vote\":1},{\"object_id\":\"contest-2-selection-2-id\",\"vote\":0}],\"object_id\":"
      "\"contest-2-id\"}],\"object_id\":\"ballot-id-123\"}";

    // hydrate the ballot from json
    eg_plaintext_ballot_t *subject = NULL;
    if (eg_plaintext_ballot_from_json(data, &subject)) {
        assert(false);
    }

    // Act

    // verify the ballot property accessors
    char *ballot_id;
    if (eg_plaintext_ballot_get_object_id(subject, &ballot_id)) {
        assert(false);
    }
    assert(strings_are_equal("ballot-id-123", ballot_id) == true);

    // iterate over the contests in the ballot
    for (size_t i = 0; i < eg_plaintext_ballot_get_contests_size(subject); i++) {

        eg_plaintext_ballot_contest_t *contest = NULL;
        if (eg_plaintext_ballot_get_contest_at_index(subject, i, &contest)) {
            assert(false);
        }

        // verify contest property accessors
        char *contest_id;
        if (eg_plaintext_ballot_contest_get_object_id(contest, &contest_id)) {
            assert(false);
        }

        char expected_contest_id[50];
        sprintf(expected_contest_id, "contest-%lu-id", i + 1);
        assert(strings_are_equal(expected_contest_id, contest_id) == true);

        // iterate over the selections in the contest
        for (size_t j = 0; j < eg_plaintext_ballot_contest_get_selections_size(contest); j++) {
            eg_plaintext_ballot_selection_t *selection = NULL;
            if (eg_plaintext_ballot_contest_get_selection_at_index(contest, j, &selection)) {
                assert(false);
            }

            // verify the selection property accessors
            char *selection_id;
            if (eg_plaintext_ballot_selection_get_object_id(selection, &selection_id)) {
                assert(false);
            }

            char expected_selection_id[50];
            sprintf(expected_selection_id, "contest-%lu-selection-%lu-id", i + 1, j + 1);
            assert(strings_are_equal(expected_selection_id, selection_id) == true);

            free(selection_id);
        }

        free(contest_id);
    }

    // Clean Up
    if (eg_plaintext_ballot_free(subject)) {
        assert(false);
    }

    free(ballot_id);

    return true;
}

bool test_ballot_serialization(void)
{
    // Arrange
    char *json =
      "{\"ballot_style\":\"ballot-style-1\",\"contests\":[{\"ballot_selections\":[{\"is_"
      "placeholder_selection\":false,\"object_id\":\"contest-1-selection-1-id\",\"vote\":1},{\"is_"
      "placeholder_selection\":false,\"object_id\":\"contest-1-selection-2-id\",\"vote\":0},{\"is_"
      "placeholder_selection\":false,\"object_id\":\"contest-1-selection-3-id\",\"vote\":0}],"
      "\"object_id\":\"contest-1-id\"},{\"ballot_selections\":[{\"is_placeholder_selection\":false,"
      "\"object_id\":\"contest-2-selection-1-id\",\"vote\":1},{\"is_placeholder_selection\":false,"
      "\"object_id\":\"contest-2-selection-2-id\",\"vote\":0}],\"object_id\":\"contest-2-id\"}],"
      "\"object_id\":\"ballot-id-123\"}";

    // Act
    eg_plaintext_ballot_t *fromJson = NULL;
    if (eg_plaintext_ballot_from_json(json, &fromJson)) {
        assert(false);
    }

    char *derived;
    size_t size;
    if (eg_plaintext_ballot_to_json(fromJson, &derived, &size)) {
        assert(false);
    }

    assert(size > 0);

    // Assert
    assert(strings_are_equal(json, derived) == true);

    // Clean Up
    if (eg_plaintext_ballot_free(fromJson)) {
        assert(false);
    }

    free(derived);
    return true;
}

bool test_plaintext_ballot_selection_is_valid(void)
{
    // Arrange
    char *object_id = "some-object-id";

    // Act
    eg_plaintext_ballot_selection_t *subject = NULL;
    if (eg_plaintext_ballot_selection_new(object_id, 1UL, false, &subject)) {
        assert(false);
    }

    // Assert
    assert(eg_plaintext_ballot_selection_is_valid(subject, object_id) == true);

    // clean up
    eg_plaintext_ballot_selection_free(subject);
    return true;
}

bool test_plaintext_ballot_selection_is_invalid(void)
{
    bool different_object_ids_fail(void);
    bool vote_out_of_range_fails(void);

    return different_object_ids_fail() && vote_out_of_range_fails();
}

bool different_object_ids_fail()
{
    // Arrange
    char *object_id = "some-object-id";

    // Act
    eg_plaintext_ballot_selection_t *subject = NULL;
    if (eg_plaintext_ballot_selection_new(object_id, 1UL, false, &subject)) {
        assert(false);
    }

    // Assert
    assert(eg_plaintext_ballot_selection_is_valid(subject, "some-other-object-id") == false);

    // Clean up
    eg_plaintext_ballot_selection_free(subject);
    return true;
}

bool vote_out_of_range_fails()
{
    // Arrange
    char *object_id = "some-object-id";

    // Act
    eg_plaintext_ballot_selection_t *subject = NULL;
    if (eg_plaintext_ballot_selection_new(object_id, 2UL, false, &subject)) {
        assert(false);
    }

    // Assert
    assert(eg_plaintext_ballot_selection_is_valid(subject, object_id) == false);

    // Clean up
    eg_plaintext_ballot_selection_free(subject);
    return true;
}

#include "generators/ballot.h"
#include "generators/election.h"
#include "generators/manifest.h"
#include "utils/utils.h"

#include <assert.h>
#include <electionguard/election.h>
#include <stdlib.h>

bool strings_are_equal(char *expected, char *actual);

static bool test_assign_extended_data_to_ciphertext_election_context(void);

bool test_election(void)
{
    printf("\n -------- test_election.c --------- \n");
    return test_assign_extended_data_to_ciphertext_election_context();
}

bool test_assign_extended_data_to_ciphertext_election_context()
{
    printf("\n -------- test_assign_extended_data_to_ciphertext_election_context --------- \n");

    // Arrange

    eg_linked_list_t *extended_data = NULL;
    if (eg_linked_list_new(&extended_data)) {
        assert(false);
    }

    if (eg_linked_list_append(extended_data, "ballot_base_uri", "http://something.vote/")) {
        assert(false);
    }

    eg_element_mod_p_t *one_p = NULL;
    if (eg_element_mod_p_new(ONE_MOD_P_ARRAY, &one_p)) {
        assert(false);
    }

    eg_element_mod_q_t *one_q = NULL;
    if (eg_element_mod_q_new(ONE_MOD_Q_ARRAY, &one_q)) {
        assert(false);
    }

    // Act
    eg_ciphertext_election_context_t *context = NULL;
    if (eg_ciphertext_election_context_make_with_extended_data(3UL, 2UL, one_p, one_q, one_q,
                                                               extended_data, &context)) {
        assert(false);
    }

    eg_linked_list_t *cached = NULL;
    if (eg_ciphertext_election_context_get_extended_data(context, &cached)) {
        assert(false);
    }
    eg_linked_list_node_t *resolved = cached->head;

    // Assert

    assert(strings_are_equal(extended_data->head->value, resolved->value) == true);

    // Cleanup

    eg_ciphertext_election_context_free(context);
    eg_linked_list_free(cached);
    eg_linked_list_free(extended_data);
    eg_element_mod_p_free(one_p);
    eg_element_mod_q_free(one_q);

    return true;
}

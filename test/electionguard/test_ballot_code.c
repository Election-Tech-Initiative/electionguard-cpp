#include <assert.h>
#include <electionguard/ballot_code.h>

static bool test_ballot_code_rotates(void);

bool test_ballot_code(void)
{
    printf("\n -------- test_ballot_code.c --------- \n");
    return test_ballot_code_rotates();
}

bool test_ballot_code_rotates(void)
{
    printf("\n -------- test_ballot_code_rotates --------- \n");
    // Arrange
    eg_element_mod_q_t *device_hash = NULL;
    if (eg_ballot_code_get_hash_for_device(12345UL, 23456UL, 34567UL, "some-location-string",
                                           &device_hash)) {
        assert(false);
    }

    uint64_t first_hash[4] = {1, 2, 3, 4};
    eg_element_mod_q_t *first_ballot_hash = NULL;
    if (eg_element_mod_q_new(first_hash, &first_ballot_hash)) {
        assert(false);
    }

    uint64_t second_hash[4] = {2, 3, 4, 5};
    eg_element_mod_q_t *second_ballot_hash = NULL;
    if (eg_element_mod_q_new(second_hash, &second_ballot_hash)) {
        assert(false);
    }

    // Act
    eg_element_mod_q_t *rotating_hash_1 = NULL;
    if (eg_ballot_code_get_ballot_code(device_hash, 1000, first_ballot_hash, &rotating_hash_1)) {
        assert(false);
    }
    eg_element_mod_q_t *rotating_hash_2 = NULL;
    if (eg_ballot_code_get_ballot_code(rotating_hash_1, 1001, second_ballot_hash,
                                       &rotating_hash_2)) {
        assert(false);
    }

    // Assert
    assert(device_hash != NULL);
    assert(rotating_hash_1 != NULL);
    assert(rotating_hash_2 != NULL);

    uint64_t *device_hash_data = NULL;
    uint64_t device_hash_size;
    if (eg_element_mod_q_get_data(device_hash, &device_hash_data, &device_hash_size)) {
        assert(false);
    }

    uint64_t *rotating_hash_1_data = NULL;
    uint64_t rotating_hash_1_size;
    if (eg_element_mod_q_get_data(rotating_hash_1, &rotating_hash_1_data, &rotating_hash_1_size)) {
        assert(false);
    }

    uint64_t *rotating_hash_2_data = NULL;
    uint64_t rotating_hash_2_size;
    if (eg_element_mod_q_get_data(rotating_hash_2, &rotating_hash_2_data, &rotating_hash_2_size)) {
        assert(false);
    }

    assert(device_hash_data != NULL);
    assert(rotating_hash_1_data != NULL);
    assert(rotating_hash_2_data != NULL);

    assert(rotating_hash_1_data != device_hash_data);
    assert(rotating_hash_2_data != device_hash_data);
    assert(rotating_hash_1_data != rotating_hash_2_data);

    assert(rotating_hash_1_data[0] != device_hash_data[0]);

    // Clean Up
    if (eg_element_mod_q_free(first_ballot_hash)) {
        assert(false);
    }

    if (eg_element_mod_q_free(second_ballot_hash)) {
        assert(false);
    }

    if (eg_element_mod_q_free(rotating_hash_1)) {
        assert(false);
    }

    if (eg_element_mod_q_free(rotating_hash_2)) {
        assert(false);
    }

    if (eg_element_mod_q_free(device_hash)) {
        assert(false);
    }

    device_hash_data = NULL;
    rotating_hash_1_data = NULL;
    rotating_hash_2_data = NULL;

    return true;
}

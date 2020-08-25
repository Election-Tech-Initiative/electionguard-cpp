#include <assert.h>
#include <electionguard/ballot.h>
#include <electionguard/encrypt.h>
#include <electionguard/group.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool strings_are_equal(char *expected, char *actual);

int main()
{
    // insantiate the stateful mediator
    eg_encryption_mediator_t *encrypter = eg_encryption_mediator_new();
    int instance_encrypt = eg_encryption_mediator_encrypt(encrypter);
    assert(instance_encrypt == 9);

    // instantiate a selection on a ballot
    eg_plaintext_ballot_selection_t *plaintext =
      eg_plaintext_ballot_selection_new("some-object-id", "1");

    eg_ciphertext_ballot_selection_t *ciphertext;

    int function_encrypt = eg_encrypt_selection(plaintext, &ciphertext);
    assert(function_encrypt == 1); // arbitrarily returns one for success

    // check the object id by accessing the property getter
    char *plaintext_object_id = eg_plaintext_ballot_selection_get_object_id(plaintext);
    char *ciphertext_object_id = eg_ciphertext_ballot_selection_get_object_id(ciphertext);
    assert(strings_are_equal(plaintext_object_id, ciphertext_object_id) == true);

    // get out one of the ElementModQ hashes
    eg_element_mod_q_t *description_hash =
      eg_ciphertext_ballot_selection_get_description_hash(ciphertext);

    uint64_t *description_hash_data;
    uint8_t size = eg_element_mod_q_get(description_hash, &description_hash_data);

    assert(size == 4);
    // the current test just arbitrarily assigns the vote to the hash
    assert(description_hash_data[0] == 1);

    eg_encryption_mediator_free(encrypter);
    eg_plaintext_ballot_selection_free(plaintext);
    eg_ciphertext_ballot_selection_free(ciphertext);
}

bool strings_are_equal(char *expected, char *actual)
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
        printf("compare_string: failed!\n");
        return false;
    }
}
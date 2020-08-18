#include <assert.h>
#include <electionguard/ballot.h>
#include <electionguard/encrypt.h>

int main()
{
    eg_encryption_mediator_t *encrypter = eg_encryption_mediator_new();
    int instance_encrypt = eg_encryption_mediator_encrypt(encrypter);
    assert(instance_encrypt == 9);

    eg_plaintext_ballot_selection_t *selection =
      eg_plaintext_ballot_selection_new("some-object-id", "1");

    int function_encrypt = eg_encrypt_selection(selection);
    assert(function_encrypt == 1);

    eg_encryption_mediator_free(encrypter);
    eg_plaintext_ballot_selection_free(selection);
}
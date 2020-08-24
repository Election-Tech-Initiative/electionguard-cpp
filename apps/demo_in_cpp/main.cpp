#include <assert.h>
#include <electionguard/ballot.h>
#include <electionguard/ballot.hpp>
#include <electionguard/encrypt.h>
#include <electionguard/encrypt.hpp>

using namespace electionguard;

int main()
{
    // Test the C API
    eg_encryption_mediator_t *encrypter = eg_encryption_mediator_new();
    int instance_encrypt = eg_encryption_mediator_encrypt(encrypter);
    assert(instance_encrypt == 9);

    eg_plaintext_ballot_selection_t *selection =
      eg_plaintext_ballot_selection_new("some-object-id", "1");

    eg_ciphertext_ballot_selection_t *ciphertext;
    int function_encrypt = eg_encrypt_selection(selection, &ciphertext);
    assert(function_encrypt == 1);

    eg_encryption_mediator_free(encrypter);
    eg_plaintext_ballot_selection_free(selection);

    // Test the C++ API
    auto *cpp_encrypter = new EncryptionMediator();
    auto cpp_instance_encrypt = cpp_encrypter->encrypt();
    assert(cpp_instance_encrypt == 9);

    auto *cpp_plaintext = new PlaintextBallotSelection("some-unique-id", "1");
    auto *cpp_ciphertext = encrypt_selection(cpp_plaintext);

    auto *cpp_result = cpp_ciphertext->getDescriptionHash()->get();
    assert(cpp_result[0] == 1);

    delete cpp_encrypter;
    delete selection;
}
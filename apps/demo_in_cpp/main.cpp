#include <assert.h>
#include <electionguard/encrypt.h>
#include <electionguard/encrypt.hpp>

using namespace electionguard;

int main()
{
    // Test the C API
    eg_encryption_compositor_t *encrypter = eg_encryption_compositor_new();
    int instance_encrypt = eg_encryption_compositor_encrypt(encrypter);
    assert(instance_encrypt == 9);

    int function_encrypt = eg_encrypt_ballot();
    assert(function_encrypt == 8);

    eg_encryption_compositor_free(encrypter);

    // Test the C++ API
    auto cpp_encrypter = new electionguard::EncryptionCompositor();
    auto cpp_instance_encrypt = cpp_encrypter->encrypt();
    assert(cpp_instance_encrypt == 9);

    auto cpp_function_encrypt = electionguard::encrypt_ballot();
    assert(cpp_function_encrypt == 8);

    delete cpp_encrypter;
}
#include <assert.h>
#include <electionguard/encrypt.h>

int main()
{
    eg_encryption_compositor_t *encrypter = eg_encryption_compositor_new();
    int instance_encrypt = eg_encryption_compositor_encrypt(encrypter);
    assert(instance_encrypt == 9);

    int function_encrypt = eg_encrypt_ballot();
    assert(function_encrypt == 8);

    eg_encryption_compositor_free(encrypter);
}
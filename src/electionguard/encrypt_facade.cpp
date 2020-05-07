#include "electionguard/encrypt.hpp"

extern "C" {
    #include "electionguard/encrypt.h"
}

#define AS_TYPE(Type, Obj) reinterpret_cast<Type *>(Obj)
#define AS_CTYPE(Type, Obj) reinterpret_cast<const Type *>(Obj)

eg_encryption_compositor_t *eg_encryption_compositor_new()
{
    return AS_TYPE(eg_encryption_compositor_t, new electionguard::EncryptionCompositor());
}

void eg_encryption_compositor_free(eg_encryption_compositor_t *composer)
{
    if (!composer)
        return;
    delete AS_TYPE(electionguard::EncryptionCompositor, composer);
}

int eg_encryption_compositor_encrypt(eg_encryption_compositor_t *composer)
{
    return AS_TYPE(electionguard::EncryptionCompositor, composer)->encrypt();
}

int eg_encrypt_ballot()
{
    return electionguard::encrypt_ballot();
}

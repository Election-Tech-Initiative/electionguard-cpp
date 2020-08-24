#include "electionguard/encrypt.hpp"

#include "electionguard/elgamal.hpp"
#include "hash.hpp"

#include <iostream>

extern "C" {
#include "../kremlin/Hacl_Bignum4096.h"
}

namespace electionguard
{
    EncryptionMediator::EncryptionMediator()
    {
        std::cout << __func__ << " : Creating EncryptionCompositor[" << this << "]" << std::endl;
    }
    EncryptionMediator::~EncryptionMediator()
    {
        std::cout << __func__ << " : Destroying EncryptionCompositor[" << this << "]" << std::endl;
    }

    int EncryptionMediator::encrypt()
    {
        std::cout << __func__ << " : encrypting by instance " << std::endl;
        return 9;
    }

    CiphertextBallotSelection *encrypt_selection(PlaintextBallotSelection *selection)
    {
        // TESTING: just putting the plaintext value in the hash for now
        uint64_t selection_as_int = selection->toInt();
        uint64_t hash_rep[4] = {};
        hash_rep[0] = selection_as_int;
        // TODO: Safety
        auto *descriptionHash = new ElementModQ(hash_rep);
        auto *fakePublicKey = new ElementModP(hash_rep);
        auto ciphertext = elgamalEncrypt(selection_as_int, descriptionHash, fakePublicKey);
        if (ciphertext->getPad()->get() != nullptr) {
            // just bypass compiler error
        }

        return new CiphertextBallotSelection(selection->getObjectId(), descriptionHash);
    }

} // namespace electionguard

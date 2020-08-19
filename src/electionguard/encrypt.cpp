#include "electionguard/encrypt.hpp"

#include "electionguard/elgamal.hpp"
#include "hash.hpp"

#include <assert.h>
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
        uint64_t one[64] = {1};
        std::cout << __func__ << " : encrypting by instance " << std::endl;
        auto result = hash_elems("some string");
        if (result == 5) {
        }

        uint64_t res[64] = {};

        uint64_t carry = Hacl_Bignum4096_add(one, one, res);
        if (carry > 0) {
        }

        assert(res[0] == 2);

        return 9;
    }

    CiphertextBallotSelection *encrypt_selection(PlaintextBallotSelection *selection)
    {
        std::cout << __func__ << " : encrypting by function " << std::endl;
        // TESTING: just putting the plaintext value in the hash for now
        uint64_t selection_as_int = selection->toInt();
        uint64_t hash_rep[4] = {};
        hash_rep[0] = selection_as_int;
        auto descriptionHash = new ElementModQ(hash_rep);
        auto fakePublicKey = new ElementModP(hash_rep);
        auto ciphertext = elgamalEncrypt(selection_as_int, descriptionHash, fakePublicKey);
        if (ciphertext->getPad()->get() != nullptr) {
            // just bypass compiler error
        }
        return new CiphertextBallotSelection(selection->getObjectId(), descriptionHash);
    }

} // namespace electionguard

#include "electionguard/encrypt.hpp"

#include "hash.hpp"

#include <assert.h>
#include <iostream>

uint64_t one[64] = {1};

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

    int encrypt_selection(PlaintextBallotSelection *selection)
    {
        std::cout << __func__ << " : encrypting by function " << std::endl;
        return selection->toInt();
    }

} // namespace electionguard

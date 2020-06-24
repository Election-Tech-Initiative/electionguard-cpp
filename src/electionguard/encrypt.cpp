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
    EncryptionCompositor::EncryptionCompositor()
    {
        std::cout << __func__ << " : Creating EncryptionCompositor[" << this
                  << "]" << std::endl;
    }
    EncryptionCompositor::~EncryptionCompositor()
    {
        std::cout << __func__ << " : Destroying EncryptionCompositor[" << this
                  << "]" << std::endl;
    }

    int EncryptionCompositor::encrypt()
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

    int encrypt_ballot()
    {
        std::cout << __func__ << " : encrypting by function " << std::endl;
        return 8;
    }

} // namespace electionguard

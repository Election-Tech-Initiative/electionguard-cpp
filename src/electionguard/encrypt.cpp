#include "electionguard/encrypt.hpp"

#include "hash.hpp"

#include <iostream>

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
        return 9;
    }

    int encrypt_ballot()
    {
        std::cout << __func__ << " : encrypting by function " << std::endl;
        return 8;
    }

} // namespace electionguard

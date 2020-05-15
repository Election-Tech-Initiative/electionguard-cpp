#ifndef __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__

#include "export.h"

namespace electionguard
{
    class EG_API EncryptionCompositor
    {
      public:
        EncryptionCompositor();
        ~EncryptionCompositor();

        int encrypt();
    };

    EG_API int encrypt_ballot();
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__ */
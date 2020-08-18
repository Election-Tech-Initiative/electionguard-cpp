#ifndef __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__

#include "electionguard/ballot.hpp"
#include "export.h"

namespace electionguard
{
    class EG_API EncryptionMediator
    {
      public:
        EncryptionMediator();
        ~EncryptionMediator();

        int encrypt();
    };

    EG_API int encrypt_selection(PlaintextBallotSelection *selection);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__ */
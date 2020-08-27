#ifndef __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__

#include "electionguard/ballot.hpp"
#include "electionguard/election.hpp"
#include "electionguard/group.hpp"
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

    /// <summary>
    /// Encrypt a specific `BallotSelection` in the context of a specific `BallotContest`
    ///
    /// <param name="selection">the selection in the valid input form</param>
    /// <returns>A `CiphertextBallotSelection`</returns>
    /// </summary>
    EG_API CiphertextBallotSelection *
    encrypt_selection(PlaintextBallotSelection *selection, SelectionDescription *description,
                      ElementModP *elgamal_public_key, ElementModQ *crypto_extended_base_hash,
                      ElementModQ *nonce_seed, bool is_placeholder, bool should_verify_proofs);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__ */
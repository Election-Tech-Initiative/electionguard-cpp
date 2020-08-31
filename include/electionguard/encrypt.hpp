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
    /// <param name="plaintext">the selection in the valid input form</param>
    /// <param name="description">the `SelectionDescription` from the `ContestDescription`
    ///                           which defines this selection's structure</param>
    /// <param name="elgamal_public_key">the public key (K) used to encrypt the ballot</param>
    /// <param name="crypto_extended_base_hash">the extended base hash of the election</param>
    /// <param name="nonce_seed">an `ElementModQ` used as a header to seed the `Nonce` generated
    ///                          for this selection. this value can be (or derived from) the
    ///                          BallotContest nonce, but no relationship is required</param>
    /// <param name="is_placeholder">specifies if this is a placeholder selection</param>
    /// <param name="should_verify_proofs">specify if the proofs should be verified prior to returning (default True)</param>
    /// <returns>A `CiphertextBallotSelection`</returns>
    /// </summary>
    EG_API CiphertextBallotSelection *
    encrypt_selection(PlaintextBallotSelection *plaintext, SelectionDescription *description,
                      ElementModP *elgamal_public_key, ElementModQ *crypto_extended_base_hash,
                      ElementModQ *nonce_seed, bool is_placeholder, bool should_verify_proofs);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_ENCRYPT_HPP_INCLUDED__ */
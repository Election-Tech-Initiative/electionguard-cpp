#ifndef __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__

#include "elgamal.hpp"
#include "export.h"
#include "group.hpp"

extern "C" {
#include "election_object_base.h"
}

#include <string>

namespace electionguard
{
    struct PlaintextBallotSelectionData : public ElectionObjectBase {
        char vote[256];
        bool isPlaceholderSelection;
    };

    /// <summary>
    /// A BallotSelection represents an individual selection on a ballot.
    ///
    /// This class accepts a `vote` string field which has no constraints
    /// in the ElectionGuard Data Specification, but is constrained logically
    /// in the application to resolve to `True` or `False`.  This implies that the
    /// data specification supports passing any string that can be represented as
    /// an integer, however only 0 and 1 is supported for now.
    ///
    /// This class can also be designated as `is_placeholder_selection` which has no
    /// context to the data specification but is useful for running validity checks internally
    ///
    /// an `extended_data` field exists to support any arbitrary data to be associated
    /// with the selection.  In practice, this field is the cleartext representation
    /// of a write-in candidate value.  In the current implementation these values are
    /// discarded when encrypting.
    /// </summary>
    class EG_API PlaintextBallotSelection
    {
      public:
        PlaintextBallotSelection(const std::string object_id, std::string vote);
        PlaintextBallotSelection(const char *object_id, char *vote);
        ~PlaintextBallotSelection();

        int toInt();

        char *getObjectId();

      private:
        PlaintextBallotSelectionData data;
    };

    struct CiphertextBallotSelectionData : public ElectionObjectBase {
        ElementModQ *descriptionHash;
        ElGamalCiphertext *ciphertext;
        ElementModQ *crypto_hash;
        bool isPlaceholderSelection;
        ElementModQ *nonce;
    };

    /// <summary>
    /// A CiphertextBallotSelection represents an individual encrypted selection on a ballot.
    ///
    ///This class accepts a `description_hash` and a `ciphertext` as required parameters
    ///in its constructor.
    ///
    ///When a selection is encrypted, the `description_hash` and `ciphertext` required fields must
    ///be populated at construction however the `nonce` is also usually provided by convention.
    ///
    ///After construction, the `crypto_hash` field is populated automatically in the `__post_init__` cycle
    ///
    ///A consumer of this object has the option to discard the `nonce` and/or discard the `proof`,
    ///or keep both values.
    ///
    ///By discarding the `nonce`, the encrypted representation and `proof`
    /// can only be regenerated if the nonce was derived from the ballot's master nonce.  If the nonce
    ///used for this selection is truly random, and it is discarded, then the proofs cannot be regenerated.
    ///
    ///By keeping the `nonce`, or deriving the selection nonce from the ballot nonce, an external system can
    ///regenerate the proofs on demand.  This is useful for storage or memory constrained systems.
    ///
    ///By keeping the `proof` the nonce is not required fotor verify the encrypted selection.
    /// </summary>
    class EG_API CiphertextBallotSelection
    {
      public:
        CiphertextBallotSelection(const std::string object_id, ElementModQ *descriptionHash);
        CiphertextBallotSelection(const char *object_id, ElementModQ *descriptionHash);
        ~CiphertextBallotSelection();

        char *getObjectId();

        /// <summary>
        /// The SelectionDescription hash
        /// </summary>
        ElementModQ *getDescriptionHash();

      private:
        CiphertextBallotSelectionData data;
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__ */
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

    class EG_API CiphertextBallotSelection
    {
      public:
        CiphertextBallotSelection(const std::string object_id, ElementModQ *descriptionHash);
        CiphertextBallotSelection(const char *object_id, ElementModQ *descriptionHash);
        ~CiphertextBallotSelection();

        char *getObjectId();
        ElementModQ *getDescriptionHash();

      private:
        CiphertextBallotSelectionData data;
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_BALLOT_HPP_INCLUDED__ */
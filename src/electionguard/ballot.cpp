#include "electionguard/ballot.hpp"

#include <stdlib.h>
using namespace std;
namespace electionguard
{
    PlaintextBallotSelection::PlaintextBallotSelection(const string object_id, string vote) : data()
    {
        // TODO: safety
        auto _osize = object_id.size() + 1;
        object_id.copy(data.object_id, _osize);

        auto _vsize = vote.size() + 1;
        vote.copy(data.vote, _vsize);
    }

    PlaintextBallotSelection::PlaintextBallotSelection(const char *object_id, char *vote) : data()
    {
        // TODO: safety
        strcpy(data.object_id, object_id);
        strcpy(data.vote, vote);
    }

    PlaintextBallotSelection::~PlaintextBallotSelection() { data = {}; }

    int PlaintextBallotSelection::toInt() { return atoi(data.vote); }

    char *PlaintextBallotSelection::getObjectId() { return data.object_id; }

    CiphertextBallotSelection::CiphertextBallotSelection(const std::string object_id,
                                                         ElementModQ *descriptionHash)
        : data()
    {
        auto _osize = object_id.size() + 1;
        object_id.copy(data.object_id, _osize);
        data.descriptionHash = descriptionHash;
    }

    CiphertextBallotSelection::CiphertextBallotSelection(const char *object_id,
                                                         ElementModQ *descriptionHash)
        : data()
    {
        strcpy(data.object_id, object_id);
        data.descriptionHash = descriptionHash;
    }

    CiphertextBallotSelection::~CiphertextBallotSelection() { data = {}; }

    char *CiphertextBallotSelection::getObjectId() { return data.object_id; }

    ElementModQ *CiphertextBallotSelection::getDescriptionHash() { return data.descriptionHash; }

} // namespace electionguard
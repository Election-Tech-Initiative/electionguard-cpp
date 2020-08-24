#include "electionguard/ballot.hpp"

#include <cstdlib>
using std::string;
namespace electionguard
{
    PlaintextBallotSelection::PlaintextBallotSelection(string object_id, const string vote) : data()
    {
        auto _osize = object_id.size() + 1;
        object_id.copy(data.object_id, _osize, 0UL);

        auto _vsize = vote.size() + 1;
        vote.copy(data.vote, _vsize, 0UL);
    }

    PlaintextBallotSelection::PlaintextBallotSelection(const char *object_id, char *vote) : data()
    {
        size_t _osize = strlen(object_id);
        strlcpy(data.object_id, object_id, _osize);

        size_t _vsize = strlen(vote);
        strlcpy(data.vote, vote, _vsize);
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
        size_t _osize = strlen(object_id);
        strlcpy(data.object_id, object_id, _osize);
        data.descriptionHash = descriptionHash;
    }

    CiphertextBallotSelection::~CiphertextBallotSelection() { data = {}; }

    char *CiphertextBallotSelection::getObjectId() { return data.object_id; }

    ElementModQ *CiphertextBallotSelection::getDescriptionHash() { return data.descriptionHash; }

} // namespace electionguard
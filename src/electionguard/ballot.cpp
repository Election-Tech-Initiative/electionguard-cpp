#include "electionguard/ballot.hpp"

#include <stdlib.h>
using namespace std;
namespace electionguard
{
    PlaintextBallotSelection::PlaintextBallotSelection(const string object_id, string vote) : data()
    {
        auto _osize = object_id.size() + 1;
        // char _object_id[_osize];
        object_id.copy(data.object_id, _osize);

        auto _vsize = vote.size() + 1;
        // char _vote[_vsize];
        vote.copy(data.vote, _vsize);

        // const static PlaintextBallotSelectionData _data = {_object_id, _vote, false};
        // data = _data;
    };

    PlaintextBallotSelection::PlaintextBallotSelection(const char *object_id, char *vote) : data()
    {
        // TODO: any sort of safety
        // size_t _osize = strlen(object_id);
        // size_t _vsize = strlen(vote);

        strcpy(data.object_id, object_id);
        strcpy(data.vote, vote);

        // const static PlaintextBallotSelectionData _data = {_object_id, _vote, false};
        // data = _data;
    };

    PlaintextBallotSelection::~PlaintextBallotSelection() { data = {}; };

    int PlaintextBallotSelection::toInt() { return atoi(data.vote); }
} // namespace electionguard
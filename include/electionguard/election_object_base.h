#ifndef __ELECTIONGUARD_CORE_ELECTION_OBJECT_BASE_H_INCLUDED__
#define __ELECTIONGUARD_CORE_ELECTION_OBJECT_BASE_H_INCLUDED__

#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

struct ElectionObjectBase {
    char object_id[MAX_IDENIFIER_LENGTH];
};

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CORE_ELECTION_OBJECT_BASE_H_INCLUDED__ */
#ifndef __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__
#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <string>

using namespace std;

extern "C" {
#include "../kremlin/Hacl_Bignum4096.h"
}

namespace electionguard
{
    EG_INTERNAL_API ElementModQ *hash_elems(string const &a);
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__ */

#ifndef __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__
#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <string>
#include <vector>

using namespace std;

extern "C" {
#include "../kremlin/Hacl_Bignum4096.h"
}

namespace electionguard
{
    class CryptoHashable
    {
      public:
        ElementModQ *crypto_hash();
    };

    class CryptoHashCheckable
    {
      public:
        ElementModQ *crypto_hash_with(ElementModQ *seed_hash);
    };

    EG_INTERNAL_API ElementModQ *hash_elems();

    EG_INTERNAL_API ElementModQ *hash_elems(nullptr_t p);

    EG_INTERNAL_API ElementModQ *hash_elems(vector<CryptoHashable> v);

    EG_INTERNAL_API ElementModQ *hash_elems(vector<ElementModP> v);

    EG_INTERNAL_API ElementModQ *hash_elems(vector<ElementModQ> v);

    EG_INTERNAL_API ElementModQ *hash_elems(vector<uint64_t> v);

    EG_INTERNAL_API ElementModQ *hash_elems(CryptoHashable *a);

    EG_INTERNAL_API ElementModQ *hash_elems(ElementModP *a);

    EG_INTERNAL_API ElementModQ *hash_elems(ElementModQ *a);

    EG_INTERNAL_API ElementModQ *hash_elems(uint64_t const &a);

    EG_INTERNAL_API ElementModQ *hash_elems(string const &a);
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__ */

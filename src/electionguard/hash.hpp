#ifndef __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__
#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <initializer_list>
#include <string>
#include <variant>
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
        virtual ElementModQ *crypto_hash() { return nullptr; };
    };

    class CryptoHashCheckable
    {
      public:
        virtual ElementModQ *crypto_hash_with(ElementModQ *seed_hash) { return nullptr; };
    };

    using CryptoHashableType =
      variant<nullptr_t, CryptoHashable *, ElementModP *, ElementModQ *, uint64_t, string,
              vector<CryptoHashable *>, vector<ElementModP *>, vector<ElementModQ *>,
              vector<uint64_t>, vector<string>>;

    // TODO: figure out Fold Expressions for Variadic Templates  (iterating with vectors for now)
    // template <typename... Args> ElementModQ *hash_elems(CryptoHashableType a, Args... args);
    EG_INTERNAL_API ElementModQ *hash_elems(vector<CryptoHashableType> a);
    EG_INTERNAL_API ElementModQ *hash_elems(CryptoHashableType a);
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_HASH_HPP_INCLUDED__ */

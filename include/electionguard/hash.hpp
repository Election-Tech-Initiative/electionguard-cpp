#ifndef __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__
#include <electionguard/crypto_hashable.hpp>
#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <string>
#include <variant>
#include <vector>

namespace electionguard
{
    using CryptoHashableType =
      variant<nullptr_t, CryptoHashable *, ElementModP *, ElementModQ *, uint64_t, string,
              vector<CryptoHashable *>, vector<ElementModP *>, vector<ElementModQ *>,
              vector<uint64_t>, vector<string>>;

    // TODO: Fold Expressions for Variadic Templates  (iterating with vectors for now)
    // template <typename... Args> ElementModQ *hash_elems(CryptoHashableType a, Args... args);
    EG_API ElementModQ *hash_elems(vector<CryptoHashableType> a);
    EG_API ElementModQ *hash_elems(CryptoHashableType a);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__ */

#ifndef __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__
#include <electionguard/crypto_hashable.hpp>
#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <initializer_list>
#include <string>
#include <variant>
#include <vector>

namespace electionguard
{
    using CryptoHashableType = variant<nullptr_t, CryptoHashable *, ElementModP *, ElementModQ *,
                                       vector<uint64_t>, uint64_t, string>;

    // TODO: Fold Expressions for Variadic Templates  (iterating with initializer_lists for now)
    // template <typename... Args> ElementModQ *hash_elems(CryptoHashableType a, Args... args);
    EG_INTERNAL_API ElementModQ *hash_elems(initializer_list<CryptoHashableType> a);
    EG_INTERNAL_API ElementModQ *hash_elems(CryptoHashableType a);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__ */

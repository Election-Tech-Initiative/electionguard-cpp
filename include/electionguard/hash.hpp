#ifndef __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__
#include <electionguard/crypto_hashable.hpp>
#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace electionguard
{
    using CryptoHashableType =
      variant<nullptr_t, CryptoHashable *, ElementModP *, ElementModQ *,
              reference_wrapper<CryptoHashable>, reference_wrapper<ElementModP>,
              reference_wrapper<ElementModQ>, uint64_t, string, vector<CryptoHashable *>,
              vector<ElementModP *>, vector<ElementModQ *>,
              vector<reference_wrapper<CryptoHashable>>, vector<reference_wrapper<ElementModP>>,
              vector<reference_wrapper<ElementModQ>>, vector<uint64_t>, vector<string>>;

    EG_API unique_ptr<ElementModQ> hash_elems(const vector<CryptoHashableType> &a);
    EG_API unique_ptr<ElementModQ> hash_elems(CryptoHashableType a);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__ */

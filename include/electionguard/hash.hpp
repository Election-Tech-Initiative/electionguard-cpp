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
    using CryptoHashableType = std::variant<
      std::nullptr_t, CryptoHashable *, ElementModP *, ElementModQ *,
      std::reference_wrapper<CryptoHashable>, std::reference_wrapper<ElementModP>,
      std::reference_wrapper<ElementModQ>, std::reference_wrapper<const CryptoHashable>,
      std::reference_wrapper<const ElementModP>, std::reference_wrapper<const ElementModQ>,
      uint64_t, std::string, std::vector<CryptoHashable *>, std::vector<ElementModP *>,
      std::vector<ElementModQ *>, std::vector<std::reference_wrapper<CryptoHashable>>,
      std::vector<std::reference_wrapper<ElementModP>>,
      std::vector<std::reference_wrapper<ElementModQ>>,
      std::vector<std::reference_wrapper<const CryptoHashable>>,
      std::vector<std::reference_wrapper<const ElementModP>>,
      std::vector<std::reference_wrapper<const ElementModQ>>, std::vector<uint64_t>,
      std::vector<std::string>>;

    EG_API std::unique_ptr<ElementModQ> hash_elems(const std::vector<CryptoHashableType> &a);
    EG_API std::unique_ptr<ElementModQ> hash_elems(CryptoHashableType a);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_HASH_HPP_INCLUDED__ */

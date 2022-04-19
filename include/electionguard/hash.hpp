#ifndef __ELECTIONGUARD_CPP_HASH_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_HASH_HPP_INCLUDED__
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

    /// <Summary>
    /// Given zero or more elements, calculate their cryptographic hash
    /// using SHA256. Allowed element types are `ElementModP`, `ElementModQ`,
    /// `str`, or `int`, anything implementing `CryptoHashable`, and vectors
    /// or references of any of those types.

    /// <param name="a"> Zero or more elements of any of the accepted types.</param>
    /// <returns>A cryptographic hash of these elements, concatenated.</returns>
    /// </Summary>
    EG_API std::unique_ptr<ElementModQ> hash_elems(const std::vector<CryptoHashableType> &a);

    /// <Summary>
    /// Given zero or more elements, calculate their cryptographic hash
    /// using SHA256. Allowed element types are `ElementModP`, `ElementModQ`,
    /// `str`, or `int`, anything implementing `CryptoHashable`, and vectors
    /// or references of any of those types.

    /// <param name="a"> Zero or more elements of any of the accepted types.</param>
    /// <returns>A cryptographic hash of these elements, concatenated.</returns>
    /// </Summary>
    EG_API std::unique_ptr<ElementModQ> hash_elems(CryptoHashableType a);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_HASH_HPP_INCLUDED__ */

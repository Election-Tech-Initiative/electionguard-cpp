#ifndef __ELECTIONGUARD_CORE_CRYPTO_HASHABLE_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_CRYPTO_HASHABLE_HPP_INCLUDED__

#include "export.h"
#include "group.hpp"

#include <memory>

namespace electionguard
{
    class EG_API CryptoHashable
    {
      public:
        virtual ~CryptoHashable(){};

        /// <summary>
        /// Generates a hash given the fields on the implementing instance.
        /// </summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash()
        {
            throw "CryptoHashable crypto_hash not implemented";
        };
        virtual std::unique_ptr<ElementModQ> crypto_hash() const
        {
            throw "CryptoHashable const crypto_hash not implemented";
        };
    };

    class EG_API CryptoHashCheckable
    {
      public:
        virtual ~CryptoHashCheckable(){};
        /// <summary>
        /// Generates a hash with a given seed that can be checked later against the seed and class metadata.
        /// </summary>
        virtual std::unique_ptr<ElementModQ> crypto_hash_with(const ElementModQ &seedHash)
        {
            throw "CryptoHashCheckable crypto_hash_with not implemented";
        };

        virtual std::unique_ptr<ElementModQ> crypto_hash_with(const ElementModQ &seedHash) const
        {
            throw "CryptoHashCheckable const crypto_hash_with not implemented";
        };
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_CRYPTO_HASHABLE_HPP_INCLUDED__ */
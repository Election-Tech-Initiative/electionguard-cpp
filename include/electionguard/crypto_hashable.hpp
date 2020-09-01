#ifndef __ELECTIONGUARD_CORE_CRYPTO_HASHABLE_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_CRYPTO_HASHABLE_HPP_INCLUDED__

#include "export.h"
#include "group.hpp"

namespace electionguard
{
    class EG_API CryptoHashable
    {
      public:
        virtual ~CryptoHashable(){};

        /// <summary>
        /// Generates a hash given the fields on the implementing instance.
        /// </summary>
        virtual ElementModQ *crypto_hash() { return nullptr; };
    };

    class EG_API CryptoHashCheckable
    {
      public:
        virtual ~CryptoHashCheckable(){};
        /// <summary>
        /// Generates a hash with a given seed that can be checked later against the seed and class metadata.
        /// </summary>
        virtual ElementModQ *crypto_hash_with(ElementModQ *seed_hash) { return nullptr; };
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_CRYPTO_HASHABLE_HPP_INCLUDED__ */
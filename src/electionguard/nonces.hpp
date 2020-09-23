#ifndef __ELECTIONGUARD__CORE_NONCES_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_NONCES_HPP_INCLUDED__

#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <memory>
#include <variant>
#include <vector>
using namespace std;

namespace electionguard
{
    using NoncesHeaderType = variant<ElementModP *, ElementModQ *, string>;

    class EG_INTERNAL_API Nonces
    {
      public:
        Nonces(const Nonces &other);
        Nonces(const Nonces &&other);
        Nonces(const ElementModQ &seed, const NoncesHeaderType &headers);
        Nonces(const ElementModQ &seed);
        ~Nonces();

        Nonces &operator=(Nonces rhs);
        Nonces &operator=(Nonces &&rhs);

        unique_ptr<ElementModQ> get(uint64_t item);
        unique_ptr<ElementModQ> get(uint64_t item, string headers);
        vector<unique_ptr<ElementModQ>> get(uint64_t startItem, uint64_t count);
        unique_ptr<ElementModQ> next();

      private:
        struct Impl;
        unique_ptr<Impl> pimpl;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_NONCES_HPP_INCLUDED__ */

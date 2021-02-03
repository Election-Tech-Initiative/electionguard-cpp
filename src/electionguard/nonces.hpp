#ifndef __ELECTIONGUARD_CPP_NONCES_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_NONCES_HPP_INCLUDED__

#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <memory>
#include <variant>
#include <vector>

namespace electionguard
{
    using NoncesHeaderType = std::variant<ElementModP *, ElementModQ *, std::string>;

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

        std::unique_ptr<ElementModQ> get(uint64_t item);
        std::unique_ptr<ElementModQ> get(uint64_t item, std::string headers);
        std::vector<std::unique_ptr<ElementModQ>> get(uint64_t startItem, uint64_t count);
        std::unique_ptr<ElementModQ> next();

      private:
        struct Impl;
        std::unique_ptr<Impl> pimpl;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_NONCES_HPP_INCLUDED__ */

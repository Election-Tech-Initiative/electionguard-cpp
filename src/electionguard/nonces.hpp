#ifndef __ELECTIONGUARD__CORE_NONCES_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_NONCES_HPP_INCLUDED__

#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <variant>
#include <vector>
using namespace std;

namespace electionguard
{
    using NoncesHeaderType = variant<ElementModP *, ElementModQ *, string>;

    struct NoncesData {
        ElementModQ *seed;
        uint64_t nextItem;
    };

    class EG_INTERNAL_API Nonces
    {
      public:
        Nonces(ElementModQ *seed, NoncesHeaderType headers);
        Nonces(ElementModQ *seed);
        ~Nonces();

        ElementModQ *get(uint64_t item);
        ElementModQ *get(uint64_t item, string headers);
        vector<ElementModQ *> get(uint64_t startItem, uint64_t count);
        ElementModQ *next();

      private:
        NoncesData data;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_NONCES_HPP_INCLUDED__ */

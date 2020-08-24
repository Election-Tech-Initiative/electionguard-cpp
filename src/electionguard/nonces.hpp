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
        int nextItem;
    };

    class Nonces
    {
      public:
        Nonces(ElementModQ *seed, NoncesHeaderType headers);
        Nonces(ElementModQ *seed);
        ~Nonces();

        ElementModQ *get(int item);
        ElementModQ *get(int item, string headers);
        vector<ElementModQ *> get(int startItem, int count);
        ElementModQ *next();

      private:
        NoncesData data;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_NONCES_HPP_INCLUDED__ */

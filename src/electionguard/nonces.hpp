#ifndef __ELECTIONGUARD__CORE_NONCE_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_NONCE_HPP_INCLUDED__
#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <vector>
using namespace std;

namespace electionguard
{
    struct NoncesData {
        ElementModQ *seed;
        int nextItem;
    };

    class Nonces
    {
      public:
        Nonces(ElementModQ *seed, ElementModP *headers);
        Nonces(ElementModQ *seed, ElementModQ *headers);
        Nonces(ElementModQ *seed, string headers);
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

#endif /* __ELECTIONGUARD__CORE_NONCE_HPP_INCLUDED__ */

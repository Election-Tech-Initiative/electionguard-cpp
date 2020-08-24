#include "electionguard/nonces.hpp"

#include "hash.hpp"
#include "variant_cast.hpp"

namespace electionguard
{
    Nonces::Nonces(ElementModQ *seed, NoncesHeaderType headers) : data()
    {
        CryptoHashableType h = variant_cast(headers);
        this->data.seed = hash_elems({seed, h});
        this->data.nextItem = 0;
    }

    Nonces::Nonces(ElementModQ *seed) : data()
    {
        this->data.seed = seed;
        this->data.nextItem = 0;
    }

    Nonces::~Nonces() {}

    ElementModQ *Nonces::get(int item)
    {
        this->data.nextItem = item + 1;
        return hash_elems({this->data.seed, item});
    }

    ElementModQ *Nonces::get(int item, string headers)
    {
        this->data.nextItem = item + 1;
        return hash_elems({this->data.seed, item, headers});
    }

    vector<ElementModQ *> Nonces::get(int startItem, int count)
    {
        vector<ElementModQ *> result;
        int endItem = startItem + count;
        for (int i(startItem); i < endItem; i++) {
            result.push_back(this->get(i));
        }
        return result;
    }

    ElementModQ *Nonces::next() { return this->get(this->data.nextItem); }
} // namespace electionguard
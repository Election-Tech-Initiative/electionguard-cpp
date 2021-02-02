#include "nonces.hpp"

#include "electionguard/hash.hpp"
#include "log.hpp"
#include "variant_cast.hpp"

using std::make_unique;
using std::string;
using std::unique_ptr;

namespace electionguard
{
    struct Nonces::Impl {

        Impl(const ElementModQ &seed, const NoncesHeaderType &headers)
        {
            CryptoHashableType headers_ = variant_cast(headers);
            this->seed = hash_elems({&const_cast<ElementModQ &>(seed), headers_});
            nextItem = 0;
        }
        explicit Impl(const ElementModQ &seed)
        {
            this->seed = make_unique<ElementModQ>(const_cast<ElementModQ &>(seed));
            nextItem = 0;
        }

        unique_ptr<ElementModQ> get(uint64_t item)
        {
            nextItem = item + 1;
            return hash_elems({seed.get(), item});
        }

        unique_ptr<ElementModQ> get(uint64_t item, string headers)
        {
            nextItem = item + 1;
            return hash_elems({seed.get(), item, headers});
        }

        unique_ptr<ElementModQ> next() { return this->get(nextItem); }

        unique_ptr<ElementModQ> seed;
        uint64_t nextItem;
    };

    Nonces::Nonces(const ElementModQ &seed, const NoncesHeaderType &headers)
        : pimpl(new Impl(seed, headers))
    {
    }

    Nonces::Nonces(const ElementModQ &seed) : pimpl(new Impl(seed)) {}

    Nonces &Nonces::operator=(Nonces rhs)
    {
        swap(pimpl, rhs.pimpl);
        return *this;
    }

    Nonces::~Nonces() = default;

    unique_ptr<ElementModQ> Nonces::get(uint64_t item) { return pimpl->get(item); }

    unique_ptr<ElementModQ> Nonces::get(uint64_t item, string headers)
    {
        return pimpl->get(item, move(headers));
    }

    vector<unique_ptr<ElementModQ>> Nonces::get(uint64_t startItem, uint64_t count)
    {
        // TODO: ISSUE #137: preallocate and address possible overflow
        vector<unique_ptr<ElementModQ>> result;
        uint64_t endItem = startItem + count;
        for (uint64_t i(startItem); i < endItem; i++) {
            result.push_back(this->get(i));
        }
        return result;
    }

    unique_ptr<ElementModQ> Nonces::next() { return pimpl->next(); }
} // namespace electionguard

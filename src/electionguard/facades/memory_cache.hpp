#ifndef __ELECTIONGUARD__CORE_CACHE_HPP_INCLUDED__
#define __ELECTIONGUARD__CORE_CACHE_HPP_INCLUDED__

#include <memory>
#include <unordered_map>
#include <vector>

using namespace std;

namespace electionguard
{
    // A simple container to allow object references
    // to live beyond the scope of a method while
    // crossing the C-API boundary
    template <typename T> class Cache
    {
      public:
        Cache() {}
        // take ownership of the unique_ptr
        // and cache it as a shared_ptr
        T *retain(unique_ptr<T> object)
        {
            shared_ptr<T> handle{move(object)};
            this->_cache.push_back(handle);
            return handle.get();
        }

      private:
        Cache(Cache const &notImplemented);
        Cache &operator=(Cache const &notImplemented);
        vector<shared_ptr<T>> _cache;
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD__CORE_CACHE_HPP_INCLUDED__ */

#ifndef __ELECTIONGUARD_CPP_UTILS_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_UTILS_HPP_INCLUDED__

#include <chrono>
#include <exception>
#include <map>

using std::map;
using std::out_of_range;
using std::chrono::system_clock;

namespace electionguard
{
    template <typename K, typename V>
    K findByValue(const map<K, const V> &collection, const V &value)
    {
        for (auto &element : collection) {
            if (element.second == value) {
                return element.first;
            }
        }
        throw out_of_range("value not found");
    }

    inline uint64_t getSystemTimestamp()
    {
        auto now = system_clock::now();
        auto ticks = now.time_since_epoch();
        return ticks.count() * system_clock::period::num / system_clock::period::den;
    }

} // namespace electionguard

#endif // __ELECTIONGUARD_CPP_UTILS_HPP_INCLUDED__

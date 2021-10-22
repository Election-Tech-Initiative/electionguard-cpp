#ifndef __ELECTIONGUARD_CPP_UTILS_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_UTILS_HPP_INCLUDED__

#include <chrono>
#include <electionguard/export.h>
#include <exception>
#include <map>

using std::map;
using std::out_of_range;
using std::chrono::duration;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::steady_clock;
using std::chrono::system_clock;
using std::chrono::time_point;

namespace electionguard
{
    inline bool is_little_endian()
    {
        const unsigned one = 1U;
        return reinterpret_cast<const char *>(&one) + sizeof(unsigned) - 1;
    }

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

#if defined(__GNUC__)        
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
#endif // __GNUC__
        return ticks.count() * system_clock::period::num / system_clock::period::den;
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif // __GNUC__

    }

    /// <Summary>
    /// A simple Stopwatch class with an API similar to C#
    /// </Summary>
    class EG_INTERNAL_API Stopwatch
    {

      public:
        void start() { _startTime = steady_clock::now(); }

        void stop() { _elapsedTime = steady_clock::now() - _startTime; }

        int64_t getElapsed() const
        {
            auto ms = duration_cast<milliseconds>(_elapsedTime);
            return ms.count();
        }

      private:
        std::chrono::time_point<steady_clock> _startTime = steady_clock::now();
        duration<double> _elapsedTime = duration<double>::zero();
    };

} // namespace electionguard

#endif // __ELECTIONGUARD_CPP_UTILS_HPP_INCLUDED__

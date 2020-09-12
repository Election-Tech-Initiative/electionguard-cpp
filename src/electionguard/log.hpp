#ifndef __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__

#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

namespace electionguard
{
    class Log
    {
      public:
        static void debug(string msg, const char *caller = __builtin_FUNCTION())
        {
            cout << caller << msg << endl;
        }
        static void debug(uint64_t *bn, size_t bnLen, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            cout << caller << msg << endl;
            for (size_t i(0); i < bnLen; i++) {
                cout << "[" << hex << setw(16) << setfill('0') << bn[i] << "]";
            }
            cout << endl;
        }
        static void debug(uint8_t *bytes, size_t bLen, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            cout << caller << msg << endl;
            for (size_t i(0); i < bLen; i++) {
                cout << "[" << hex << setw(2) << setfill('0') << (uint32_t)bytes[i] << "]";
            }
            cout << endl;
        }
        static void debug(vector<uint64_t> &container, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            cout << caller << msg << endl;
            for (auto element : container) {
                cout << "[" << hex << setw(2) << setfill('0') << element << "]";
            }
            cout << endl;
        }
        static void debug(vector<uint8_t> &container, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            cout << caller << msg << endl;
            for (auto element : container) {
                cout << "[" << hex << setw(2) << setfill('0') << (uint32_t)element << "]";
            }
            cout << endl;
        }
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__ */
#ifndef __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__

#include <ctime>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace electionguard
{
    // TODO: cleanup
    class Log
    {
        static void prefix(const char *caller = "")
        {
            auto now = time(nullptr);
            cout << "[ " << put_time(gmtime(&now), "%c") << " ]: " << caller << ": ";
        }

        static void postfix() { cout << endl; }

      public:
        static void debug(string msg, const char *caller = __builtin_FUNCTION())
        {
            cout << caller << msg << endl;
        }

        static void debugHex(string msg, string hexValue, const char *caller = __builtin_FUNCTION())
        {
            prefix(caller);
            cout << msg << endl;

            stringstream stream;
            string decValue;

            stream << hex << hexValue;
            stream >> decValue;
            cout << dec << setw(2) << decValue;

            postfix();
        }

        static void debug(map<string, bool> printMap, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            prefix(caller);
            cout << msg << endl;
            cout << "----------- Begin Map ------------" << endl;
            for (auto &x : printMap) {
                cout << x.first << ": " << boolalpha << x.second << endl;
            }
            cout << "----------- End Map ------------" << endl;
            postfix();
        }

        static void debug(uint64_t *bn, size_t bnLen, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            prefix(caller);
            cout << msg << endl;
            ios cout_state(nullptr);
            cout_state.copyfmt(cout);
            cout << "base-16: " << endl;
            for (size_t i(0); i < bnLen; i++) {
                cout << "[" << hex << setw(16) << setfill('0') << bn[i] << "]";
            }
            postfix();
            cout.copyfmt(cout_state);
        }
        static void debug(uint8_t *bytes, size_t bLen, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            prefix(caller);
            cout << msg << endl;
            ios cout_state(nullptr);
            cout_state.copyfmt(cout);
            cout << "base-16: " << endl;
            for (size_t i(0); i < bLen; i++) {
                cout << "[" << hex << setw(2) << setfill('0') << (uint32_t)bytes[i] << "]";
            }
            postfix();
            cout.copyfmt(cout_state);
        }
        static void debug(vector<uint64_t> &container, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            prefix(caller);
            cout << msg << endl;
            for (auto element : container) {
                cout << "[" << hex << setw(2) << setfill('0') << element << "]";
            }
            cout << endl;
        }
        static void debug(vector<uint8_t> &container, string msg,
                          const char *caller = __builtin_FUNCTION())
        {
            prefix(caller);
            cout << msg << endl;
            for (auto element : container) {
                cout << "[" << hex << setw(2) << setfill('0') << (uint32_t)element << "]";
            }
            postfix();
        }
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__ */

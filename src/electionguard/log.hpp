#ifndef __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__

#include <iostream>

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
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_LOG_HPP_INCLUDED__ */
#ifndef __ELECTIONGUARD_CPP_LOG_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_LOG_HPP_INCLUDED__

#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <map>
#include <string>
#include <variant>
#include <vector>

using std::exception;
using std::ios;
using std::map;
using std::shared_ptr;
using std::string;
using std::stringstream;
using std::vector;

namespace electionguard
{
    using DomainLoggableType =
      std::variant<std::nullptr_t, uint64_t, std::string, ElementModP *, ElementModQ *,
                   std::reference_wrapper<ElementModP>, std::reference_wrapper<ElementModQ>,
                   std::reference_wrapper<const ElementModP>,
                   std::reference_wrapper<const ElementModQ>, std::map<std::string, bool>>;

    class EG_INTERNAL_API Log
    {
      public:
        Log(Log const &) = delete;
        void operator=(Log const &) = delete;

        static void trace(string msg, const char *caller = __builtin_FUNCTION());
        static void trace(string msg, DomainLoggableType obj,
                          const char *caller = __builtin_FUNCTION());

        static void debug(string msg, const char *caller = __builtin_FUNCTION());
        static void debug(string msg, DomainLoggableType obj,
                          const char *caller = __builtin_FUNCTION());

        static void info(string msg, const char *caller = __builtin_FUNCTION());
        static void info(string msg, DomainLoggableType obj,
                         const char *caller = __builtin_FUNCTION());

        static void warn(string msg, const char *caller = __builtin_FUNCTION());
        static void warn(string msg, DomainLoggableType obj,
                         const char *caller = __builtin_FUNCTION());

        static void error(string msg);
        static void error(string msg, DomainLoggableType obj,
                          const char *caller = __builtin_FUNCTION());
        static void error(string msg, const std::exception &obj,
                          const char *caller = __builtin_FUNCTION());

      private:
        class Impl;
        Log() {}
    };

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_LOG_HPP_INCLUDED__ */

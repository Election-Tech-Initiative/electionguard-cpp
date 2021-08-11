#include "log.hpp"

#include "spdlog/cfg/env.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/spdlog.h"

#include <iomanip>
#include <sstream>

using std::boolalpha;
using std::endl;
using std::get;
using std::make_unique;
using std::ostringstream;
using std::reference_wrapper;
using std::shared_ptr;
using std::to_string;

namespace electionguard
{
    enum DomainLoggableTypeEnum {
        NULL_PTR = 0,
        UINT64_T = 1,
        STRING = 2,
        PTR_ELEMENTMODP = 3,
        PTR_ELEMENTMODQ = 4,
        REF_WRAP_ELEMENTMODP = 5,
        REF_WRAP_ELEMENTMODQ = 6,
        REF_WRAP_CONST_ELEMENTMODP = 7,
        REF_WRAP_CONST_ELEMENTMODQ = 8,
        MAP_STRING_BOOL = 9

    };

    string format(DomainLoggableType element)
    {
        switch (element.index()) {
            case NULL_PTR: {
                return "nullptr";
            }
            case UINT64_T: {
                return to_string(get<uint64_t>(element));
            }
            case STRING: {
                return get<string>(element);
            }
            case PTR_ELEMENTMODP: {
                return get<ElementModP *>(element)->toHex();
            }
            case PTR_ELEMENTMODQ: {
                return get<ElementModQ *>(element)->toHex();
            }
            case REF_WRAP_ELEMENTMODP: {
                return get<reference_wrapper<ElementModP>>(element).get().toHex();
            }
            case REF_WRAP_ELEMENTMODQ: {
                return get<reference_wrapper<ElementModQ>>(element).get().toHex();
            }
            case REF_WRAP_CONST_ELEMENTMODP: {
                return get<reference_wrapper<const ElementModP>>(element).get().toHex();
            }
            case REF_WRAP_CONST_ELEMENTMODQ: {
                return get<reference_wrapper<const ElementModQ>>(element).get().toHex();
            }
            case MAP_STRING_BOOL: {
                auto mapping = get<map<string, bool>>(element);
                ostringstream ss;
                ss << endl;
                ss << "----------- Begin Map ------------" << endl;
                for (auto &x : mapping) {
                    ss << x.first << ": " << boolalpha << x.second << endl;
                }
                ss << "------------ End Map -------------" << endl;
                return ss.str();
            }
        }
        return "unknown";
    }

    class Log::Impl
    {
      private:
        Impl() {}

      public:
        shared_ptr<spdlog::logger> logger;

        static Impl &instance()
        {
            static Impl _instance;
            if (_instance.logger != nullptr)
                return _instance;

            auto console = spdlog::stdout_logger_mt("console");
#if LOG_DEBUG
            console->set_level(spdlog::level::debug);
#else
            console->set_level(spdlog::level::info);
#endif
            spdlog::set_default_logger(console);
            spdlog::set_pattern("[%H:%M:%S:%e %z] [p: %P] [t: %t] [%l] :: %v");
            _instance.logger = console;
            return _instance;
        }
    };

    void Log::trace(string msg, const char *caller) { Impl::instance().logger->trace(msg); }
    void Log::trace(string msg, DomainLoggableType obj, const char *caller)
    {
        Impl::instance().logger->trace("{}::{}: {}", caller, msg, format(obj));
    }

    void Log::debug(string msg, const char *caller) { Impl::instance().logger->debug(msg); }
    void Log::debug(string msg, DomainLoggableType obj, const char *caller)
    {
        Impl::instance().logger->debug("{}::{}: {}", caller, msg, format(obj));
    }

    void Log::info(string msg, const char *caller) { Impl::instance().logger->info(msg); }
    void Log::info(string msg, DomainLoggableType obj, const char *caller)
    {
        Impl::instance().logger->info("{}::{}: {}", caller, msg, format(obj));
    }

    void Log::warn(string msg, const char *caller) { Impl::instance().logger->warn(msg); }
    void Log::warn(string msg, DomainLoggableType obj, const char *caller)
    {
        Impl::instance().logger->warn("{}::{}: {}", caller, msg, format(obj));
    }

    void Log::error(string msg) { Impl::instance().logger->error(msg); }
    void Log::error(string msg, DomainLoggableType obj, const char *caller)
    {
        Impl::instance().logger->error("{}::{}: {}", caller, msg, format(obj));
    }
    void Log::error(string msg, const std::exception &obj, const char *caller)
    {
        Impl::instance().logger->error("{}::{}: {}", caller, msg, obj.what());
    }

} // namespace electionguard

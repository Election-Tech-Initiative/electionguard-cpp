#include "date/date.h"

#include <chrono>
#include <codecvt>
#include <iomanip>
#include <iostream>
#include <kremlin/lowstar_endianness.h>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using time_point = std::chrono::system_clock::time_point;
using std::get_time;
using std::gmtime;
using std::mktime;
using std::stringstream;
using std::chrono::system_clock;

namespace electionguard
{
    string timePointToIsoString(const time_point &time, const string &format)
    {
        auto c_time = system_clock::to_time_t(time);
        struct tm gmt;

#ifdef _WIN32
        // TODO: ISSUE #136: handle err
        gmtime_s(&gmt, &c_time);
#else
        gmtime_r(&c_time, &gmt);
#endif
        stringstream ss;
        ss << std::put_time(&gmt, format.c_str());
        return ss.str();
    }

    time_point timePointFromIsoString(const string &time, const string &format)
    {
        date::sys_seconds tm;
        std::istringstream ss{time};
        ss >> date::parse(format, tm);
        return tm;
    }

} // namespace electionguard

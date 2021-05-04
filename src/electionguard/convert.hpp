#ifndef __ELECTIONGUARD_CPP_CONVERT_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_CONVERT_HPP_INCLUDED__

#include <chrono>
#include <codecvt>
#include <iomanip>
#include <iostream>
#include <kremlin/lowstar_endianness.h>
#include <sstream>
#include <string>
#include <vector>

using std::bad_cast;
using std::begin;
using std::end;
using std::hex;
using std::setfill;
using std::setw;
using std::string;
using std::vector;
using time_point = std::chrono::system_clock::time_point;
using std::get_time;
using std::gmtime;
using std::mktime;
using std::stringstream;
using std::uppercase;
using std::wstring;
using std::chrono::system_clock;

namespace electionguard
{
    template <typename T> auto release(std::vector<T> &container)
    {
        container.clear();
        container.shrink_to_fit();
        container = vector<T>();
        return container.capacity();
    }

    inline uint32_t convert(size_t size)
    {
        if (size > UINT32_MAX) {
            throw bad_cast();
        }
        return static_cast<uint32_t>(size);
    }

    inline void hex_to_bytes(const string &hex, uint8_t *bytesOut)
    {
        const size_t baseHex = 16;
        for (size_t i(0); i < hex.size(); i += 2) {
            string byteString = hex.substr(i, 2);
            uint16_t as_int = static_cast<uint16_t>(stoi(byteString, nullptr, baseHex));
            uint8_t byte = static_cast<uint8_t>(as_int);
            bytesOut[i / 2] = byte;
        }
    }

    inline void hex_to_bytes_be(const string &hex, uint8_t *bytesOut)
    {
        const size_t baseHex = 16;
        for (size_t i(0); i < hex.size(); i += 2) {
            string byteString = hex.substr(i, 2);
            uint16_t as_int = static_cast<uint16_t>(stoi(byteString, nullptr, baseHex));
            uint16_t bigEndian = htobe16(as_int);
            bytesOut[i / 2] = static_cast<uint8_t>(bigEndian);
        }
    }

    inline vector<uint8_t> hex_to_bytes(const string &hexString)
    {
        vector<uint8_t> bytes;
        const size_t baseHex = 16;
        for (size_t i = 0; i < hexString.size(); i += 2) {
            auto byteString = hexString.substr(i, 2);
            uint16_t as_int = static_cast<uint16_t>(stoi(byteString, nullptr, baseHex));
            uint8_t array[sizeof(uint16_t)];
            memmove(&array, &as_int, sizeof(as_int));
            bytes.push_back(array[0]);
        }

        return bytes;
    }

    inline vector<uint8_t> hex_to_bytes_be(const string &hexString)
    {
        vector<uint8_t> bytes;
        const size_t baseHex = 16;
        for (size_t i = 0; i < hexString.size(); i += 2) {
            auto byteString = hexString.substr(i, 2);
            uint16_t as_int = static_cast<uint16_t>(stoi(byteString, nullptr, baseHex));
            uint8_t array[sizeof(uint16_t)];
            uint16_t bigEndian = htobe16(as_int);
            memmove(&array, &bigEndian, sizeof(bigEndian));
            bytes.push_back(array[0]);
        }

        return bytes;
    }

    inline string bytes_to_hex(const vector<uint8_t> &bytes)
    {
        // Iterate through the returned bytes to convert to Hex representation
        // while ignoring any initial 0-bytes
        bool detectedFirstNonZeroBytes = false;
        stringstream stream;
        stream << hex << uppercase;
        for (auto byte : bytes) {
            if (!detectedFirstNonZeroBytes && byte != 0) {
                detectedFirstNonZeroBytes = true;
            }
            if (detectedFirstNonZeroBytes) {
                stream << setw(2) << setfill('0') << static_cast<uint16_t>(byte);
            }
        }

        return detectedFirstNonZeroBytes ? (stream.str()) : "00";
    }

    template <typename T, size_t S> string bytes_to_hex(T (&data)[S])
    {
        vector<T> vec(begin(data), end(data));
        auto result = bytes_to_hex(vec);
        release(vec);
        return result;
    }

    const string LEADING_CHARS = "0 \n\r\t\f\v";

    inline string sanitize_hex_string(const string &hexString)
    {
        string sanitized;
        string leftTrimmed = hexString.substr(hexString.find_first_not_of(LEADING_CHARS));
        bool isOdd = (leftTrimmed.size() % 2) != 0U;
        if (isOdd) {
            sanitized.insert(0, 1, '0');
        }
        sanitized.append(leftTrimmed);
        return sanitized;
    }

    inline wstring stringToWideString(const std::string &str)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.from_bytes(str);
    }

    inline string wideStringToString(const std::wstring &wstr)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;

        return converterX.to_bytes(wstr);
    }

    /// Copy the string to a heap-allocated null-termianted array
    inline char *dynamicCopy(const string &data, size_t *out_size)
    {
        auto data_size = data.length() + 1;
        auto *data_array = static_cast<char *>(malloc(data_size));
// TODO: allow warning and change to safe?
#pragma warning(suppress : 4996)
        strncpy(data_array, data.c_str(), data_size);
        *out_size = data_size;
        return data_array;
    }

    /// Copy the string to a heap-allocated null-termianted array
    inline char *dynamicCopy(const string &data)
    {
        size_t size = 0;
        return dynamicCopy(data, &size);
    }

    /// Copy the vector byte array to a heap-allocated byte array
    inline uint8_t *dynamicCopy(vector<uint8_t> &data, size_t *out_size)
    {
        auto *data_array = new uint8_t[data.size()];
        copy(data.begin(), data.end(), data_array);
        *out_size = data.size();
        return data_array;
    }

    string timePointToIsoString(const time_point &time, const string &format);
    time_point timePointFromIsoString(const string &time, const string &format);

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_CONVERT_HPP_INCLUDED__ */

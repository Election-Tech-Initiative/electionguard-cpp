#ifndef __ELECTIONGUARD_CPP_CONVERT_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_CONVERT_HPP_INCLUDED__

#include "facades/Hacl_Bignum256.hpp"
#include "facades/Hacl_Bignum4096.hpp"
#include "log.hpp"

#include <chrono>
#include <cmath>
#include <codecvt>
#include <iomanip>
#include <iostream>
#include <karamel/LowStar_Endianness.h>
#include <sstream>
#include <string>
#include <vector>

using std::bad_cast;
using std::begin;
using std::end;
using std::get_time;
using std::gmtime;
using std::hex;
using std::mktime;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::uppercase;
using std::vector;
using std::wstring;
using std::chrono::system_clock;

using electionguard::Log;

namespace electionguard
{
    const string LEADING_CHARS = "0 \n\r\t\f\v";

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

    inline string uint64_to_hex_string(uint64_t number)
    {
        bool detectedFirstNonZeroBytes = false;
        stringstream stream;
        stream << hex << uppercase;
        stream << setw(16) << setfill('0') << std::hex << number;

        return stream.str();
    }

    inline size_t uint64_to_size(uint64_t number)
    {
        if (number > SIZE_MAX) {
            throw std::out_of_range("the uint64_t value is larger than the size_t width");
        }
        return (size_t)number;
    }

    inline vector<uint8_t> bignum_to_bytes(const vector<uint64_t> &bignum)
    {
        size_t offset = sizeof(uint64_t) / sizeof(uint8_t);
        std::vector<uint8_t> bytes;
        bytes.reserve((bignum.size() * offset));
        for (auto number : bignum) {
            uint64_t buffer = htobe64(number);
            for (size_t i = 0; i < sizeof(buffer); i++) {
                bytes.push_back(buffer & 0xFF);
                buffer >>= 8;
            }
        }
        return bytes;
    }

    inline vector<uint8_t> bignum_to_bytes(uint64_t *data, size_t size)
    {
        vector<uint64_t> vec(data, data + size);
        auto result = bignum_to_bytes(vec);
        release(vec);
        return result;
    }

    inline vector<uint64_t> copy_bytes_to_bignum_64(const vector<uint8_t> &bytes, size_t outputMask)
    {
        vector<uint64_t> bignum;
        bignum.reserve(outputMask);
        size_t offset = sizeof(uint64_t) / sizeof(uint8_t);
        auto residue = bytes.size() % offset;

        auto limbs = static_cast<size_t>(std::ceil((bytes.size() / offset)));
        limbs += residue > 0 ? 1 : 0;

        size_t prepend = outputMask > limbs ? static_cast<size_t>((outputMask - limbs)) : 0;

        Log::trace("bytes.size(): " + std::to_string(bytes.size()));
        Log::trace("offset: " + std::to_string(offset));
        Log::trace("residue: " + std::to_string(residue));
        Log::trace("limbs: " + std::to_string(limbs));
        Log::trace("prepend: " + std::to_string(prepend));

        // prepend zeroes
        for (size_t i = 0; i < prepend; i += 1) {
            bignum.push_back(0UL);
        }

        // iterate over a window
        for (size_t i = 0; i < bytes.size(); i += offset) {

            uint64_t as_int = 0;
            auto remaining = bytes.size() - i;

            if (remaining > offset) {
                for (size_t j = i; j < i + 8; j++) {
                    as_int = (as_int << 8) + (bytes[j] & 0xFF);
                }
            } else {
                // assign the rest of the values
                for (size_t j = i; j < i + remaining; j++) {
                    as_int = (as_int << 8) + (bytes[j] & 0xFF);
                }
            }

            // Log::debug("as_hex: " + uint64_to_hex_string(as_int));
            bignum.push_back(as_int);
        }

        return bignum;
    }

    inline string sanitize_hex_string(const string &hexString)
    {
        string sanitized;
        string leftTrimmed = hexString.substr(hexString.find_first_not_of(LEADING_CHARS));
        bool isOdd = (leftTrimmed.size() % 2) != 0U;
        if (isOdd) {
            Log::trace("sanitize_hex_string: is odd");
            sanitized.insert(0, 1, '0');
        }
        sanitized.append(leftTrimmed);
        return sanitized;
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

    template <typename T> string bytes_to_hex(T *data, size_t size)
    {
        vector<T> vec(begin(data), data + size);
        auto result = bytes_to_hex(vec);
        release(vec);
        return result;
    }

    inline string bignum_to_hex_string(const vector<uint64_t> &bignum)
    {
        auto bytes = bignum_to_bytes(bignum);
        return bytes_to_hex(bytes);
    }

    inline string bignum_to_hex_string(uint64_t *data, size_t size)
    {
        vector<uint64_t> vec(data, data + size);
        auto result = bignum_to_hex_string(vec);
        release(vec);
        return result;
    }

    inline auto hacl_to_hex_256(uint64_t *data)
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 32 bytes
        uint8_t byteResult[MAX_Q_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        hacl::Bignum256::toBytes(static_cast<uint64_t *>(data), static_cast<uint8_t *>(byteResult));
        return bytes_to_hex(byteResult);
    }

    inline auto hacl_to_hex_4096(uint64_t *data)
    {
        // Returned bytes array from Hacl needs to be pre-allocated to 512 bytes
        uint8_t byteResult[MAX_P_SIZE] = {};
        // Use Hacl to convert the bignum to byte array
        hacl::Bignum4096::toBytes(static_cast<uint64_t *>(data),
                                  static_cast<uint8_t *>(byteResult));
        return bytes_to_hex(byteResult);
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

    /// Copy the vector byte array to a heap-allocated byte array
    inline uint32_t *dynamicCopy(vector<uint32_t> &data, size_t *out_size)
    {
        auto *data_array = new uint32_t[data.size()];
        copy(data.begin(), data.end(), data_array);
        *out_size = data.size();
        return data_array;
    }

    /// Copy the vector byte array to a heap-allocated byte array
    inline uint64_t *dynamicCopy(vector<uint64_t> &data, size_t *out_size)
    {
        auto *data_array = new uint64_t[data.size()];
        copy(data.begin(), data.end(), data_array);
        *out_size = data.size();
        return data_array;
    }

    string timePointToIsoString(const std::chrono::system_clock::time_point &time);
    string timePointToIsoString(const std::chrono::system_clock::time_point &time,
                                const string &format);
    std::chrono::system_clock::time_point timePointFromIsoString(const string &time);
    std::chrono::system_clock::time_point timePointFromIsoString(const string &time,
                                                                 const string &format);

} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_CONVERT_HPP_INCLUDED__ */

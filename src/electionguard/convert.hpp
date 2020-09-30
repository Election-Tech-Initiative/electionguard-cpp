#ifndef __ELECTIONGUARD_CORE_CONVERT_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_CONVERT_HPP_INCLUDED__

#include <iomanip>
#include <iostream>
#include <kremlin/lowstar_endianness.h>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace electionguard
{
    template <typename T> auto release(std::vector<T> &container)
    {
        container.clear();
        container.shrink_to_fit();
        container = vector<T>();
        return container.capacity();
    }

    void hex_to_bytes(const string &hex, uint8_t *bytesOut)
    {
        const size_t baseHex = 16;
        for (size_t i(0); i < hex.size(); i += 2) {
            string byteString = hex.substr(i, 2);
            auto byte = static_cast<uint8_t>(stoi(byteString, nullptr, baseHex));
            bytesOut[i / 2] = byte;
        }
    }

    void hex_to_bytes_be(const string &hex, uint8_t *bytesOut)
    {
        const size_t baseHex = 16;
        for (size_t i(0); i < hex.size(); i += 2) {
            string byteString = hex.substr(i, 2);
            uint16_t as_int = stoi(byteString, nullptr, baseHex);
            uint16_t bigEndian = htobe16(as_int);
            bytesOut[i / 2] = (uint8_t)bigEndian;
        }
    }

    vector<uint8_t> hex_to_bytes(const string &hexString)
    {
        vector<uint8_t> bytes;
        const size_t baseHex = 16;
        for (size_t i = 0; i < hexString.size(); i += 2) {
            auto byteString = hexString.substr(i, 2);
            uint16_t as_int = stoi(byteString, nullptr, baseHex);
            uint8_t array[sizeof(uint16_t)];
            memmove(&array, &as_int, sizeof(as_int));
            bytes.push_back(array[0]);
        }

        return bytes;
    }

    vector<uint8_t> hex_to_bytes_be(const string &hexString)
    {
        vector<uint8_t> bytes;
        const size_t baseHex = 16;
        for (size_t i = 0; i < hexString.size(); i += 2) {
            auto byteString = hexString.substr(i, 2);
            uint16_t as_int = stoi(byteString, nullptr, baseHex);
            uint8_t array[sizeof(uint16_t)];
            uint16_t bigEndian = htobe16(as_int);
            memmove(&array, &bigEndian, sizeof(bigEndian));
            bytes.push_back(array[0]);
        }

        return bytes;
    }

    string bytes_to_hex(const vector<uint8_t> &bytes)
    {
        // Iterate through the returned bytes to convert to Hex representation
        // while ignoring any initial 0-bytes
        bool detectedFirstNonZeroBytes = false;
        stringstream stream;
        stream << hex;
        for (auto byte : bytes) {
            if (!detectedFirstNonZeroBytes && byte != 0) {
                detectedFirstNonZeroBytes = true;
            }
            if (detectedFirstNonZeroBytes) {
                stream << setw(2) << setfill('0') << (uint16_t)byte;
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

    string sanitize_hex_string(const string &hexString)
    {
        string sanitized;
        bool isOdd = (hexString.size() % 2) != 0U;
        if (isOdd) {
            sanitized.insert(0, 1, '0');
        }
        sanitized.append(hexString);
        return sanitized;
    }
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_CONVERT_HPP_INCLUDED__ */

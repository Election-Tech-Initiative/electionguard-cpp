#ifndef __ELECTIONGUARD_CPP_TEST_UTILS_BYTE_LOGGER_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_TEST_UTILS_BYTE_LOGGER_HPP_INCLUDED__

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class ByteLogger
{
  public:
    static void print(string msg, uint64_t *bignum, size_t size)
    {
        cout << msg << endl;
        ios cout_state(nullptr);
        cout_state.copyfmt(cout);
        cout << "base-16: " << endl;
        for (size_t i(0); i < size; i++) {
            cout << "0x" << hex << setw(16) << setfill('0') << bignum[i] << ",";
        }
        cout << endl;
        cout << "string: " << endl;
        for (size_t i(0); i < size; i++) {
            cout << hex << setw(16) << setfill('0') << bignum[i];
        }
        cout << endl;
        cout.copyfmt(cout_state);
    }
    static void print(string msg, uint8_t *bytes, size_t size)
    {
        cout << msg << endl;
        ios cout_state(nullptr);
        cout_state.copyfmt(cout);
        cout << "base-16: " << endl;
        for (size_t i(0); i < size; i++) {
            cout << "[" << hex << setw(2) << setfill('0') << static_cast<uint32_t>(bytes[i]) << "]";
        }
        cout << endl;
        cout.copyfmt(cout_state);
    }
    static void print(string msg, vector<uint64_t> &container)
    {
        cout << msg << endl;
        ios cout_state(nullptr);
        cout_state.copyfmt(cout);
        cout << "base-16: " << endl;
        for (auto element : container) {
            cout << "0x" << hex << setw(16) << setfill('0') << element << ",";
        }
        cout << endl;
    }
    static void print(string msg, vector<uint8_t> &container)
    {

        cout << msg << endl;
        for (auto element : container) {
            cout << "[" << hex << setw(2) << setfill('0') << static_cast<uint32_t>(element) << "]";
        }
        cout << endl;
    }
};

#endif /* __ELECTIONGUARD_CPP_TEST_UTILS_BYTE_LOGGER_HPP_INCLUDED__ */
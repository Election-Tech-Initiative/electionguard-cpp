#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/hash.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

extern "C" {
#include "../../src/kremlin/Hacl_Bignum4096.h"
}

// TEST_CASE("Test Hacl BigNum")
// {
//     cout
//       << "--- TESTING Hacl_Bignum4096_new_bn_from_bytes_be and Hacl_Bignum4096_bn_to_bytes_be ---\n"
//       << endl;
//     uint8_t oneByte[8] = {0, 0, 0, 0, 0, 0, 0, 1};
//     cout << "original number (as decimal) = " << to_string(oneByte[7]) << endl;
//     cout << "original number (as hex) = " << hex << setw(2) << setfill('0') << (int)oneByte[7]
//          << endl;

//     uint64_t *bnResult = Hacl_Bignum4096_new_bn_from_bytes_be(sizeof(oneByte), oneByte);
//     size_t bnLength = sizeof(bnResult) / sizeof(bnResult[0]);
//     cout << "\nbnLength = " << bnLength << endl;
//     cout << "\n--- After Hacl_Bignum4096_new_bn_from_bytes_be ---\n" << endl;
//     cout << "uint64_t bnResult[8] (as decimal) = ";
//     for (size_t i(0); i < bnLength; ++i) {
//         cout << "[" << to_string(bnResult[i]) << "]";
//     }
//     cout << endl;
//     cout << "uint64_t bnResult[8] (as hex) = ";
//     for (size_t i(0); i < bnLength; ++i) {
//         cout << "[" << hex << setw(2) << setfill('0') << bnResult[i] << "]";
//     }
//     cout << endl;

//     uint8_t byteResult[64] = {};
//     uint64_t bnResult4096[64] = {};
//     memcpy(bnResult4096, bnResult, bnLength);
//     Hacl_Bignum4096_bn_to_bytes_be(bnResult4096, byteResult);
//     cout << "\n--- After Hacl_Bignum4096_bn_to_bytes_be ---\n" << endl;
//     cout << "uint8 byteResult[64] (as dec) = ";
//     for (size_t i(0); i < sizeof(byteResult); ++i) {
//         cout << "[" << to_string(byteResult[i]) << "]";
//     }
//     cout << endl;
//     cout << "uint8 byteResult[64] (as hex) = ";
//     for (size_t i(0); i < sizeof(byteResult); ++i) {
//         cout << "[" << hex << setw(2) << setfill('0') << (int)byteResult[i] << "]";
//     }
//     cout << endl;
// }

TEST_CASE("TEST MOD")
{
    uint64_t mod[64UL] = {0x06};
    electionguard::Log::debug(mod, 64, " : TEST MOD : mod :");
    uint64_t a[64UL] = {0x0c};
    electionguard::Log::debug(a, 64, " : TEST MOD : a :");
    uint64_t one[64UL] = {1UL};

    uint64_t resModQ[64UL] = {};
    Hacl_Bignum4096_mod_exp(mod, a, 64UL, one, resModQ);
    electionguard::Log::debug(resModQ, 64, " : TEST MOD : resModQ :");
}

TEST_CASE("TEST LESS THAN COMP")
{
    uint64_t a[64UL] = {0xFFFFFFFFFFFFFF43, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                        0xFFFFFFFFFFFFFFFF};
    electionguard::Log::debug(a, 64, " : TEST MOD : a :");
    uint64_t b[64UL] = {0xFFFFFFFFFFFFFF45, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                        0xFFFFFFFFFFFFFFFF};
    electionguard::Log::debug(b, 64, " : TEST MOD : b :");

    auto isLessThan = Hacl_Bignum4096_lt(a, b);
    electionguard::Log::debug(" : TEST MOD : isLessThan : " + to_string(isLessThan));
}
#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>

using namespace electionguard;

TEST_CASE("elgamalEncrypt simple with nonce is 1, publickey is g_pow_p(2)")
{
    const auto &one = ONE_MOD_Q();
    const auto &two = TWO_MOD_P();
    auto publicKey = g_pow_p(two);
    const auto &g = G();

    auto cipherText = elgamalEncrypt(0UL, one, *publicKey);

    // Log::debug(" : cipherText->getData()->toHex() = " + cipherText->getData()->toHex());
    // Log::debug(" : cipherText->getPad()->toHex() = " + cipherText->getPad()->toHex());
    CHECK((*publicKey == *cipherText->getData()));
    CHECK((const_cast<ElementModP &>(g) == *cipherText->getPad()));
}

// TEST_CASE("elgamalEncrypt simple encrypt 0")
// {
//     auto *nonce = ONE_MOD_Q();
//     auto *secret = TWO_MOD_Q();
//     auto *keypair = ElGamalKeyPair::fromSecret(secret);
//     auto *publicKey = keypair->getPublicKey();

//     CHECK((*publicKey < *P()));
//     auto *elem = g_pow_p(ZERO_MOD_Q()->toElementModP());
//     CHECK((*elem == *ONE_MOD_P())); // g^0 = 1

//     auto *cipherText = elgamalEncrypt(0UL, nonce, publicKey);
//     CHECK((*G() == *cipherText->getPad()));

//     auto *result1 = pow_mod_p(cipherText->getPad(), secret->toElementModP());
//     auto *calculatedData = pow_mod_p(publicKey, nonce->toElementModP());
//     CHECK((*result1 == *calculatedData));

//     CHECK((*cipherText->getData() == *calculatedData));

//     auto decrypted = cipherText->decrypt(secret->toElementModP());
//     CHECK((0UL == decrypted));
// }

// TEST_CASE("elgamalEncrypt simple encrypt 1")
// {
//     auto *nonce = ONE_MOD_Q();
//     auto *secret = TWO_MOD_Q();
//     auto *keypair = ElGamalKeyPair::fromSecret(secret);
//     auto *publicKey = keypair->getPublicKey();

//     CHECK((*publicKey < *P()));
//     auto *elem = g_pow_p(ONE_MOD_Q()->toElementModP());
//     CHECK((*elem == *G())); // g^1 = g

//     auto *cipherText = elgamalEncrypt(1UL, nonce, publicKey);

//     Log::debug(cipherText->getPad()->get(), MAX_P_LEN, " : cipherText->getPad() = ");
//     Log::debug(cipherText->getData()->get(), MAX_P_LEN, " : cipherText->getData()= ");

//     // CHECK((*G() == *cipherText->getPad()));

//     auto *result1 = pow_mod_p(cipherText->getPad(), secret->toElementModP());
//     auto *calculatedData = pow_mod_p(publicKey, nonce->toElementModP());
//     CHECK((*result1 == *calculatedData));

//     // CHECK((*cipherText->getData() == *calculatedData));

//     auto decrypted = cipherText->decrypt(secret->toElementModP());
//     Log::debug(": decrypted: " + to_string(decrypted));
//     CHECK((1UL == decrypted));
// }
#include "../../src/electionguard/log.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>

using namespace electionguard;

// TEST_CASE("elgamalEncrypt simple with nonce is 1, publickey is g_pow_p(2)")
// {
//     const auto &one = ONE_MOD_Q();
//     const auto &two = TWO_MOD_P();
//     auto publicKey = g_pow_p(two);
//     const auto &g = G();

//     auto cipherText = elgamalEncrypt(0UL, one, *publicKey);

//     CHECK((*publicKey == *cipherText->getData()));
//     CHECK((const_cast<ElementModP &>(g) == *cipherText->getPad()));
// }

TEST_CASE("elgamalEncrypt simple encrypt 0, with nonce 1 then publickey is g_pow_p(2)")
{
    auto &nonce = ONE_MOD_Q();
    auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret);
    auto *publicKey = keypair->getPublicKey();

    CHECK((*publicKey < P()));

    auto elem = g_pow_p(ZERO_MOD_P());
    CHECK((*elem == ONE_MOD_P())); // g^0 = 1

    auto cipherText = elgamalEncrypt(0UL, nonce, *publicKey);

    CHECK((*publicKey == *cipherText->getData()));
    CHECK((const_cast<ElementModP &>(G()) == *cipherText->getPad()));

    auto decryptedData = pow_mod_p(*cipherText->getPad(), *secret.toElementModP());
    auto calculatedData = pow_mod_p(*publicKey, *nonce.toElementModP());
    CHECK((*decryptedData == *calculatedData));
    CHECK((*cipherText->getData() == *calculatedData));

    auto decrypted = cipherText->decrypt(secret);
    CHECK((0UL == decrypted));
}

TEST_CASE("elgamalEncrypt simple encrypt 1 decrypts with secret")
{
    auto &nonce = ONE_MOD_Q();
    auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret);
    auto *publicKey = keypair->getPublicKey();

    CHECK((*publicKey < P()));

    auto elem = g_pow_p(ONE_MOD_P());
    CHECK((*elem == G())); // g^1 = g

    auto cipherText = elgamalEncrypt(1UL, nonce, *publicKey);
    CHECK((const_cast<ElementModP &>(G()) == *cipherText->getPad()));

    auto decrypted = cipherText->decrypt(secret);
    CHECK(1UL == decrypted);
}

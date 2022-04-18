#include "../../src/electionguard/log.hpp"
#include "../../src/electionguard/nonces.hpp"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>
#include <electionguard/precompute_buffers.hpp>

using namespace electionguard;
using namespace std;

TEST_CASE("ElGamalKeyPair fromSecret public key is fixed base")
{
    auto secret = rand_q();
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto *publicKey = keypair->getPublicKey();

    CHECK((keypair->getPublicKey()->isFixedBase() == true));
}

TEST_CASE("elgamalEncrypt simple encrypt 0, with nonce 1 then publickey is g_pow_p(2)")
{
    const auto &nonce = ONE_MOD_Q();
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
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

TEST_CASE("elgamalEncrypt simple encrypt 0, with real nonce")
{
    unique_ptr<ElementModQ> nonce = rand_q();
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    CHECK((*publicKey < P()));

    auto cipherText = elgamalEncrypt(0UL, *nonce, *publicKey);

    auto decrypted = cipherText->decrypt(secret);
    CHECK((0UL == decrypted));
}

TEST_CASE("elgamalEncrypt simple encrypt 0 compared with elgamalEncrypt_with_precomputed")
{
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    CHECK((*publicKey < P()));

    // cause a two triples and a quad to be populated
    PrecomputeBufferContext::init(1);
    PrecomputeBufferContext::populate(*keypair->getPublicKey());
    PrecomputeBufferContext::stop_populate();

    // this function runs off to look in the precomputed values buffer and if
    // it finds what it needs the the returned class will contain those values
    auto precomputedTwoTriplesAndAQuad = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();

    CHECK(precomputedTwoTriplesAndAQuad != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad->isPopulated() == true);

    auto triple1 = precomputedTwoTriplesAndAQuad->get_triple1();

    auto cipherText1 = elgamalEncrypt(0UL, *triple1->get_exp(), *publicKey);
    auto cipherText2 =
      elgamalEncrypt_with_precomputed(0UL, *triple1->get_g_to_exp(), *triple1->get_pubkey_to_exp());

    CHECK((*cipherText1->getPad() == *cipherText2->getPad()));
    CHECK((*cipherText1->getData() == *cipherText2->getData()));

    auto decrypted1 = cipherText1->decrypt(secret);
    CHECK((0UL == decrypted1));
    auto decrypted2 = cipherText2->decrypt(secret);
    CHECK((0UL == decrypted2));
}

TEST_CASE("elgamalEncrypt_with_precomputed simple encrypt 0")
{
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    CHECK((*publicKey < P()));

    // cause a two triples and a quad to be populated
    PrecomputeBufferContext::init(1);
    PrecomputeBufferContext::populate(*keypair->getPublicKey());
    PrecomputeBufferContext::stop_populate();

    // this function runs off to look in the precomputed values buffer and if
    // it finds what it needs the the returned class will contain those values
    auto precomputedTwoTriplesAndAQuad = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();

    CHECK(precomputedTwoTriplesAndAQuad != nullptr);
    CHECK(precomputedTwoTriplesAndAQuad->isPopulated() == true);

    auto triple1 = precomputedTwoTriplesAndAQuad->get_triple1();
    CHECK(triple1 != nullptr);

    auto cipherText =
      elgamalEncrypt_with_precomputed(0UL, *triple1->get_g_to_exp(),
                                      *triple1->get_pubkey_to_exp());

    auto decrypted = cipherText->decrypt(secret);
    CHECK((0UL == decrypted));
}


TEST_CASE("elgamalEncrypt simple encrypt 1 decrypts with secret")
{
    const auto &nonce = ONE_MOD_Q();
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    CHECK((*publicKey < P()));

    auto elem = g_pow_p(ONE_MOD_P());
    CHECK((*elem == G())); // g^1 = g

    auto cipherText = elgamalEncrypt(1UL, nonce, *publicKey);
    CHECK((const_cast<ElementModP &>(G()) == *cipherText->getPad()));

    auto decrypted = cipherText->decrypt(secret);
    CHECK(1UL == decrypted);
}

TEST_CASE("elgamalEncrypt encrypt 1 decrypts with secret")
{
    auto nonce = ElementModQ::fromHex(a_fixed_nonce);
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto *publicKey = keypair->getPublicKey();

    CHECK((*publicKey < P()));

    auto cipherText = elgamalEncrypt(1UL, *nonce, *publicKey);
    auto cipherText2 = elgamalEncrypt(1UL, *nonce, *publicKey);
    auto decrypted = cipherText->decrypt(*secret);
    CHECK(1UL == decrypted);
}

TEST_CASE("elgamalEncrypt_with_precomputed encrypt 1, decrypts with secret")
{
    //auto nonce = ElementModQ::fromHex(a_fixed_nonce);
    auto secret = ElementModQ::fromHex(a_fixed_secret);
    auto keypair = ElGamalKeyPair::fromSecret(*secret);
    auto *publicKey = keypair->getPublicKey();

    CHECK((*publicKey < P()));

    // cause a two triples and a quad to be populated
    PrecomputeBufferContext::init(1);
    PrecomputeBufferContext::populate(*keypair->getPublicKey());
    PrecomputeBufferContext::stop_populate();

    // this function runs off to look in the precomputed values buffer and if
    // it finds what it needs the the returned class will contain those values
    auto precomputedTwoTriplesAndAQuad = PrecomputeBufferContext::getTwoTriplesAndAQuadruple();
    auto triple1 = precomputedTwoTriplesAndAQuad->get_triple1();

    auto cipherText =
      elgamalEncrypt_with_precomputed(1UL, *triple1->get_g_to_exp(), *triple1->get_pubkey_to_exp());

    auto decrypted = cipherText->decrypt(*secret);
    CHECK(1UL == decrypted);
}


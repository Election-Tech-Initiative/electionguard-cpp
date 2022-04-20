#include "../../src/electionguard/log.hpp"
#include "../../src/electionguard/nonces.hpp"
#include "../../src/kremlin/Hacl_Hash.h"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>
#include <electionguard/precompute_buffers.hpp>
#include <stdexcept>

using namespace electionguard;
using namespace std;

using std::wstring;

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
      elgamalEncrypt_with_precomputed(0UL, *triple1->get_g_to_exp(), *triple1->get_pubkey_to_exp());

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

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt data")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};

    uint8_t bytes_to_use[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};

    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto descriptionHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();
    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));

    std::unique_ptr<HashedElGamalCiphertext> HEGResult =
      hashedElgamalEncrypt(plaintext, *nonce, *publicKey, *descriptionHash, false);

    unique_ptr<ElementModQ> hash = HEGResult->crypto_hash();

    unique_ptr<ElementModP> pad = make_unique<ElementModP>(*HEGResult->getPad());
    vector<uint8_t> data = HEGResult->getData();
    vector<uint8_t> mac = HEGResult->getMac();

    unique_ptr<HashedElGamalCiphertext> newHEG = make_unique<HashedElGamalCiphertext>(
      move(pad), HEGResult->getData(), HEGResult->getMac());

    vector<uint8_t> new_plaintext = newHEG->decrypt(*nonce, *publicKey, *descriptionHash, false);

    CHECK(plaintext == new_plaintext);
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt string data with padding")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};

    uint8_t bytes_to_use[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};

    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto descriptionHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    std::wstring plaintext_string(L"ElectionGuard Rocks!");
    vector<uint8_t> plaintext((uint8_t *)&plaintext_string.front(),
                              +(uint8_t *)&plaintext_string.front() +
                                (plaintext_string.size() * 2));

    auto HEGResult = hashedElgamalEncrypt(plaintext, *nonce, *publicKey, *descriptionHash, true);

    auto pad = HEGResult->getPad();
    unique_ptr<ElementModP> p_pad = make_unique<ElementModP>(*pad);
    auto data = HEGResult->getData();
    auto mac = HEGResult->getMac();

    // now lets decrypt
    unique_ptr<HashedElGamalCiphertext> newHEG = make_unique<HashedElGamalCiphertext>(
      move(p_pad), HEGResult->getData(), HEGResult->getMac());

    vector<uint8_t> new_plaintext = newHEG->decrypt(*nonce, *publicKey, *descriptionHash, true);

    CHECK(plaintext == new_plaintext);
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt data failure different nonce")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};
    // one bit difference in the nonce
    uint64_t different_qwords_to_use[4] = {0x0202030405060708, 0x090a0b0c0d0e0f10,
                                           0x1112131415161718, 0x191a1b1c1d1e1f20};
    uint8_t bytes_to_use[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};

    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto different_nonce = make_unique<ElementModQ>(different_qwords_to_use);
    const auto descriptionHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();
    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));
    bool decrypt_failed = false;

    std::unique_ptr<HashedElGamalCiphertext> HEGResult =
      hashedElgamalEncrypt(plaintext, *nonce, *publicKey, *descriptionHash, false);

    unique_ptr<ElementModP> pad = make_unique<ElementModP>(*HEGResult->getPad());
    vector<uint8_t> data = HEGResult->getData();
    vector<uint8_t> mac = HEGResult->getMac();

    unique_ptr<HashedElGamalCiphertext> newHEG = make_unique<HashedElGamalCiphertext>(
      move(pad), HEGResult->getData(), HEGResult->getMac());

    try {
        vector<uint8_t> new_plaintext =
          newHEG->decrypt(*different_nonce, *publicKey, *descriptionHash, false);
    } catch (std::runtime_error &e) {
        decrypt_failed = true;
    }

    CHECK(decrypt_failed);
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt data failure - tampered with ciphertext")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};
    uint8_t bytes_to_use[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};

    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto descriptionHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();
    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));
    bool decrypt_failed = false;

    std::unique_ptr<HashedElGamalCiphertext> HEGResult =
      hashedElgamalEncrypt(plaintext, *nonce, *publicKey, *descriptionHash, false);

    unique_ptr<ElementModP> pad = make_unique<ElementModP>(*HEGResult->getPad());
    vector<uint8_t> data = HEGResult->getData();
    vector<uint8_t> mac = HEGResult->getMac();

    // change a byte of data
    if (data[data.size() / 2] == 0x00) {
        data[data.size() / 2] = 0xff;
    } else {
        data[data.size() / 2] = 0x00;
    }

    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(pad), data, HEGResult->getMac());

    try {
        vector<uint8_t> new_plaintext =
          newHEG->decrypt(*nonce, *publicKey, *descriptionHash, false);
    } catch (std::runtime_error &e) {
        decrypt_failed = true;
    }

    CHECK(decrypt_failed);
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

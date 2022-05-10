#include "../../src/electionguard/log.hpp"
#include "../../src/electionguard/nonces.hpp"
#include "../../src/karamel/internal/Hacl_Hash.h"
#include "utils/constants.hpp"

#include <doctest/doctest.h>
#include <electionguard/constants.h>
#include <electionguard/elgamal.hpp>
#include <electionguard/group.hpp>
#include <electionguard/precompute_buffers.hpp>
#include <electionguard/convert.hpp>
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
    PrecomputeBufferContext::empty_queues();
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

    auto triple1 = precomputedTwoTriplesAndAQuad->get_triple1();
    CHECK(triple1 != nullptr);

    auto cipherText =
      elgamalEncrypt_with_precomputed(0UL, *triple1->get_g_to_exp(), *triple1->get_pubkey_to_exp());

    auto decrypted = cipherText->decrypt(secret);
    CHECK((0UL == decrypted));
    PrecomputeBufferContext::empty_queues();
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
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();
    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));

    std::unique_ptr<HashedElGamalCiphertext> HEGResult = hashedElgamalEncrypt(
      plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, NO_PADDING, false);

    unique_ptr<ElementModQ> hash_of_HEG = HEGResult->crypto_hash();

    unique_ptr<ElementModP> pad = make_unique<ElementModP>(*HEGResult->getPad());
    vector<uint8_t> ciphertext = HEGResult->getData();
    vector<uint8_t> mac = HEGResult->getMac();

    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(pad), HEGResult->getData(), HEGResult->getMac());

    vector<uint8_t> new_plaintext = newHEG->decrypt(secret, *cryptoExtendedBaseHash, false);

    CHECK(plaintext == new_plaintext);
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt data with padding but on boundary")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};

    uint8_t bytes_to_use[BYTES_512] = {0x09};

    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();
    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));

    std::unique_ptr<HashedElGamalCiphertext> HEGResult =
      hashedElgamalEncrypt(plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, BYTES_512, true);

    unique_ptr<ElementModQ> hash_of_HEG = HEGResult->crypto_hash();

    unique_ptr<ElementModP> pad = make_unique<ElementModP>(*HEGResult->getPad());
    vector<uint8_t> ciphertext = HEGResult->getData();
    vector<uint8_t> mac = HEGResult->getMac();
    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(pad), ciphertext, mac);

    CHECK(ciphertext.size() == (BYTES_512 + sizeof(uint16_t)));
    vector<uint8_t> new_plaintext = newHEG->decrypt(secret, *cryptoExtendedBaseHash, true);

    CHECK(plaintext == new_plaintext);
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt string data with padding")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};

    uint8_t bytes_to_use[BYTES_512] = {0x09};
    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    std::wstring plaintext_string(L"ElectionGuard Rocks!");
    vector<uint8_t> plaintext((uint8_t *)&plaintext_string.front(),
                              +(uint8_t *)&plaintext_string.front() +
                                (plaintext_string.size() * 2));

    auto HEGResult =
      hashedElgamalEncrypt(plaintext, *nonce, *publicKey,
          *cryptoExtendedBaseHash, BYTES_512, true);

    unique_ptr<ElementModQ> hash_of_HEG = HEGResult->crypto_hash();

    auto pad = HEGResult->getPad();
    unique_ptr<ElementModP> p_pad = make_unique<ElementModP>(*pad);
    auto ciphertext = HEGResult->getData();
    auto mac = HEGResult->getMac();

    CHECK(ciphertext.size() == (BYTES_512 + sizeof(uint16_t)));

    // now lets decrypt
    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(p_pad), HEGResult->getData(), HEGResult->getMac());

    vector<uint8_t> new_plaintext = newHEG->decrypt(secret, *cryptoExtendedBaseHash, true);

    CHECK(plaintext == new_plaintext);
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt string data with padding and truncate")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};

    uint8_t bytes_to_use[BYTES_512 + 20] = {0x1a};
    uint8_t truncated_bytes[BYTES_512] = {0x1a};

    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));

    auto HEGResult =
      hashedElgamalEncrypt(plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, BYTES_512, true);

    unique_ptr<ElementModQ> hash_of_HEG = HEGResult->crypto_hash();

    auto pad = HEGResult->getPad();
    unique_ptr<ElementModP> p_pad = make_unique<ElementModP>(*pad);
    auto ciphertext = HEGResult->getData();
    auto mac = HEGResult->getMac();

    CHECK(ciphertext.size() == (BYTES_512 + sizeof(uint16_t)));

    // now lets decrypt
    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(p_pad), HEGResult->getData(), HEGResult->getMac());

    vector<uint8_t> new_plaintext = newHEG->decrypt(secret, *cryptoExtendedBaseHash, true);

    vector<uint8_t> plaintext_truncated(truncated_bytes, truncated_bytes + sizeof(truncated_bytes));

    CHECK(plaintext_truncated == new_plaintext);
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt no data")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};

    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    vector<uint8_t> plaintext; // no data in plaintext
    CHECK(plaintext.size() == 0);

    auto HEGResult =
      hashedElgamalEncrypt(plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, BYTES_512, true);

    unique_ptr<ElementModQ> hash_of_HEG = HEGResult->crypto_hash();

    auto pad = HEGResult->getPad();
    unique_ptr<ElementModP> p_pad = make_unique<ElementModP>(*pad);
    auto ciphertext = HEGResult->getData();
    auto mac = HEGResult->getMac();
    CHECK(ciphertext.size() ==
          (BYTES_512 + sizeof(uint16_t))); // two more bytes than max_len input to encrypt

    // now lets decrypt
    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(p_pad), HEGResult->getData(), HEGResult->getMac());

    vector<uint8_t> new_plaintext = newHEG->decrypt(secret, *cryptoExtendedBaseHash, true);
    CHECK(new_plaintext.size() == 0);

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
    const auto different_secret = make_unique<ElementModQ>(different_qwords_to_use);
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();
    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));
    bool decrypt_failed = false;

    std::unique_ptr<HashedElGamalCiphertext> HEGResult = hashedElgamalEncrypt(
      plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, NO_PADDING, false);

    unique_ptr<ElementModP> pad = make_unique<ElementModP>(*HEGResult->getPad());
    vector<uint8_t> ciphertext = HEGResult->getData();
    vector<uint8_t> mac = HEGResult->getMac();

    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(pad), HEGResult->getData(), HEGResult->getMac());

    try {
        vector<uint8_t> new_plaintext =
          newHEG->decrypt(*different_secret, *cryptoExtendedBaseHash, false);
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
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();
    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));
    bool decrypt_failed = false;

    std::unique_ptr<HashedElGamalCiphertext> HEGResult = hashedElgamalEncrypt(
      plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, NO_PADDING, false);

    unique_ptr<ElementModP> pad = make_unique<ElementModP>(*HEGResult->getPad());
    vector<uint8_t> ciphertext = HEGResult->getData();
    vector<uint8_t> mac = HEGResult->getMac();

    // change a byte of ciphertext
    if (ciphertext[ciphertext.size() / 2] == 0x00) {
        ciphertext[ciphertext.size() / 2] = 0xff;
    } else {
        ciphertext[ciphertext.size() / 2] = 0x00;
    }

    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(pad), ciphertext, HEGResult->getMac());

    try {
        vector<uint8_t> new_plaintext = newHEG->decrypt(secret, *cryptoExtendedBaseHash, false);
    } catch (std::runtime_error &e) {
        decrypt_failed = true;
    }

    CHECK(decrypt_failed);
}

TEST_CASE("HashedElGamalCiphertext encrypt failure length cases")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};
    uint8_t bytes_to_use[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};
    uint8_t longer_bytes_to_use[513] = {0x01};
    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();
    vector<uint8_t> plaintext(bytes_to_use, bytes_to_use + sizeof(bytes_to_use));
    vector<uint8_t> longer_plaintext(longer_bytes_to_use,
                                     longer_bytes_to_use + sizeof(longer_bytes_to_use));
    bool encrypt_longer_plaintext_failed = false;
    bool encrypt_no_pad_not_block_length_failed = false;

    try {
        std::unique_ptr<HashedElGamalCiphertext> HEGResult = hashedElgamalEncrypt(
          longer_plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, BYTES_512, false);
    } catch (std::invalid_argument &e) {
        encrypt_longer_plaintext_failed = true;
    }
    CHECK(encrypt_longer_plaintext_failed);

    try {
        std::unique_ptr<HashedElGamalCiphertext> HEGResult = hashedElgamalEncrypt(
          longer_plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, NO_PADDING, false);
    } catch (std::invalid_argument &e) {
        encrypt_no_pad_not_block_length_failed = true;
    }
    CHECK(encrypt_no_pad_not_block_length_failed);
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt string data with padding and precompute")
{
    uint64_t qwords_to_use[4] = {0x0102030405060708, 0x090a0b0c0d0e0f10, 0x1112131415161718,
                                 0x191a1b1c1d1e1f20};

    uint8_t bytes_to_use[32] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,
                                0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
                                0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20};

    const auto nonce = make_unique<ElementModQ>(qwords_to_use);
    const auto cryptoExtendedBaseHash = make_unique<ElementModQ>(qwords_to_use);
    const auto &secret = TWO_MOD_Q();
    auto keypair = ElGamalKeyPair::fromSecret(secret, false);
    auto *publicKey = keypair->getPublicKey();

    std::wstring plaintext_string(L"ElectionGuard Rocks!");
    vector<uint8_t> plaintext((uint8_t *)&plaintext_string.front(),
                              +(uint8_t *)&plaintext_string.front() +
                                (plaintext_string.size() * 2));

    // cause precomputed entries that will be used by the selection
    // encryptions, that should be more than enough and on teardown
    // the rest will be removed.
    PrecomputeBufferContext::init(3);
    PrecomputeBufferContext::populate(*keypair->getPublicKey());
    PrecomputeBufferContext::stop_populate();

    auto HEGResult =
      hashedElgamalEncrypt(plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, BYTES_512, true);

    auto pad = HEGResult->getPad();
    unique_ptr<ElementModP> p_pad = make_unique<ElementModP>(*pad);
    auto ciphertext = HEGResult->getData();
    auto mac = HEGResult->getMac();

    CHECK(ciphertext.size() == (BYTES_512 + sizeof(uint16_t)));

    // now lets decrypt
    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(p_pad), HEGResult->getData(), HEGResult->getMac());

    vector<uint8_t> new_plaintext = newHEG->decrypt(secret, *cryptoExtendedBaseHash, true);

    CHECK(plaintext == new_plaintext);
    PrecomputeBufferContext::empty_queues();
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
    PrecomputeBufferContext::empty_queues();
}

TEST_CASE("HashedElGamalCiphertext encrypt and decrypt with hard coded data")
{
    unique_ptr<ElementModQ> secret =
      ElementModQ::fromHex("094CDA6CEB3332D62438B6D37BBA774D23C420FA019368671AD330AD50456603");
    auto keypair = ElGamalKeyPair::fromSecret(*secret, false);
    auto *publicKey = keypair->getPublicKey();
    string plaintext_string("ElectionGuard Rocks");
    vector<uint8_t> plaintext(&plaintext_string.front(),
                              &plaintext_string.front() + plaintext_string.size());
    const auto nonce =
      ElementModQ::fromHex("46B6532A1AD7B2AFCDDAA30EEE464884883804B46058DB38E76FCDC79EE5C702");
    const auto cryptoExtendedBaseHash =
      ElementModQ::fromHex("6E418518C6C244CA58399C0F47A9C761BAE7B876F8F5360D8D15FCFF26A42BAA");

    std::unique_ptr<HashedElGamalCiphertext> HEGResult = hashedElgamalEncrypt(
      plaintext, *nonce, *publicKey, *cryptoExtendedBaseHash, BYTES_512, false);

    unique_ptr<ElementModQ> hash_of_HEG = HEGResult->crypto_hash();

    unique_ptr<ElementModP> pad = make_unique<ElementModP>(*HEGResult->getPad());
    vector<uint8_t> data = HEGResult->getData();
    vector<uint8_t> mac = HEGResult->getMac();
    
    string hard_coded_pad_string("C102BAB526517D74FE5D5C249E"
      "7F422993C0306C40A9398FBAD01A0D3547B50BDFD77C6EFC187C7B1FD7918A0B3C2A2FB0A3776A7240F9A"
      "75410569379B3D16877B547F52E79542C1129F6E369F2D006D0A1AA3919F0228CA07F5C9A4DFD1118A606"
      "AA4B7000F9EDC65963F130663FD4F7246F7CFE7A38F1E1DC9BC0698CAB881DCD5A75E6D7165B329C28D80"
      "B719D7A2ED50031A2448A4528275FF161F541CFE304A28CBE7193A4BF8676B2D4F2DE68F175C5B4BFD14B"
      "4B1D9868D00E0BD95B6491C96460159DEABF85239B10A7C86B3D975EF58BBF833C6ABFFF223DAF78C1AE4"
      "C6F64D084C4118F3B5A2618628FA18852BAB55DCE95C04FFCBBAF582D75C7B8B830424C74A8F8EACD1543"
      "00FD67CF753EE14FCE94DDED95F1DD2C1386D92B3FF03A9D6EDEE0F67EC80C72E6425B4EA1C17D7B9CC5B"
      "2165905373A4E304496462CE2BA077F195302A39C52F0077CA682BC718074F928040D1A36F585AC187A74"
      "1F51C843C5ED88BC5FB8B86ED96C42BCF84EDF833489D7D3AC407C6D0740CC94BA1D5B885EB430CE8C601"
      "7F8660A6C72F4378BF133AA663DBA36CAB967AAC0F7738478110ECEABAE3E914CB7A796C5394F7DF15094"
      "0BEA43264765B34851ADE4E5F1F213C25DCF66D35BE92611555D8C05ACFDF1AC5CA82B7D7F0D9BE49596F"
      "8B7F3269D9887F40B4BAB5C3D2BA7049B6D2119C3D0D01501836203412869E0");
    CHECK(pad->toHex() == hard_coded_pad_string);
    
    string hard_coded_data_string("F859AA9F57B163A7C661D8B48841C3D1D3CCA437B0132F87E9F1CB9DF"
      "D905718034DF2B83AC5EEB4DCCB695E38E2FA3BD69B8059796FE7D1DA2A331EDB4AC604616620FA9A1893"
      "12694615044FC3047A21EA3393B53FC9B6721772EDE049B58B153DF2295155CDFDED65341B4ACC00F0122"
      "9509D260E77757EDA929ADC8398AAE65F7634B13716E6598C19D2827D8ACCDB1B1EE427EE50A404C79BB5"
      "4E09EF503A50A939177D689FF60A6F60B3FC6B8319EC17FA424F0461949CD49B764C6360AC0D492696E43"
      "EE83A6A7CE7AEA4DDBA206F365AA81E918F63709DE796F0338CCD311360D97CDC821506D3EDB434922264"
      "966B8AF7E304A403E18384DDCF53AEF1FFC19A66FBCD9C2D04EFC8F2D456BE52DB9C460E3CA10AC4ABFE0"
      "B726E19A715546F1CD9CA89C57ED52DA9D78C30BEFE5FE99A8BDEA33B7C06EDFD4E92D514661CD55B99B5"
      "4E5C468118E16F4827F78FB381845B093F202111E3B84CFCF8DAEE7948BA57698475F3EBC3729559835BF"
      "63AAB0F5659019965A2F0CF55E953B1CD37BCBED8EA0D5F161D461E03031BA7D0B042B978F7F6776DDFBC"
      "AA7145DE30BA24C29BDFA05C7CCF54D7DD58E75143A16F8619053FCF4DE7BDCCA031F0873A65ACCC56FE7"
      "8F32B8FC192D2106CF1A1E5339A5C5657E6703D7F30F908CEEF05A84C67C426B187CBC1599FB334307146"
      "EAECB16774C5CB7630F4CB093E840086");
    CHECK(bytes_to_hex(data) == hard_coded_data_string);

    string hard_coded_mac_string("154BD2692ECBB3D89A64E07D3D166979835508035B116BDEA2BE2EF607"
      "74EA77");
    CHECK(bytes_to_hex(mac) == hard_coded_mac_string);

    unique_ptr<HashedElGamalCiphertext> newHEG =
      make_unique<HashedElGamalCiphertext>(move(pad), HEGResult->getData(), HEGResult->getMac());

    vector<uint8_t> new_plaintext = newHEG->decrypt(*secret, *cryptoExtendedBaseHash, true);

    CHECK(plaintext == new_plaintext);
}

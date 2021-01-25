#ifndef __ELECTIONGUARD_CORE_SERIALIZE_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_SERIALIZE_HPP_INCLUDED__

#include "../kremlin/Hacl_HMAC_DRBG.h"
#include "../kremlin/Lib_RandomBuffer_System.h"
#include "convert.hpp"
#include "log.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

namespace electionguard
{
    enum ByteSize { SHA256 = 256, SHA384 = 384, SHA512 = 512 };
    class Random
    {
      public:
        /// https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90Ar1.pdf
        static vector<uint8_t> getBytes(ByteSize size = SHA256)
        {
            auto entropy = getRandomBytes(size * 2);
            Hacl_HMAC_DRBG_state state = Hacl_HMAC_DRBG_create_in(hashAlgo);

            auto nonce = getRandomBytes(size);
            auto personalization = getTime();
            Hacl_HMAC_DRBG_instantiate(hashAlgo, state, entropy.size(), entropy.data(),
                                       nonce.size(), nonce.data(), personalization.size(),
                                       reinterpret_cast<uint8_t *>(personalization.data()));

            auto *array = new uint8_t[size];
            auto input = getRandomBytes(size);
            if (Hacl_HMAC_DRBG_generate(hashAlgo, array, state, size, input.size(), input.data())) {
                vector<uint8_t> result(array, array + size);
                cleanup(state, entropy, nonce);
                delete[] array;
                return result;
            } else {
                cleanup(state, entropy, nonce);
                delete[] array;
                throw bad_alloc();
            }
        }

      private:
        static void cleanup(Hacl_HMAC_DRBG_state &state, vector<uint8_t> entropy,
                            vector<uint8_t> nonce)
        {
            release(entropy);
            release(nonce);
            free(state.k);
            free(state.v);
            free(state.reseed_counter);
        }
        static vector<uint8_t> getRandomBytes(uint32_t count = 256)
        {
            // TODO: use unique_ptr or vector instead of direct heap allocation
            auto *array = new uint8_t[count];
            if (Lib_RandomBuffer_System_randombytes(const_cast<uint8_t *>(array), count)) {
                vector<uint8_t> result(array, array + count);
                delete[] array;
                return result;

            } else {
                delete[] array;
                throw bad_alloc();
            }
        }
        static string getTime()
        {
            auto now_seconds = time(nullptr);
            struct tm now;

            stringstream stream;
#ifdef _WIN32
            // TODO: handle error
            gmtime_s(&now, &now_seconds);
            stream << put_time(&now, "%c");
#else
            stream << put_time(gmtime_r(&now_seconds, &now), "%c");
#endif
            return stream.str();
        }
        static const Spec_Hash_Definitions_hash_alg hashAlgo =
          (Spec_Hash_Definitions_hash_alg)Spec_Hash_Definitions_SHA2_256;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_SERIALIZE_HPP_INCLUDED__ */

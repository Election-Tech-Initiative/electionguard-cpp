#ifndef __ELECTIONGUARD_CPP_SERIALIZE_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_SERIALIZE_HPP_INCLUDED__

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
    enum ByteSize {
        CHAR = 8,
        SHORT = 16,
        INT = 32,
        LONG = 64,
        SHA256 = 256,
        SHA384 = 384,
        SHA512 = 512
    };

    /// <summary>
    /// A convenience wrapper around the HACL* HMAC Deterministic Random Bit Generator
    /// that supports retrieving an arbitrary number of pseudo-random bytes as specified.
    ///
    /// This implementation currently does not support reseeding the same instantiation
    /// and expects the caller to initialize a new instance before the entropy pool is exhausted
    ///
    /// Please refer to the NIST publication for more information:
    /// https://nvlpubs.nist.gov/nistpubs/SpecialPublications/NIST.SP.800-90Ar1.pdf
    /// </summary>
    class Random
    {
      public:
        /// <summary>
        /// Get pseudo-random bytes for the specified ByteSize.
        ///
        /// Note:
        /// This implmenetation expects the OS random source to have at least `size * 2`
        /// available entropy to seed the DRBG properly.  Consumers should consider the
        /// available entropy before calling this function.
        /// </summary>
        static vector<uint8_t> getBytes(ByteSize size = SHA256)
        {
            // Get some random bytes from the operating system
            auto entropy = getRandomBytes(size * 2);

            // Allocate the DRBG
            Hacl_HMAC_DRBG_state state = Hacl_HMAC_DRBG_create_in(hashAlgo);

            // Derive a nonce from the OS entropy pool
            auto nonce = getRandomBytes(size);

            // Derive a personalization string from the system clock
            auto personalization = getTime();

            // Instantiate the DRBG
            Hacl_HMAC_DRBG_instantiate(hashAlgo, state, entropy.size(), entropy.data(),
                                       nonce.size(), nonce.data(), personalization.size(),
                                       reinterpret_cast<uint8_t *>(personalization.data()));

            auto *array = new uint8_t[size];
            auto input = getRandomBytes(size);

            // Try to generate some random bits
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

        /// <summary>
        /// Get random bytes from the operating system using the default generator
        ///
        /// Depending on the OS, this may be a blocking or non-blocking call
        /// see: https://github.com/project-everest/hacl-star/blob/master/dist/election-guard/Lib_RandomBuffer_System.c
        static vector<uint8_t> getRandomBytes(uint32_t count = 256)
        {
            // TODO: ISSUE #137: use unique_ptr or vector instead of a direct heap allocation
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

        // statically pin the hashing algorithm to SHA256
        static const Spec_Hash_Definitions_hash_alg hashAlgo =
          (Spec_Hash_Definitions_hash_alg)Spec_Hash_Definitions_SHA2_256;
    };
} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_SERIALIZE_HPP_INCLUDED__ */

#include "electionguard/hmac.hpp"

#include "../karamel/Lib_Memzero0.h"
#include "../karamel/internal/Hacl_HMAC.h"
#include "log.hpp"

#include <iomanip>
#include <iostream>

using std::get;
using std::make_unique;
using std::nullptr_t;
using std::out_of_range;
using std::reference_wrapper;
using std::string;
using std::to_string;
using std::unique_ptr;
using std::vector;

namespace electionguard
{
    vector<uint8_t> get_hmac(std::vector<uint8_t> key, std::vector<uint8_t> message,
                             uint32_t length, uint32_t start)
    {
        vector<uint8_t> hmac(32, 0);
        vector<uint8_t> data_to_hmac;

        if (length > 0) {
            data_to_hmac.insert(data_to_hmac.end(), (uint8_t *)&start,
                                (uint8_t *)&start + sizeof(start));
            data_to_hmac.insert(data_to_hmac.end(), message.begin(), message.end());
            data_to_hmac.insert(data_to_hmac.end(), (uint8_t *)&length,
                                (uint8_t *)&length + sizeof(length));
        } else {
            data_to_hmac = message;
        }

        // calculate the hmac and then zeroize the buffer holding the data we hmaced
        Hacl_HMAC_compute_sha2_256(&hmac.front(), &key.front(), key.size(), &data_to_hmac.front(),
                                   data_to_hmac.size());
        Lib_Memzero0_memzero(&data_to_hmac.front(), data_to_hmac.size());

        return hmac;
    }

} // namespace electionguard

#ifndef __ELECTIONGUARD_CPP_WORDS_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_WORDS_HPP_INCLUDED__

#include <electionguard/export.h>
#include <electionguard/group.hpp>
#include <string>

extern "C" {
#include <electionguard/group.h>
#include <electionguard/status.h>
}

namespace electionguard::test::mocks
{
    EG_API std::string getWord(uint16_t index);

    EG_API int16_t getIndex(const char *word);
    EG_API int16_t getIndex(const std::string &word);

    EG_API std::string hashToWords(const ElementModQ &trackerHash, const char *separator = " ");
} // namespace electionguard::test::mocks

#ifdef __cplusplus
extern "C" {
#endif

EG_API eg_electionguard_status_t
eg_hash_to_words_with_default_separator(eg_element_mod_q_t *in_tracker_hash, char **out_hash_words);
EG_API eg_electionguard_status_t eg_hash_to_words(eg_element_mod_q_t *in_tracker_hash,
                                                  const char *in_separator, char **out_hash_words);

#ifdef __cplusplus
}
#endif

#endif /* __ELECTIONGUARD_CPP_WORDS_HPP_INCLUDED__ */

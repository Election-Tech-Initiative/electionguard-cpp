#ifndef __ELECTIONGUARD_CPP_WORDS_HPP_INCLUDED__
#define __ELECTIONGUARD_CPP_WORDS_HPP_INCLUDED__

#include <electionguard/export.h>
#include <string>

using std::string;

namespace electionguard
{
    EG_INTERNAL_API string getWord(uint16_t index);

    EG_INTERNAL_API int16_t getIndex(const char *word);
    EG_INTERNAL_API int16_t getIndex(const string &word);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CPP_WORDS_HPP_INCLUDED__ */

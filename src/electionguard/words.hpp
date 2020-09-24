#ifndef __ELECTIONGUARD_CORE_WORDS_HPP_INCLUDED__
#define __ELECTIONGUARD_CORE_WORDS_HPP_INCLUDED__

#include <electionguard/export.h>
#include <string>

using std::string;

namespace electionguard
{
    EG_INTERNAL_API const string getWord(uint16_t index);

    EG_INTERNAL_API int16_t getIndex(const char *word);
    EG_INTERNAL_API int16_t getIndex(const string &word);
} // namespace electionguard

#endif /* __ELECTIONGUARD_CORE_WORDS_HPP_INCLUDED__ */

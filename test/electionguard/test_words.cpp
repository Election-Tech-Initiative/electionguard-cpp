#include <doctest/doctest.h>
#include <electionguard/log.hpp>
#include <electionguard/words.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace electionguard;
using namespace std;

TEST_CASE("Retrieving words from the words list succeeds")
{
    // Arrange
    uint16_t INDEX_MIN = 0;
    uint16_t INDEX_RANDOM_1 = 100;
    uint16_t INDEX_RANDOM_2 = 1000;
    uint16_t INDEX_MAX = 4095;

    // Act
    auto word_min = getWord(INDEX_MIN);

    // Assert
    CHECK(word_min == "aardvark");
}

TEST_CASE("Retrieving indicies from the words list succeeds")
{
    // Arrange
    const char *as_c_str = "thiswordshouldfail";
    string as_std_str = "thiswordshouldfail";

    // Act
    auto cstr_index = getIndex(as_c_str);
    auto stdstr_index = getIndex(as_std_str);

    // Assert
    CHECK(cstr_index == -1);
    CHECK(stdstr_index == -1);
}
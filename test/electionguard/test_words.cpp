#include "mocks/words.hpp"

#include <doctest/doctest.h>
#include <electionguard/ballot_code.hpp>
#include <electionguard/log.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace electionguard;
using namespace electionguard::test::mocks;
using namespace std;

TEST_SUITE("Demonstrate a method for making friendly names from hash codes")
{

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

    TEST_CASE("Hash to words converts to words")
    {
        // Arrange
        auto deviceHash =
          BallotCode::getHashForDevice(12345UL, 23456UL, 34567UL, "some-location-string");
        const auto &ballotHash = ONE_MOD_Q();
        const auto &anotherBallotHash = TWO_MOD_Q();
        uint64_t timestamp = 1000;
        auto firstRotatingHash =
          BallotCode::getRotatingBallotCode(*deviceHash, timestamp, ballotHash);
        auto secondRotatingHash =
          BallotCode::getRotatingBallotCode(*deviceHash, timestamp, anotherBallotHash);

        // Act
        auto deviceWords = hashToWords(*deviceHash);
        auto firstRotatingWords = hashToWords(*firstRotatingHash);
        auto secondRotatingWords = hashToWords(*secondRotatingHash);

        // Assert
        CHECK(deviceWords.empty() == false);
        CHECK(deviceWords != firstRotatingWords);
        CHECK(deviceWords != secondRotatingWords);
        CHECK(firstRotatingWords != secondRotatingWords);
    }

    TEST_CASE("Hash to words converts to known words")
    {
        // Arrange
        const auto expectedHash =
          "325AB2622D35311DB0320C9F3B421EE93017D16B9E4C7FEF06704EDA4FA5E30B";
        const auto expectedWords =
          "civilian-AB262-championship-5311D-maybe-20C9F-configuration-21EE9-"
          "chipmunk-7D16B-lambkin-C7FEF-allergist-04EDA-disclosure-5E30B";
        const auto &deviceHash = ONE_MOD_Q();
        const auto &ballotHash = TWO_MOD_Q();
        uint64_t timestamp = 1000;
        auto ballotCode = BallotCode::getRotatingBallotCode(deviceHash, timestamp, ballotHash);

        // Act
        auto trackerWords = hashToWords(*ballotCode, "-");

        // Assert
        CHECK(trackerWords.empty() == false);
        CHECK(ballotCode->toHex() == expectedHash);
        CHECK(trackerWords == expectedWords);
    }
}
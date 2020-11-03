#include <doctest/doctest.h>
#include <electionguard/log.hpp>
#include <electionguard/tracker.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace electionguard;

TEST_CASE("Get rotating tracker hash rotates")
{
    // Arrange
    auto deviceHash = Tracker::getHashForDevice(1234UL, "some-location-string");
    uint64_t firstHash[MAX_Q_LEN] = {1, 2, 3, 4};
    auto firstBallotHash = make_unique<ElementModQ>(firstHash);
    uint64_t secondHash[MAX_Q_LEN] = {2, 3, 4, 5};
    auto secondBallotHash = make_unique<ElementModQ>(secondHash);

    // Act
    auto rotatingHash1 = Tracker::getRotatingTrackerHash(*deviceHash, 1000UL, *firstBallotHash);
    auto rotatingHash2 = Tracker::getRotatingTrackerHash(*rotatingHash1, 1001UL, *secondBallotHash);

    //Assert
    CHECK(deviceHash != nullptr);
    CHECK(rotatingHash1 != nullptr);
    CHECK(rotatingHash2 != nullptr);

    CHECK(rotatingHash1 != deviceHash);
    CHECK(rotatingHash2 != deviceHash);
    CHECK(rotatingHash1 != rotatingHash2);

    CHECK((*rotatingHash1 != *deviceHash));
    CHECK(&rotatingHash1 != &deviceHash);

    CHECK((*rotatingHash2 != *deviceHash));
    CHECK(&rotatingHash2 != &deviceHash);

    CHECK((*rotatingHash1 != *rotatingHash2));
    CHECK(&rotatingHash1 != &rotatingHash2);
}

TEST_CASE("Hash to words converts to words")
{
    // Arrange
    auto deviceHash = Tracker::getHashForDevice(1234UL, "some-location-string");
    const auto &ballotHash = ONE_MOD_Q();
    const auto &anotherBallotHash = TWO_MOD_Q();
    uint64_t timestamp = 1000;
    auto firstRotatingHash = Tracker::getRotatingTrackerHash(*deviceHash, timestamp, ballotHash);
    auto secondRotatingHash =
      Tracker::getRotatingTrackerHash(*deviceHash, timestamp, anotherBallotHash);

    // Act
    auto deviceWords = Tracker::hashToWords(*deviceHash);
    auto firstRotatingWords = Tracker::hashToWords(*firstRotatingHash);
    auto secondRotatingWords = Tracker::hashToWords(*secondRotatingHash);

    // Assert
    CHECK(deviceWords.empty() == false);
    CHECK(deviceWords != firstRotatingWords);
    CHECK(deviceWords != secondRotatingWords);
    CHECK(firstRotatingWords != secondRotatingWords);
}

TEST_CASE("Hash to words converts to known words")
{
    // Arrange
    const auto expectedHash = "325AB2622D35311DB0320C9F3B421EE93017D16B9E4C7FEF06704EDA4FA5E30B";
    const auto expectedWords = "civilian-AB262-championship-5311D-maybe-20C9F-configuration-21EE9-"
                               "chipmunk-7D16B-lambkin-C7FEF-allergist-04EDA-disclosure-5E30B";
    const auto &deviceHash = ONE_MOD_Q();
    const auto &ballotHash = TWO_MOD_Q();
    uint64_t timestamp = 1000;
    auto trackerHash = Tracker::getRotatingTrackerHash(deviceHash, timestamp, ballotHash);

    // Act
    auto trackerWords = Tracker::hashToWords(*trackerHash, "-");

    // Assert
    CHECK(trackerWords.empty() == false);
    CHECK(trackerHash->toHex() == expectedHash);
    CHECK(trackerWords == expectedWords);
}

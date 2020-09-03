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
    auto *deviceHash = Tracker::getHashForDevice(1234, "some-location-string");
    uint64_t firstHash[4] = {1, 2, 3, 4};
    auto firstBallotHash = new ElementModQ(firstHash);
    uint64_t secondHash[4] = {2, 3, 4, 5};
    auto secondBallotHash = new ElementModQ(secondHash);

    // Act
    auto *rotatingHash1 = Tracker::getRotatingTrackerHash(deviceHash, 1000, firstBallotHash);
    auto *rotatingHash2 = Tracker::getRotatingTrackerHash(rotatingHash1, 1001, secondBallotHash);

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

// todo: TEST_CASE("Hash to words converts to words")
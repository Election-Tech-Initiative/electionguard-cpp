#include <doctest/doctest.h>
#include <electionguard/log.hpp>
#include <electionguard/tracker.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace electionguard;
using namespace std;

TEST_CASE("Get rotating tracker hash rotates")
{
    // Arrange
    auto deviceHash = Tracker::getHashForDevice(12345UL, 23456UL, 34567UL, "some-location-string");
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

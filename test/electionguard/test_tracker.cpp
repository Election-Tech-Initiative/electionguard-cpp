#include <doctest/doctest.h>
#include <electionguard/log.hpp>
#include <electionguard/tracker.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

TEST_CASE("Get rotating tracker hash rotates")
{
    // Arrange
    auto deviceHash = getHashForDevice(1234, "some-location");

    // Act
    auto rotatingHash1 = getRotatingTrackerHash(deviceHash, 1000, 2345);
    auto rotatingHash2 = getRotatingTrackerHash(rotatingHash1, 1001, 3456);

    //Assert
    CHECK(deviceHash != nullptr);
    CHECK(rotatingHash1 != nullptr);
    CHECK(rotatingHash2 != nullptr);

    CHECK(rotatingHash1 != deviceHash);
    CHECK(rotatingHash2 != deviceHash);
    CHECK(rotatingHash1 != rotatingHash2);
}

// todo: TEST_CASE("Hash to words converts to words")
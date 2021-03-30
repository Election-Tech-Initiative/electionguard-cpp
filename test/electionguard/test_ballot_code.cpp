#include <doctest/doctest.h>
#include <electionguard/ballot_code.hpp>
#include <electionguard/log.hpp>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace electionguard;
using namespace std;

TEST_CASE("Get rotating ballot code rotates")
{
    // Arrange
    auto deviceHash =
      BallotCode::getHashForDevice(12345UL, 23456UL, 34567UL, "some-location-string");
    uint64_t firstHash[MAX_Q_LEN] = {1, 2, 3, 4};
    auto firstBallotHash = make_unique<ElementModQ>(firstHash);
    uint64_t secondHash[MAX_Q_LEN] = {2, 3, 4, 5};
    auto secondBallotHash = make_unique<ElementModQ>(secondHash);

    // Act
    auto rotatingHash1 = BallotCode::getRotatingBallotCode(*deviceHash, 1000UL, *firstBallotHash);
    auto rotatingHash2 =
      BallotCode::getRotatingBallotCode(*rotatingHash1, 1001UL, *secondBallotHash);

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

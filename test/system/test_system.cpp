#include <fstream>
#include <vector>
#include <tuple>
#include "config.h"
#include "gtest/gtest.h"
#include "manager.h"

#ifdef TEST_THOROUGH
std::vector<std::tuple<int, int, double, int>> loadParams()
{
    std::vector<int> numberOfTrajectories {4, 6, 8, 10, 12, 14, 16, 18, 20};
    std::vector<int> flockLength {4, 6, 8, 10, 12, 14, 16, 18, 20};
    std::vector<double> gridSize {0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5};
    std::vector<std::tuple<int, int, double, int>> params;

    std::ifstream file("test_data/thorough_result", std::ifstream::in);
    std::string line;

    for (int n : numberOfTrajectories) {
        for (int l : flockLength) {
            for (double g : gridSize) {
                std::getline(file, line);
                params.push_back(std::make_tuple(n, l, g, atoi(line.c_str())));
            }
        }
    }
    return params;
}

class ThoroughTest : public ::testing::TestWithParam<std::tuple<int, int, double, int>> {
    void SetUp() override {
        Config::setDecoderParameters(std::vector<std::string>{"test_data/trucks_by_time.txt"});
        Config::setDecoder("truckspaper");
        Config::setTimeSlotSize(1);
        Config::setCompatibilityMode(true);
        Config::setCoordinateSystem(Config::Metric);
    }
};

TEST_P(ThoroughTest, Thoroughly) {
    // given
    int numberOfTrajectoriesPerFlock;
    int flockLength;
    double gridSize;
    int expectedResult;
    std::tie(numberOfTrajectoriesPerFlock, flockLength, gridSize, expectedResult) = GetParam();
    Config::setNumberOfTrajectoriesPerFlock(numberOfTrajectoriesPerFlock);
    Config::setFlockLength(flockLength);
    Config::setGridSize(gridSize);

    // when
    Manager manager;
    manager.start();

    // then
    ASSERT_EQ(expectedResult, manager.foundFlocks());
}

INSTANTIATE_TEST_CASE_P(System, ThoroughTest, ::testing::ValuesIn(loadParams()));

#else

TEST(System, Find_35218_Flocks)
{
    // given
    Config::setNumberOfTrajectoriesPerFlock(4);
    Config::setFlockLength(6);
    Config::setGridSize(1.5);
    Config::setTimeSlotSize(1);
    Config::setDecoder("truckspaper");
    Config::setCompatibilityMode(true);
    Config::setCoordinateSystem(Config::Metric);
    Config::setDecoderParameters(std::vector<std::string>{"test_data/trucks_by_time.txt"});

    // when
    Manager manager;
    manager.start();

    // then
    ASSERT_EQ(35218, manager.foundFlocks());
}

#endif  // TEST_THOROUGH

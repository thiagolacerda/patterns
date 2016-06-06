#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <tuple>
#include <vector>
#include "gtest/gtest.h"
#include "orchestrator.h"

void assertFlocks(unsigned n, unsigned l, double g, unsigned flocks)
{
    // get all stdout output
    std::streambuf* oldCoutStreamBuf = std::cout.rdbuf();
    std::ostringstream strCout;
    std::cout.rdbuf(strCout.rdbuf());

    std::unordered_map<std::string, std::string> confMap = {
        {"connector", "f"},
        {"decoder", "t"},
        {"listener", "g"},
        {"processor", "f"},
        {"cp", "file=data/trucks_by_time.txt separator=\\t"},
        {"dp", ""},
        {"lp", "timeSlotSize=1 patternLength=" + std::to_string(l)},
        {"pp", "gridSize=" + std::to_string(g) + " trajectoriesPerFlock=" + std::to_string(n) + " flockLength=" +
            std::to_string(l)}
    };

    // when
    Orchestrator orchestrator;
    orchestrator.loadConfigFromMapAndRegisterComponents(confMap);
    orchestrator.start();

    // then
    // restore old cout.
    std::cout.rdbuf(oldCoutStreamBuf);

    std::regex flocksRegex("Flocks found: (\\d+)");
    std::smatch flockMatch;
    std::regex_search(strCout.str(), flockMatch, flocksRegex);

    ASSERT_EQ(flocks, std::stoul(flockMatch[1].str()));
}

#ifdef TEST_THOROUGH
std::vector<std::tuple<unsigned, unsigned, double, unsigned>> loadParams()
{
    std::vector<unsigned> numberOfTrajectories {4, 6, 8, 10, 12, 14, 16, 18, 20};
    std::vector<unsigned> flockLength {4, 6, 8, 10, 12, 14, 16, 18, 20};
    std::vector<double> gridSize {0.8, 0.9, 1, 1.1, 1.2, 1.3, 1.4, 1.5};
    std::vector<std::tuple<unsigned, unsigned, double, unsigned>> params;

    std::ifstream file("data/thorough_result", std::ifstream::in);
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

// class is needed for parameter test
class SystemTest : public ::testing::TestWithParam<std::tuple<unsigned, unsigned, double, unsigned>> {
};

TEST_P(SystemTest, thoroughTest) {
    // given
    unsigned n;
    unsigned l;
    double g;
    unsigned expected;
    std::tie(n, l, g, expected) = GetParam();

    assertFlocks(n, l, g, expected);
}

INSTANTIATE_TEST_CASE_P(ThoroughTest, SystemTest, ::testing::ValuesIn(loadParams()));

#else

TEST(System, find_35218_Flocks)
{
    std::unordered_map<std::string, std::string> confMap = {
        {"connector", "f"},
        {"decoder", "t"},
        {"listener", "g"},
        {"processor", "f"},
        {"cp", "file=data/trucks_by_time.txt separator=\\t"},
        {"dp", ""},
        {"lp", "timeSlotSize=1 patternLength=6"},
        {"pp", "gridSize=1.5 trajectoriesPerFlock=4 flockLength=6"}
    };

    assertFlocks(4, 6, 1.5, 35214);
}

#endif  // TEST_THOROUGH

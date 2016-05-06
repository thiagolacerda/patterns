#include <math.h>
#include "config.h"
#include "gpspoint.h"
#include "gtest/gtest.h"
#include "utils.h"

static constexpr double LATITUDE_TEST = 38.018470;
static constexpr double LONGITUDE_TEST = 23.845089;
static constexpr double LATITUDE_METERS_TEST = 4203597.2;
static constexpr double LONGITUDE_METERS_TEST = 2654452.0;

double roundToDecimalPlaces(double number, int decimalPlaces)
{
    double multiplier = pow(10, decimalPlaces);
    return round(number * multiplier) / multiplier;
}

class UtilsTest : public ::testing::TestWithParam<std::pair<std::string, std::string>> {
public:
    void TearDown() override
    {
        Config::reset();
    }
};

TEST(UtilsTest, fuzzyEqual)
{
    ASSERT_TRUE(Utils::fuzzyEqual(3.999, 4));
}

TEST(UtilsTest, latLongToMeters)
{
    // given
    double latMeters = 0;
    double longMeters = 0;

    // when
    Utils::latLongToMeters(LATITUDE_TEST, LONGITUDE_TEST, latMeters, longMeters);

    // then
    ASSERT_EQ(LATITUDE_METERS_TEST, roundToDecimalPlaces(latMeters, 1));
    ASSERT_EQ(LONGITUDE_METERS_TEST, roundToDecimalPlaces(longMeters, 1));
}

TEST(UtilsTest, latLongToMeters_negativeLatitude)
{
    // given
    double latMeters = 0;
    double longMeters = 0;

    // when
    Utils::latLongToMeters(-LATITUDE_TEST, LONGITUDE_TEST, latMeters, longMeters);

    // then
    ASSERT_EQ(-LATITUDE_METERS_TEST, roundToDecimalPlaces(latMeters, 1));
    ASSERT_EQ(LONGITUDE_METERS_TEST, roundToDecimalPlaces(longMeters, 1));
}

TEST(UtilsTest, latLongToMeters_negativeLongitude)
{
    // given
    double latMeters = 0;
    double longMeters = 0;

    // when
    Utils::latLongToMeters(LATITUDE_TEST, -LONGITUDE_TEST, latMeters, longMeters);

    // then
    ASSERT_EQ(LATITUDE_METERS_TEST, roundToDecimalPlaces(latMeters, 1));
    ASSERT_EQ(-LONGITUDE_METERS_TEST, roundToDecimalPlaces(longMeters, 1));
}

TEST(UtilsTest, latLongToMeters_allNegative)
{
    // given
    double latMeters = 0;
    double longMeters = 0;

    // when
    Utils::latLongToMeters(-LATITUDE_TEST, -LONGITUDE_TEST, latMeters, longMeters);

    // then
    ASSERT_EQ(-LATITUDE_METERS_TEST, roundToDecimalPlaces(latMeters, 1));
    ASSERT_EQ(-LONGITUDE_METERS_TEST, roundToDecimalPlaces(longMeters, 1));
}

TEST(UtilsTest, metersToLatLong)
{
    // given
    double latitude = 0;
    double longitude = 0;

    // when
    Utils::metersToLatLong(LATITUDE_METERS_TEST, LONGITUDE_METERS_TEST, latitude, longitude);

    // then
    ASSERT_EQ(LATITUDE_TEST, roundToDecimalPlaces(latitude, 6));
    ASSERT_EQ(LONGITUDE_TEST, roundToDecimalPlaces(longitude, 6));
}

TEST(UtilsTest, metersToLatLong_negativeLatitude)
{
    // given
    double latitude = 0;
    double longitude = 0;

    // when
    Utils::metersToLatLong(-LATITUDE_METERS_TEST, LONGITUDE_METERS_TEST, latitude, longitude);

    // then
    ASSERT_EQ(-LATITUDE_TEST, roundToDecimalPlaces(latitude, 6));
    ASSERT_EQ(LONGITUDE_TEST, roundToDecimalPlaces(longitude, 6));
}

TEST(UtilsTest, metersToLatLong_negativeLongitude)
{
    // given
    double latitude = 0;
    double longitude = 0;

    // when
    Utils::metersToLatLong(LATITUDE_METERS_TEST, -LONGITUDE_METERS_TEST, latitude, longitude);

    // then
    ASSERT_EQ(LATITUDE_TEST, roundToDecimalPlaces(latitude, 6));
    ASSERT_EQ(-LONGITUDE_TEST, roundToDecimalPlaces(longitude, 6));
}

TEST(UtilsTest, metersToLatLong_allNegative)
{
    // given
    double latitude = 0;
    double longitude = 0;

    // when
    Utils::metersToLatLong(-LATITUDE_METERS_TEST, -LONGITUDE_METERS_TEST, latitude, longitude);

    // then
    ASSERT_EQ(-LATITUDE_TEST, roundToDecimalPlaces(latitude, 6));
    ASSERT_EQ(-LONGITUDE_TEST, roundToDecimalPlaces(longitude, 6));
}

TEST(UtilsTest, interpolate)
{
    // given
    Config::setCoordinateSystem(Config::WSG84);
    uint64_t timestamp1 = 12435690;
    uint64_t timestamp2 = 14481908;
    uint32_t trajectoryId = 6;
    GPSPoint p1(LATITUDE_TEST, LONGITUDE_TEST, timestamp1, trajectoryId);

    double p2Lat = LATITUDE_TEST + 1.3;
    double p2Long = LONGITUDE_TEST + 1.21;
    GPSPoint p2(p2Lat, p2Long, timestamp2, trajectoryId);

    double interpolatedLat = 38.67;
    double interpolatedLong = 24.4446;
    uint64_t midTimestamp = (timestamp1 + timestamp2) / 2;
    GPSPoint expected(interpolatedLat, interpolatedLong, midTimestamp, trajectoryId);

    // when
    std::shared_ptr<GPSPoint> actual = Utils::interpolate(p1, p2);

    // then
    ASSERT_EQ(expected, *actual.get());
}

TEST_P(UtilsTest, testTrimWithParam)
{
    const auto& leftRightExtra = GetParam();
    const std::string tested = "nospaces";
    const std::string withExtra = std::get<0>(leftRightExtra) + tested + std::get<1>(leftRightExtra);
    ASSERT_EQ(tested, Utils::trim(withExtra));
}

std::vector<std::pair<std::string, std::string>> loadParams()
{
    const auto toTrim = {"", " ", "\n", "\f", "\t", "\v", "\r"};
    std::vector<std::pair<std::string, std::string>> params;
    for (const auto& p1 : toTrim) {
        for (const auto& p2 : toTrim)
            params.push_back(std::make_pair(p1, p2));
    }

    // some more
    params.push_back(std::make_pair(" \t\f\v\n\r", " \t\f\v\n\r"));
    return params;
}

INSTANTIATE_TEST_CASE_P(trimTest, UtilsTest, ::testing::ValuesIn(loadParams()));

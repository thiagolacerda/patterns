#include <math.h>
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
    Utils::latLongToMeters(LATITUDE_TEST, LONGITUDE_TEST, &latMeters, &longMeters);

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
    Utils::latLongToMeters(-LATITUDE_TEST, LONGITUDE_TEST, &latMeters, &longMeters);

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
    Utils::latLongToMeters(LATITUDE_TEST, -LONGITUDE_TEST, &latMeters, &longMeters);

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
    Utils::latLongToMeters(-LATITUDE_TEST, -LONGITUDE_TEST, &latMeters, &longMeters);

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
    Utils::metersToLatLong(LATITUDE_METERS_TEST, LONGITUDE_METERS_TEST, &latitude, &longitude);

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
    Utils::metersToLatLong(-LATITUDE_METERS_TEST, LONGITUDE_METERS_TEST, &latitude, &longitude);

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
    Utils::metersToLatLong(LATITUDE_METERS_TEST, -LONGITUDE_METERS_TEST, &latitude, &longitude);

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
    Utils::metersToLatLong(-LATITUDE_METERS_TEST, -LONGITUDE_METERS_TEST, &latitude, &longitude);

    // then
    ASSERT_EQ(-LATITUDE_TEST, roundToDecimalPlaces(latitude, 6));
    ASSERT_EQ(-LONGITUDE_TEST, roundToDecimalPlaces(longitude, 6));
}

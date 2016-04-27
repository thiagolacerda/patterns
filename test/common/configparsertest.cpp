#include <fstream>
#include <cstdio>
#include <string>
#include <unordered_map>
#include "configparser.h"
#include "gtest/gtest.h"
#include <iostream>

class ConfigParserTest : public ::testing::Test {
public:
    void TearDown() override;

protected:
    void writeConfigFile(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& config);
    void writeSection(const std::string& section, const std::unordered_map<std::string, std::string>& values,
        std::ofstream& testInputFile);
    void writeAndLoadConfig(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&
        config);
    void assertConfig(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>& config);

    static std::string m_testInputFileName;
    ConfigParser m_configParser;
};

std::string ConfigParserTest::m_testInputFileName = "configParserTestInput.txt";

static const auto onlyDefault = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>{
    {"",
        {
            {"key1", "value6"},
            {"key2", "value8"},
            {"key3", "value"}
        }
    }
};

static const auto multipleSections = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>{
    {"section1",
        {
            {"key1", "value1"},
            {"key2", "value2"},
            {"key3", "value3"}
        }
    },
    {"section2",
        {
            {"key4", "value4"}
        }
    }
};

static const auto defaultAndSections = std::unordered_map<std::string, std::unordered_map<std::string,
        std::string>>{
    {"",
        {
            {"key1", "value1"},
            {"key2", "value2"},
            {"key3", "value3"}
        }
    },
    {"section2",
        {
            {"key4", "value4"}
        }
    }
};

void ConfigParserTest::TearDown()
{
    std::remove(m_testInputFileName.c_str());
}

void ConfigParserTest::writeSection(const std::string& section, const std::unordered_map<std::string, std::string>&
    values, std::ofstream& testInputFile)
{
    if (!section.empty())
        testInputFile << "[" + section << "]\n";

    for (const auto& keyValue : values)
        testInputFile << keyValue.first << "=" << keyValue.second << "\n";
}

void ConfigParserTest::writeConfigFile(const std::unordered_map<std::string, std::unordered_map<std::string,
    std::string>>& config)
{
    std::ofstream testInput(m_testInputFileName, std::ofstream::out);
    auto defaultSection = config.find("");
    if (defaultSection != config.end())
        writeSection("", defaultSection->second, testInput);

    for (const auto& sectionPair : config) {
        if (sectionPair.first.empty())
            continue;

        writeSection(sectionPair.first, sectionPair.second, testInput);
    }
}

void ConfigParserTest::writeAndLoadConfig(const std::unordered_map<std::string, std::unordered_map<std::string,
    std::string>>& config)
{
    writeConfigFile(config);
    m_configParser.loadConfig(m_testInputFileName);
}

void ConfigParserTest::assertConfig(const std::unordered_map<std::string, std::unordered_map<std::string, std::string>>&
    config)
{
    writeAndLoadConfig(config);
    EXPECT_EQ(config, m_configParser.getConfig());
}

TEST_F(ConfigParserTest, testWithTwoSections)
{
    assertConfig(multipleSections);
}

TEST_F(ConfigParserTest, testWithDefaultSection)
{
    assertConfig(defaultAndSections);
}

TEST_F(ConfigParserTest, testOnlyWithDefaultSection)
{
    assertConfig(onlyDefault);
}

TEST_F(ConfigParserTest, testGetValueOnlyWithDefaultSectionFromDefaultSection)
{
    const auto testedSection = "";
    const auto testedKey = "key2";
    writeAndLoadConfig(onlyDefault);
    ASSERT_EQ(onlyDefault.at(testedSection).at(testedKey), m_configParser.getValue(testedSection, testedKey));
}

TEST_F(ConfigParserTest, testGetValueWithDefaultSectionFromDefaultSection)
{
    const auto testedSection = "";
    const auto testedKey = "key3";
    writeAndLoadConfig(defaultAndSections);
    ASSERT_EQ(defaultAndSections.at(testedSection).at(testedKey), m_configParser.getValue(testedSection, testedKey));
}

TEST_F(ConfigParserTest, testGetValueWithDefaultSectionFromNonDefaultSection)
{
    const auto testedSection = "section2";
    const auto testedKey = "key4";
    writeAndLoadConfig(defaultAndSections);
    ASSERT_EQ(defaultAndSections.at(testedSection).at(testedKey), m_configParser.getValue(testedSection, testedKey));
}

TEST_F(ConfigParserTest, testGetValueFromSection)
{
    const auto testedSection = "section1";
    const auto testedKey = "key1";
    writeAndLoadConfig(multipleSections);
    ASSERT_EQ(multipleSections.at(testedSection).at(testedKey), m_configParser.getValue(testedSection, testedKey));
}

TEST_F(ConfigParserTest, testGetValueFromNonExistentKey)
{
    const auto testedSection = "section2";
    const auto testedKey = "nonExistentKey";
    writeAndLoadConfig(defaultAndSections);
    ASSERT_EQ("", m_configParser.getValue(testedSection, testedKey));
}

TEST_F(ConfigParserTest, testGetValueFromNonExistentSection)
{
    const auto testedSection = "nonExistentSection";
    const auto testedKey = "key2";
    writeAndLoadConfig(defaultAndSections);
    ASSERT_TRUE(m_configParser.getValue(testedSection, testedKey).empty());
}

TEST_F(ConfigParserTest, testGetValueFromNonExistentKeyAndSection)
{
    const auto testedSection = "nonExistentSection";
    const auto testedKey = "nonExistentKey";
    writeAndLoadConfig(defaultAndSections);
    ASSERT_TRUE(m_configParser.getValue(testedSection, testedKey).empty());
}


TEST_F(ConfigParserTest, testGetDefaultSectionWithOnlyDefaultSection)
{
    const auto testedSection = "";
    writeAndLoadConfig(onlyDefault);
    ASSERT_EQ(onlyDefault.at(testedSection), m_configParser.getSectionConfig(testedSection));
}

TEST_F(ConfigParserTest, testGetDefaultSectionWithDefaultAndNonDefaultSection)
{
    const auto testedSection = "";
    writeAndLoadConfig(defaultAndSections);
    ASSERT_EQ(defaultAndSections.at(testedSection), m_configParser.getSectionConfig(testedSection));
}

TEST_F(ConfigParserTest, testGetSectionWithDefaultAndNonSection)
{
    const auto testedSection = "section2";
    writeAndLoadConfig(defaultAndSections);
    ASSERT_EQ(defaultAndSections.at(testedSection), m_configParser.getSectionConfig(testedSection));
}

TEST_F(ConfigParserTest, testGetSection)
{
    const auto testedSection = "section1";
    writeAndLoadConfig(multipleSections);
    ASSERT_EQ(multipleSections.at(testedSection), m_configParser.getSectionConfig(testedSection));
}

TEST_F(ConfigParserTest, testGetSectionFromNonExistentSection)
{
    const auto testedSection = "nonExistentSection";
    writeAndLoadConfig(defaultAndSections);
    ASSERT_TRUE(m_configParser.getSectionConfig(testedSection).empty());
}

TEST_F(ConfigParserTest, testDefaultSectionFromNonExistentDefaultSection)
{
    const auto testedSection = "";
    writeAndLoadConfig(multipleSections);
    ASSERT_TRUE(m_configParser.getSectionConfig(testedSection).empty());
}

TEST_F(ConfigParserTest, testEmptySectionValues)
{
    auto m = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>{
        {"",
            {
                {"key1", "value1"},
                {"key2", "value2"},
                {"key3", "value3"}
            }
        },
        {"section2", { } }
    };
    writeConfigFile(m);
    ASSERT_FALSE(m_configParser.loadConfig(m_testInputFileName));
}

TEST_F(ConfigParserTest, testEmptyValue)
{
    auto m = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>{
        {"",
            {
                {"key1", ""},
                {"key2", "value2"},
                {"key3", "value3"}
            }
        },
        {"section2", { } }
    };
    writeConfigFile(m);
    ASSERT_FALSE(m_configParser.loadConfig(m_testInputFileName));
}

TEST_F(ConfigParserTest, testLoadInvalidConfigFile)
{
    auto fileContent = "some random file\nwithvalue=key\nnothing here!";
    std::ofstream testInput(m_testInputFileName, std::ofstream::out);
    testInput << fileContent;
    testInput.close();
    ASSERT_FALSE(m_configParser.loadConfig(m_testInputFileName));
}

TEST_F(ConfigParserTest, testLoadInvalidConfigFileWithSection)
{
    auto fileContent = "[section]\nsome random file\nwithvalue=key\nnothing here!";
    std::ofstream testInput(m_testInputFileName, std::ofstream::out);
    testInput << fileContent;
    testInput.close();
    ASSERT_FALSE(m_configParser.loadConfig(m_testInputFileName));
}

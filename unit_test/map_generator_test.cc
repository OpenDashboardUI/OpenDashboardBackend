// Copyright (C) 2020 twyleg
#include <gtest/gtest.h>

#include <open_dashboard_common/helper.h>
#include <open_dashboard_common_ui/map_generator.h>

#include <iostream>
#include <fstream>
#include <filesystem>

namespace OpenDashboard::testing {

constexpr const char* TEST_API_KEY = "VVVV-WWWW-XXXX-YYYY-ZZZZ";
constexpr const char* TEST_APPLICATION_NAME = "OpenDashboardTest";

class MapGeneratorTest: public ::testing::Test {

public:

void SetUp() override
{
	auto outputDir = std::filesystem::temp_directory_path() / TEST_APPLICATION_NAME;
	std::filesystem::remove_all(outputDir);
}

std::vector<std::string> ReadMapFile(const std::filesystem::path& filePath)
{
	std::vector<std::string> result;
	std::ifstream inputFileStream(filePath);

	std::string line;
	while (std::getline(inputFileStream, line))
		result.emplace_back(line);

	return std::move(result);
}

void ExpectApiKeyInserted(const QUrl& outputFilePath)
{
	auto mapFileLines = ReadMapFile(outputFilePath.toLocalFile().toStdString());

	EXPECT_EQ(mapFileLines[10], "\t\t<script type=\"text/javascript\" src=\"https://maps.googleapis.com/maps/api/js?key=VVVV-WWWW-XXXX-YYYY-ZZZZ&callback=initMap\" async defer></script>");
}

};

TEST_F(MapGeneratorTest, GenerateMap_OutputIncludesApiKey)
{
	CommonUI::MapGenerator mapGenerator(Common::GetTempAppDirectory("OpenDashboardTest"));
	ExpectApiKeyInserted(mapGenerator.generateMap(TEST_API_KEY));
}


}

// Copyright (C) 2020 twyleg
#include <gtest/gtest.h>

#include <open_dashboard_common/config.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cassert>
#include <cstring>

namespace OpenDashboard::testing {

using namespace std::chrono_literals;

const std::string OPEN_DASHBOARD_XML = R"(<?xml version="1.0"?>
<OpenDashboard>
	<TrackInformation>track_information.xml</TrackInformation>

	<DynamicData>
		<File>dynamic_data/source_car.csv</File>
		<SampleTime>10</SampleTime>
	</DynamicData>

	<VideoData>
		<File>videos/video_a.avi</File>
		<File>videos/video_b.avi</File>
		<File>videos/video_c.avi</File>
	</VideoData>
</OpenDashboard>
)";

const std::string INVALID_OPEN_DASHBOARD_XML = R"(<?xml version="1.0"?>
<OpenDashboard>
	<TrackInformation>track_information.xml</TrackInformation>

	<ERROR>
		<File>dynamic_data/source_car.csv</File>
		<SampleTime>10</SampleTime>
	</DynamicData>

	<VideoData>
		<File>videos/video_a.avi</File>
		<File>videos/video_b.avi</File>
		<File>videos/video_c.avi</File>
	</VideoData>
</OpenDashboard>
)";

const std::string TRACK_INFORMATION_XML = R"(<?xml version="1.0"?>
<TrackInformation>
	<Name>nurburgring_nordschleife</Name>
	<Length>21500.0</Length>
	<LapRecord>05:19.540</LapRecord>
</TrackInformation>
)";

class ConfigTest : public ::testing::Test {

public:

	void SetUp() override
	{
		std::filesystem::remove(mOpenDashboardXmlFilepath);
		std::filesystem::remove(mTrackInformationXmlFilepath);
		std::filesystem::remove(mDynamicDataCsvFilepath);
		std::filesystem::remove(mVideoAFilepath);
		std::filesystem::remove(mVideoBFilepath);
		std::filesystem::remove(mVideoCFilepath);
	}

	void PrepareOpenDashboardConfigFile()
	{
		std::ofstream openDashboardXmlOutputStream(mOpenDashboardXmlFilepath);
		openDashboardXmlOutputStream.write(OPEN_DASHBOARD_XML.c_str(), OPEN_DASHBOARD_XML.size());
		openDashboardXmlOutputStream.close();
	}

	void PrepareInvalidOpenDashboardConfigFile()
	{
		std::ofstream openDashboardXmlOutputStream(mOpenDashboardXmlFilepath);
		openDashboardXmlOutputStream.write(INVALID_OPEN_DASHBOARD_XML.c_str(), INVALID_OPEN_DASHBOARD_XML.size());
		openDashboardXmlOutputStream.close();
	}

	void PrepareTrackInformationConfigFile()
	{
		std::ofstream trackInformationXmlOutputStream(mTrackInformationXmlFilepath);
		trackInformationXmlOutputStream.write(TRACK_INFORMATION_XML.c_str(), TRACK_INFORMATION_XML.size());
		trackInformationXmlOutputStream.close();
	}

	void PrepareDynamicDataDummyFile()
	{
		std::filesystem::create_directory(mDynamicDataCsvFilepath.parent_path());
		std::ofstream dynamicDataCsvOutputStream(mDynamicDataCsvFilepath);
	}

	void PrepareVideoDataDummyFiles()
	{
		std::filesystem::create_directory(mVideoAFilepath.parent_path());

		std::ofstream videoDataAOutputStream(mVideoAFilepath);
		std::ofstream videoDataBOutputStream(mVideoBFilepath);
		std::ofstream videoDataCOutputStream(mVideoCFilepath);
	}

	const std::filesystem::path mOpenDashboardXmlFilepath = "./open_dashboard.xml";
	const std::filesystem::path mTrackInformationXmlFilepath = "./track_information.xml";
	const std::filesystem::path mDynamicDataCsvFilepath = "./dynamic_data/source_car.csv";

	const std::filesystem::path mVideoAFilepath = "./videos/video_a.avi";
	const std::filesystem::path mVideoBFilepath = "./videos/video_b.avi";
	const std::filesystem::path mVideoCFilepath = "./videos/video_c.avi";
};

TEST_F(ConfigTest, ValidConfigFiles_ReadConfig_Success)
{
	PrepareOpenDashboardConfigFile();
	PrepareTrackInformationConfigFile();
	PrepareDynamicDataDummyFile();
	PrepareVideoDataDummyFiles();

	Common::Config config = Common::Config::ReadConfig(mOpenDashboardXmlFilepath);
	EXPECT_EQ(config.mVideoData[0], "./videos/video_a.avi");
	EXPECT_EQ(config.mVideoData[1], "./videos/video_b.avi");
	EXPECT_EQ(config.mVideoData[2], "./videos/video_c.avi");
	EXPECT_EQ(config.mTrackInformation.mName, "nurburgring_nordschleife");
	EXPECT_NEAR(config.mTrackInformation.mLength, 21500.0, 0.001);
	EXPECT_EQ(config.mTrackInformation.mLapRecord, std::chrono::milliseconds((5*60 + 19) * 1000 + 540));
	EXPECT_EQ(config.mDynamicData.mFile, "dynamic_data/source_car.csv");
	EXPECT_EQ(config.mDynamicData.mSampleTime, 10ms);
}

TEST_F(ConfigTest, MissingConfigFile_ReadConfig_Throw)
{
	EXPECT_THROW(Common::Config::ReadConfig(mOpenDashboardXmlFilepath), std::runtime_error);
}

TEST_F(ConfigTest, MissingSubconfigFile_ReadSubconfig_Throw)
{
	PrepareOpenDashboardConfigFile();
	EXPECT_THROW(Common::Config::ReadConfig(mOpenDashboardXmlFilepath), std::runtime_error);
}

TEST_F(ConfigTest, MissingDynamicDataFile_ReadConfig_Throw)
{
	PrepareOpenDashboardConfigFile();
	PrepareTrackInformationConfigFile();
	PrepareVideoDataDummyFiles();
	EXPECT_THROW(Common::Config::ReadConfig(mOpenDashboardXmlFilepath), std::runtime_error);
}

TEST_F(ConfigTest, MissingVideoDataFile_ReadConfig_Throw)
{
	PrepareOpenDashboardConfigFile();
	PrepareTrackInformationConfigFile();
	PrepareDynamicDataDummyFile();
	EXPECT_THROW(Common::Config::ReadConfig(mOpenDashboardXmlFilepath), std::runtime_error);
}

TEST_F(ConfigTest, InvalidConfigFiles_ReadConfig_Throw)
{
	PrepareInvalidOpenDashboardConfigFile();
	EXPECT_THROW(Common::Config::ReadConfig(mOpenDashboardXmlFilepath), xercesc::SAXParseException);
}

}


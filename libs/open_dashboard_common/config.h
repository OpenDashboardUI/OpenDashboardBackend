// Copyright (C) 2020 twyleg
#pragma once

#include <xercesc/sax/SAXParseException.hpp>

#include <filesystem>
#include <chrono>
#include <array>

namespace OpenDashboard::Common {

struct Config {

	struct TrackInformation
	{
		std::string mName;
		double mLength;
		std::chrono::milliseconds mLapRecord;
	};

	struct DynamicData
	{
		std::filesystem::path mFile;
		std::chrono::milliseconds mSampleTime;
	};

	TrackInformation mTrackInformation;
	DynamicData mDynamicData;
	std::array<std::filesystem::path, 3> mVideoData;

	static Config ReadConfig(const std::filesystem::path& configFilePath);
};

}

#pragma once

#include <string>
#include <filesystem>
#include <optional>

namespace OpenDashboard::Backend {

struct CliArguments
{
	CliArguments(int argc, char* argv[]);
	void ValidateRuntimeArguments();

	std::string mHostname;
	int mPort;
	std::optional<std::filesystem::path> mConfigFilePath;
	std::optional<std::filesystem::path> mMainQmlFilePath;

	bool mSidebarsDisabled;
};

}

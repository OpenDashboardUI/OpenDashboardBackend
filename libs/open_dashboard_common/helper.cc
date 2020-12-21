// Copyright (C) 2020 twyleg
#include "helper.h"

namespace OpenDashboard::Common {

std::filesystem::path GetTempAppDirectory(const std::string& applicationName)
{
	const auto tmpDirPath = std::filesystem::temp_directory_path();
	const auto tmpApplicationDirPath = tmpDirPath / applicationName;
	std::filesystem::create_directory(tmpApplicationDirPath);
	return tmpApplicationDirPath;
}

}

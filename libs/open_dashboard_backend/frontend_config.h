#pragma once

#include <filesystem>
#include <vector>

namespace OpenDashboard {

class FrontendConfig
{
public:

	FrontendConfig(const std::filesystem::path& frontendConfigPath);

	std::filesystem::path mQmlMainFile;
	std::vector<std::filesystem::path> mQmlImportPaths;

private:

};


}

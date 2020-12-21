// Copyright (C) 2020 twyleg
#include "helper.h"

namespace OpenDashboard::CommonUI {

QUrl QUrlFromPath(const std::filesystem::path& path)
{
	return QUrl::fromLocalFile(QString::fromStdString(std::filesystem::absolute(path).generic_string()));
}

}

// Copyright (C) 2020 twyleg
#pragma once

#include <QUrl>

#include <filesystem>

namespace OpenDashboard::CommonUI {

QUrl QUrlFromPath(const std::filesystem::path& path);

}

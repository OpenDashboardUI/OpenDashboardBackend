// Copyright (C) 2020 twyleg
#pragma once

#include <fmt/format.h>

#include <exception>
#include <filesystem>

#define FORMAT(...) fmt::format(__VA_ARGS__)
#define THROW_IF(expr, ...) if (expr) throw std::runtime_error(fmt::format(__VA_ARGS__))

namespace OpenDashboard::Common {

std::filesystem::path GetTempAppDirectory(const std::string& applicationName);

}

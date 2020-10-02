// Copyright (C) 2020 twyleg
#pragma once

#include <exception>
#include <fmt/format.h>

#define FORMAT(msg...) fmt::format(msg)

#define THROW_IF(expr, msg...) if (expr) throw std::runtime_error(fmt::format(msg))

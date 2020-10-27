// Copyright (C) 2020 twyleg
#pragma once

#include <exception>
#include <fmt/format.h>

#define FORMAT(...) fmt::format(__VA_ARGS__)
#define THROW_IF(expr, ...) if (expr) throw std::runtime_error(fmt::format(__VA_ARGS__))

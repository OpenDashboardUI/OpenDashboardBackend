// Copyright (C) 2020 twyleg
#include <gtest/gtest.h>

#include <open_dashboard_common/helper.h>

#include <iostream>
#include <sstream>
#include <cassert>
#include <cstring>


namespace OpenDashboard::testing {

TEST(HelperTest, Format_StaticFormat)
{
	const std::string output = FORMAT("Foobar");
	EXPECT_EQ(output, "Foobar");
}

TEST(HelperTest, Format_DynamicFormat_OneArg)
{
	const std::string output = FORMAT("Foobar {}", 42);
	EXPECT_EQ(output, "Foobar 42");
}

}


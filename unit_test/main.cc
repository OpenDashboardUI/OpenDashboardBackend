// Copyright (C) 2020 twyleg
#include <gtest/gtest.h>

#include <QObject>

int main(int argc, char **argv)
{
	Q_INIT_RESOURCE(common_ui_resources);

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

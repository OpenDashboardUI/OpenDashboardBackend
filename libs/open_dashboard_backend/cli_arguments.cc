// Copyright (C) 2020 twyleg
#include "cli_arguments.h"

#include <open_dashboard_common/helper.h>

#include <boost/program_options.hpp>

#include <iostream>
#include <string>

namespace OpenDashboard::Backend {

namespace {

constexpr const char* CONFIG_DEFAULT_HOSTNAME = "0.0.0.0";
constexpr int CONFIG_DEFAULT_PORT = 50000;

}

CliArguments::CliArguments(int argc, char* argv[])
{
	namespace po = boost::program_options;

	po::variables_map vm;
	po::options_description desc("Available options:");
	desc.add_options()
		("help", "Print help message.")
		("port,p", po::value<int>(&mPort)->default_value(CONFIG_DEFAULT_PORT), "Destination port")
		("hostname,h", po::value<std::string>(&mHostname)->default_value(CONFIG_DEFAULT_HOSTNAME), "Destination hostname")
		("config,c", po::value<std::filesystem::path>(), "Config")
		("qml-file", po::value<std::filesystem::path>(), "QML main file")
		("sidebars-disabled", po::bool_switch(&mSidebarsDisabled), "Disable sidebars")
	;

	po::positional_options_description p;
	p.add("qml-file", -1);

	try
	{
		po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
		po::notify(vm);
	}
	catch (const boost::program_options::required_option& e)
	{
		std::cout << "Missing required argument " << e.get_option_name() << std::endl;
		std::cout << desc << std::endl;
		std::exit(-1);
	}

	if (!vm["config"].empty())
		mConfigFilePath.emplace(vm["config"].as<std::filesystem::path>());

	if (!vm["qml-file"].empty())
		mMainQmlFilePath.emplace(vm["qml-file"].as<std::filesystem::path>());

	if (vm.count("help"))
	{
		std::cout << desc << std::endl;
		std::exit(0);
	}
}

void CliArguments::ValidateRuntimeArguments()
{
	if (mConfigFilePath)
	{
		THROW_IF(!std::filesystem::is_regular_file(*mConfigFilePath), "Config file does not exist: {}", mConfigFilePath->string());
		THROW_IF(mConfigFilePath->extension() != ".xml", "Config file does not have \".xml\" extension: {}", mConfigFilePath->string());
	}

	if (mMainQmlFilePath)
	{
		THROW_IF(!std::filesystem::is_regular_file(*mMainQmlFilePath), "Qml file does not exist: {}", mMainQmlFilePath->string());
		THROW_IF(mMainQmlFilePath->extension() != ".xml", "Qml file does not have \".qml\" extension: {}", mMainQmlFilePath->string());
	}
}

}

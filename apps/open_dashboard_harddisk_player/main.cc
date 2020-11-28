// Copyright (C) 2020 twyleg
#define _USE_MATH_DEFINES
#include <cmath>

#include <open_dashboard_common/csv_reader.h>
#include <open_dashboard_common/udp_transceiver.h>
#include <open_dashboard_common/packet.h>
#include <open_dashboard_common/config.h>
#include <open_dashboard_common/helper.h>
#include <open_dashboard_common/harddisk_player.h>

#include <open_dashboard.pb.h>

#include <google/protobuf/text_format.h>
#include <google/protobuf/util/json_util.h>

#include <boost/asio.hpp>
#include <boost/program_options.hpp>

#include <iostream>
#include <thread>
#include <filesystem>

using namespace OpenDashboard;
using namespace std::chrono_literals;

namespace {

constexpr const char* CONFIG_DEFAULT_HOSTNAME = "127.0.0.1";
constexpr int CONFIG_DEFAULT_PORT = 50000;

}

std::ostream& operator<< (std::ostream& os, const google::protobuf::Message& message)
{
	std::string output;
	google::protobuf::TextFormat::PrintToString(message, &output);
	os << output;
	return os;
}

struct CliArguments
{
	CliArguments(int argc, char* argv[])
	{
		namespace po = boost::program_options;

		po::variables_map vm;
		po::options_description desc("Available options:");
		desc.add_options()
			("help", "Print help message.")
			("port,p", po::value<int>(&mPort)->default_value(CONFIG_DEFAULT_PORT), "Remote port")
			("hostname,h", po::value<std::string>(&mHostname)->default_value(CONFIG_DEFAULT_HOSTNAME), "Remote hostname")
			("config,c", po::value<std::filesystem::path>(&mConfigFilePath)->required(), "Config file")
		;

		po::positional_options_description p;
		p.add("config", -1);

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

		if (vm.count("help")) {
			std::cout << desc << std::endl;
			std::exit(0);
		}
	}

	void ValidateRuntimeArguments()
	{
		THROW_IF(!std::filesystem::is_regular_file(mConfigFilePath), "Config file does not exist: {}", mConfigFilePath.string());
		THROW_IF(mConfigFilePath.extension() != ".xml", "Config file does not have \".xml\" extension: {}", mConfigFilePath.string());
	}

	std::string mHostname;
	int mPort;
	std::filesystem::path mConfigFilePath;
};

int main(int argc, char* argv[])
{
	CliArguments cliArguments(argc, argv);
	cliArguments.ValidateRuntimeArguments();

	Common::Config config = OpenDashboard::Common::Config::ReadConfig(cliArguments.mConfigFilePath);

	boost::asio::io_service ioService;
	boost::posix_time::milliseconds interval(config.mDynamicData.mSampleTime.count());
	boost::asio::deadline_timer timer(ioService, interval);

	Common::UdpTransmitter udpTransmitter(cliArguments.mHostname, cliArguments.mPort);
	Common::HarddiskPlayer harddiskPlayer(config.mDynamicData.mFile);

	{
		Common::OutboundPacket packet;
		Proto::Control control;
		control.set_state(Proto::STARTED);
		packet.AddMessage(Proto::CONTROL_MSG, control);
		udpTransmitter.Transmit(packet.GetData(), packet.GetSize());
	}

	const size_t framesToRead = harddiskPlayer.GetNumberOfFrames();
	size_t currentFrame = 0;

	std::function<void(const boost::system::error_code&)> run = [&](const boost::system::error_code&){
		timer.expires_at(timer.expires_at() + interval);
		timer.async_wait(run);

		if (currentFrame == framesToRead)
			std::exit(0);

		harddiskPlayer.ReadFrame(currentFrame++);

		Common::OutboundPacket packet;
		Proto::Dynamics dynamics = harddiskPlayer.GetDynamics();
		Proto::DriverInput driverInput = harddiskPlayer.GetDriverInput();
		Proto::Powertrain powertrain = harddiskPlayer.GetPowertrain();
		Proto::Gps gps = harddiskPlayer.GetGps();

		packet.AddMessage(Proto::DYNAMICS_MSG, dynamics);
		packet.AddMessage(Proto::DRIVER_INPUT_MSG, driverInput);
		packet.AddMessage(Proto::POWERTRAIN_MSG, powertrain);
		packet.AddMessage(Proto::GPS_MSG, gps);

		udpTransmitter.Transmit(packet.GetData(), packet.GetSize());

		std::cout << dynamics << std::endl;
		std::cout << driverInput << std::endl;
		std::cout << powertrain << std::endl;
		std::cout << gps << std::endl;
	};

	timer.async_wait(run);
	ioService.run();

	return 0;
}

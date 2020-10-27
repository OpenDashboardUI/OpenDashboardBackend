// Copyright (C) 2020 twyleg
#define _USE_MATH_DEFINES
#include <cmath>

#include <open_dashboard_common/csv_reader.h>
#include <open_dashboard_common/udp_transceiver.h>
#include <open_dashboard_common/packet.h>
#include <open_dashboard_common/config.h>
#include <open_dashboard_common/helper.h>

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

		try {
			po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
			po::notify(vm);
		} catch (const boost::wrapexcept<boost::program_options::required_option>& e) {
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

	Common::CsvReader csvReader(config.mDynamicData.mFile);
	Common::UdpTransmitter udpTransmitter(cliArguments.mHostname, cliArguments.mPort);
	csvReader.ReadHeader();
	csvReader.IgnoreLine();
	csvReader.IgnoreLine();

	{
		Common::OutboundPacket packet;
		Control control;
		control.set_state(STARTED);
		packet.AddMessage(CONTROL_MSG, control);
		udpTransmitter.Transmit(packet.GetData(), packet.GetSize());
	}

	std::function<void(const boost::system::error_code&)> run = [&](const boost::system::error_code&){
		timer.expires_at(timer.expires_at() + interval);
		timer.async_wait(run);

		if (csvReader.ReadLine() == false)
			std::exit(0);

		Common::OutboundPacket packet;

		VehicleDynamic vehicleDynamic;
		vehicleDynamic.set_velocity(csvReader.GetValue<double>("Vhcl.v") * 3.6);
		vehicleDynamic.set_ax(csvReader.GetValue<double>("Car.ax"));
		vehicleDynamic.set_ay(csvReader.GetValue<double>("Car.ay"));
		vehicleDynamic.set_az(csvReader.GetValue<double>("Car.az"));
		packet.AddMessage(VEHICLE_DYNAMIC_MSG, vehicleDynamic);
		std::cout << vehicleDynamic << std::endl;

		DriverInput driverInput;
		driverInput.set_throttle(csvReader.GetValue<double>("VC.Gas"));
		driverInput.set_brake(csvReader.GetValue<double>("VC.Brake"));
		driverInput.set_steering_wheel_angle(csvReader.GetValue<double>("VC.Steer.Ang") * (180.0 / M_PI));
		packet.AddMessage(DRIVER_INPUT_MSG, driverInput);
		std::cout << driverInput << std::endl;

		Powertrain powertrain;
		powertrain.set_rotation(csvReader.GetValue<double>("Vhcl.Engine.rotv") * (60.0 / (2*M_PI)));
		powertrain.set_gear(csvReader.GetValue<int>("VC.GearNo"));

		powertrain.set_engine_power_out(csvReader.GetValue<double>("PT.Engine.PwrO") / 1000.0);
		powertrain.set_engine_torque(csvReader.GetValue<double>("PT.Engine.Trq"));
		powertrain.set_engine_fuel_consumption_abs(csvReader.GetValue<double>("PT.Control.Consump.Fuel.Abs"));
		powertrain.set_engine_fuel_consumption_act(csvReader.GetValue<double>("PT.Control.Consump.Fuel.Act"));
		powertrain.set_engine_fuel_consumption_avg(csvReader.GetValue<double>("PT.Control.Consump.Fuel.Avg"));
		powertrain.set_engine_fuel_level(csvReader.GetValue<double>("PT.Engine.FuelFlow"));

		powertrain.set_gearbox_power_in(csvReader.GetValue<double>("PT.GearBox.PwrI") / 1000.0);
		powertrain.set_gearbox_power_out(csvReader.GetValue<double>("PT.GearBox.PwrO") / 1000.0);
		powertrain.set_gearbox_rotation_in(csvReader.GetValue<double>("PT.GearBox.rotv_in")  * (60.0 / (2*M_PI)));
		powertrain.set_gearbox_rotation_out(csvReader.GetValue<double>("PT.GearBox.rotv_out")  * (60.0 / (2*M_PI)));

		packet.AddMessage(POWERTRAIN_MSG, powertrain);
		std::cout << powertrain << std::endl;

		Gps gps;
		gps.set_lon(csvReader.GetValue<double>("Car.Road.GCS.Long") * (180.0 / M_PI));
		gps.set_lat(csvReader.GetValue<double>("Car.Road.GCS.Lat") * (180.0 / M_PI));
		gps.set_elevation(csvReader.GetValue<double>("Car.Road.GCS.Elev"));
		packet.AddMessage(GPS_MSG, gps);
		std::cout << gps << std::endl;

		udpTransmitter.Transmit(packet.GetData(), packet.GetSize());
	};

	timer.async_wait(run);
	ioService.run();

	return 0;
}

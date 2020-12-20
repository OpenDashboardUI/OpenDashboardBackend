// Copyright (C) 2020 twyleg
#define _USE_MATH_DEFINES
#include <cmath>

#include "harddisk_player.h"


namespace OpenDashboard::Common {

CarMakerFileParser::CarMakerFileParser(const std::filesystem::path& filepath)
	: mCsvReader(filepath)
{
	mCsvReader.ReadFileToCache(true, 2);
}

void CarMakerFileParser::ReadFrame(size_t frameNumber)
{
	mCsvReader.ReadLine(frameNumber);
}

size_t CarMakerFileParser::GetNumberOfFrames()
{
	return mCsvReader.GetNumberOfLines();
}

double CarMakerFileParser::GetTimestamp()
{
	return mCsvReader.GetValue<double>("Time");
}

Proto::Dynamics CarMakerFileParser::GetDynamics()
{
	Proto::Dynamics dynamics;
	dynamics.set_velocity(mCsvReader.GetValue<double>("Vhcl.v") * 3.6);
	dynamics.set_ax(mCsvReader.GetValue<double>("Car.ax"));
	dynamics.set_ay(mCsvReader.GetValue<double>("Car.ay"));
	dynamics.set_az(mCsvReader.GetValue<double>("Car.az"));
	return dynamics;
}

Proto::DriverInput CarMakerFileParser::GetDriverInput()
{
	Proto::DriverInput driverInput;
	driverInput.set_throttle(mCsvReader.GetValue<double>("VC.Gas"));
	driverInput.set_brake(mCsvReader.GetValue<double>("VC.Brake"));
	driverInput.set_steering_wheel_angle(mCsvReader.GetValue<double>("VC.Steer.Ang") * (180.0 / M_PI));
	return driverInput;
}

Proto::Powertrain CarMakerFileParser::GetPowertrain()
{
	Proto::Powertrain powertrain;
	powertrain.set_engine_rotation(mCsvReader.GetValue<double>("Vhcl.Engine.rotv") * (60.0 / (2*M_PI)));
	powertrain.set_engine_power_out(mCsvReader.GetValue<double>("PT.Engine.PwrO") / 1000.0);
	powertrain.set_engine_torque(mCsvReader.GetValue<double>("PT.Engine.Trq"));
	powertrain.set_engine_fuel_consumption_abs(mCsvReader.GetValue<double>("PT.Control.Consump.Fuel.Abs"));
	powertrain.set_engine_fuel_consumption_act(mCsvReader.GetValue<double>("PT.Control.Consump.Fuel.Act"));
	powertrain.set_engine_fuel_consumption_avg(mCsvReader.GetValue<double>("PT.Control.Consump.Fuel.Avg"));
	powertrain.set_engine_fuel_level(mCsvReader.GetValue<double>("PT.Engine.FuelFlow"));
	powertrain.set_gearbox_gear(mCsvReader.GetValue<int>("VC.GearNo"));
	powertrain.set_gearbox_power_in(mCsvReader.GetValue<double>("PT.GearBox.PwrI") / 1000.0);
	powertrain.set_gearbox_power_out(mCsvReader.GetValue<double>("PT.GearBox.PwrO") / 1000.0);
	powertrain.set_gearbox_rotation_in(mCsvReader.GetValue<double>("PT.GearBox.rotv_in")  * (60.0 / (2*M_PI)));
	powertrain.set_gearbox_rotation_out(mCsvReader.GetValue<double>("PT.GearBox.rotv_out")  * (60.0 / (2*M_PI)));
	return powertrain;
}

Proto::Gps CarMakerFileParser::GetGps()
{
	Proto::Gps gps;
	gps.set_lon(mCsvReader.GetValue<double>("Car.Road.GCS.Long") * (180.0 / M_PI));
	gps.set_lat(mCsvReader.GetValue<double>("Car.Road.GCS.Lat") * (180.0 / M_PI));
	gps.set_elevation(mCsvReader.GetValue<double>("Car.Road.GCS.Elev"));
	return gps;
}




HarddiskPlayer::HarddiskPlayer(const std::filesystem::path& filepath)
{
	mFileParser = std::make_unique<CarMakerFileParser>(filepath);
}

void HarddiskPlayer::ReadFrame(size_t frameNumber)
{
	mFileParser->ReadFrame(frameNumber);
}

size_t HarddiskPlayer::GetFrameCount()
{
	return mFileParser->GetNumberOfFrames();
}

double HarddiskPlayer::GetEndTimestamp()
{
	ReadFrame(GetFrameCount()-1);
	return GetTimestamp();
}

double HarddiskPlayer::GetTimestamp()
{
	return mFileParser->GetTimestamp();
}

Proto::Dynamics HarddiskPlayer::GetDynamics()
{
	return mFileParser->GetDynamics();
}

Proto::DriverInput HarddiskPlayer::GetDriverInput()
{
	return mFileParser->GetDriverInput();
}

Proto::Powertrain HarddiskPlayer::GetPowertrain()
{
	return mFileParser->GetPowertrain();
}

Proto::Gps HarddiskPlayer::GetGps()
{
	return mFileParser->GetGps();
}

}

#pragma once

#include "csv_reader.h"

#include <open_dashboard.pb.h>

#include <filesystem>

namespace OpenDashboard::Common {

class IFileParser
{
public:

	virtual ~IFileParser() = default;

	virtual void ReadFrame(size_t frameNumber) = 0;
	virtual size_t GetNumberOfFrames() = 0;

	virtual Proto::Dynamics GetDynamics() = 0;
	virtual Proto::DriverInput GetDriverInput() = 0;
	virtual Proto::Powertrain GetPowertrain() = 0;
	virtual Proto::Gps GetGps() = 0;
};

class CarMakerFileParser : public IFileParser
{
public:

	CarMakerFileParser(const std::filesystem::path&);

	void ReadFrame(size_t frameNumber) override;
	size_t GetNumberOfFrames() override;

	Proto::Dynamics GetDynamics() override;
	Proto::DriverInput GetDriverInput() override;
	Proto::Powertrain GetPowertrain() override;
	Proto::Gps GetGps() override;

private:

	CsvReader mCsvReader;
};

class HarddiskPlayer
{
public:

	explicit HarddiskPlayer(const std::filesystem::path&);

	void ReadFrame(size_t frameNumber);
	size_t GetNumberOfFrames();

	Proto::Dynamics GetDynamics();
	Proto::DriverInput GetDriverInput();
	Proto::Powertrain GetPowertrain();
	Proto::Gps GetGps();

private:

	std::unique_ptr<IFileParser> mFileParser;
};

}

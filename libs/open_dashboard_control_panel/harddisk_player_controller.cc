// Copyright (C) 2020 twyleg
#include "harddisk_player_controller.h"

#include <open_dashboard_common_ui/helper.h>

namespace OpenDashboard::ControlPanel {


HarddiskPlayerController::HarddiskPlayerController(CommonUI::ConnectionManager& connectionManager)
	: mConnectionManager(connectionManager)
{
	QObject::connect(&mPlayTimer, SIGNAL(timeout()), this, SLOT(handlePlayTimerTimeout()));
}

void HarddiskPlayerController::openFile(const QUrl& filepath)
{
	Common::Config config = OpenDashboard::Common::Config::ReadConfig(filepath.toLocalFile().toStdString());

	mHarddiskPlayer = std::make_unique<Common::HarddiskPlayer>(config.mDynamicData.mFile);

	SetRecordFramesCount(mHarddiskPlayer->GetFrameCount());
	SetRecordCurrentFrame(0);
	SetRecordEndTimestamp(mHarddiskPlayer->GetEndTimestamp());
	SetRecordCurrentTimestamp(0);

	SetRecordSampleRateMillis(config.mDynamicData.mSampleTime.count());
	SetRecordFilename(QString::fromStdString(config.mDynamicData.mFile.generic_string()));
	SetRecordDataSource(QString::fromStdString(config.mDynamicData.mDataSource));
	SetRecordTrackName(QString::fromStdString(config.mTrackInformation.mName));
	SetRecordVehicleName(QString::fromStdString(config.mDynamicData.mVehicle));
	SetRecordComment(QString::fromStdString(config.mDynamicData.mComment));

	QVector<QUrl> videoFiles{
		CommonUI::QUrlFromPath(config.mVideoData[0]),
		CommonUI::QUrlFromPath(config.mVideoData[1]),
		CommonUI::QUrlFromPath(config.mVideoData[2]),
	};
	SetVideoStreams(videoFiles);
}

void HarddiskPlayerController::play()
{
	mPlayTimer.start(recordSampleRateMillis);
	SetRunning(true);
}

void HarddiskPlayerController::pause()
{
	mPlayTimer.stop();
	SetRunning(false);
}

void HarddiskPlayerController::seek(int frames)
{
	qint64 frame = recordCurrentFrame + frames;
	std::cout << "seek" << std::endl;
	jump(frame);
}

void HarddiskPlayerController::jump(qint64 frame)
{
	SetRecordCurrentFrame(std::max(std::min(frame, static_cast<qint64>(recordFramesCount-1)), qint64(0)));
	TransmitCurrentFrame();
	SetRecordCurrentTimestamp(mHarddiskPlayer->GetTimestamp());
	emit jumped();
}

void HarddiskPlayerController::handlePlayTimerTimeout()
{
	TransmitCurrentFrame();
	SetRecordCurrentFrame(recordCurrentFrame+1);
	SetRecordCurrentTimestamp(mHarddiskPlayer->GetTimestamp());
}


void HarddiskPlayerController::TransmitCurrentFrame()
{
	mHarddiskPlayer->ReadFrame(recordCurrentFrame);

	Common::OutboundPacket packet;
	Proto::Dynamics dynamics = mHarddiskPlayer->GetDynamics();
	Proto::DriverInput driverInput = mHarddiskPlayer->GetDriverInput();
	Proto::Powertrain powertrain = mHarddiskPlayer->GetPowertrain();
	Proto::Gps gps = mHarddiskPlayer->GetGps();

	packet.AddMessage(Proto::DYNAMICS_MSG, dynamics);
	packet.AddMessage(Proto::DRIVER_INPUT_MSG, driverInput);
	packet.AddMessage(Proto::POWERTRAIN_MSG, powertrain);
	packet.AddMessage(Proto::GPS_MSG, gps);

	mConnectionManager.Transmit(packet.GetData(), packet.GetSize());
}

}

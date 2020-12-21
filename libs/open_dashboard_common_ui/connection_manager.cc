// Copyright (C) 2020 twyleg
#include "connection_manager.h"

namespace OpenDashboard::CommonUI {

ConnectionManager::ConnectionManager()
{
	QObject::connect(&mAverageTransferRateTimer, SIGNAL(timeout()), this, SLOT(handleAverageTransferRateTimerTimeout()));
	mAverageTransferRateTimer.start(1000);
}

void ConnectionManager::initTransmitter(const QString& hostname, quint16 port)
{
	mUdpTransmitter = std::make_unique<Common::UdpTransmitter>(hostname.toStdString(), port);
}

void ConnectionManager::initReceiver(const QString& hostname, quint16 port)
{
	mUdpReceiver = std::make_unique<Common::UdpReceiver>(hostname.toStdString(), port);
}

size_t ConnectionManager::Transmit(const char* data, const size_t len)
{
	if (!mUdpTransmitter)
		return 0;

	SetTransmitCount(transmitCount + len);
	return mUdpTransmitter->Transmit(data, len);
}

size_t ConnectionManager::Receive(char* data, const size_t len)
{
	if (!mUdpReceiver)
		return 0;

	const size_t readCount = mUdpReceiver->Receive(data, len);
	SetReceiveCount(receiveCount + readCount);
	return readCount;
}

void ConnectionManager::handleAverageTransferRateTimerTimeout()
{

	SetTransmitBps(transmitCount - mPreviousTransmitCount);
	SetReceiveBps(receiveCount - mPreviousReceiveCount);

	mPreviousReceiveCount = receiveCount;
	mPreviousTransmitCount = transmitCount;
}

}

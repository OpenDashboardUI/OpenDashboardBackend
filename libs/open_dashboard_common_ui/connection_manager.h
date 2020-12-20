// Copyright (C) 2020 twyleg
#pragma once

#include <open_dashboard_common/udp_transceiver.h>
#include <open_dashboard_data_models/helper.h>

#include <QObject>
#include <QTimer>

#include <memory>

namespace OpenDashboard::CommonUI {

class ConnectionManager: public QObject
{
	Q_OBJECT

	ADD_PROPERTY(qint64, transmitCount, TransmitCount, 0);
	ADD_PROPERTY(qint64, transmitBps, TransmitBps, 0);
	ADD_PROPERTY(qint64, receiveCount, ReceiveCount, 0);
	ADD_PROPERTY(qint64, receiveBps, ReceiveBps, 0);

public:

	ConnectionManager();

	size_t Transmit(const char* data, const size_t);
	size_t Receive(char* data, const size_t);

public slots:

	void initTransmitter(const QString& hostname, quint16 port);
	void initReceiver(const QString& hostname, quint16 port);

private slots:

	void handleAverageTransferRateTimerTimeout();

private:

	QTimer mAverageTransferRateTimer;

	qint64 mPreviousTransmitCount = 0;
	qint64 mPreviousReceiveCount = 0;

	std::unique_ptr<Common::UdpTransmitter> mUdpTransmitter;
	std::unique_ptr<Common::UdpReceiver> mUdpReceiver;

};

}

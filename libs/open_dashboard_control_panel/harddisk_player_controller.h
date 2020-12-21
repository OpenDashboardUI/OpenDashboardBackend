// Copyright (C) 2020 twyleg
#pragma once
#include <open_dashboard_common/harddisk_player.h>
#include <open_dashboard_common/packet.h>
#include <open_dashboard_common/config.h>
#include <open_dashboard_common_ui/connection_manager.h>
#include <open_dashboard_data_models/helper.h>


#include <QObject>
#include <QUrl>
#include <QTimer>

#include <memory>

namespace OpenDashboard::ControlPanel {


class HarddiskPlayerController : public QObject
{
	Q_OBJECT

	ADD_PROPERTY(bool, running, Running, false);
	ADD_PROPERTY(quint64, recordFramesCount, RecordFramesCount, 0);
	ADD_PROPERTY(quint64, recordCurrentFrame, RecordCurrentFrame, 0);
	ADD_PROPERTY(double, recordCurrentTimestamp, RecordCurrentTimestamp, 0);
	ADD_PROPERTY(double, recordEndTimestamp, RecordEndTimestamp, 0);

	ADD_PROPERTY(QString, recordFilename, RecordFilename, "-");
	ADD_PROPERTY(QString, recordDataSource, RecordDataSource, "-");
	ADD_PROPERTY(QString, recordTrackName, RecordTrackName, "-");
	ADD_PROPERTY(QString, recordVehiclekName, RecordVehicleName, "-");
	ADD_PROPERTY(QString, recordComment, RecordComment, "-");
	ADD_PROPERTY(int, recordSampleRateMillis, RecordSampleRateMillis, 0);

	ADD_PROPERTY(QVector<QUrl>, videoStreams, VideoStreams, {});

public:

	HarddiskPlayerController(CommonUI::ConnectionManager&);

public slots:

	void openFile(const QUrl& filepath);

	void play();
	void pause();
	void seek(int frames);
	void jump(qint64 frame);

private slots:

	void handlePlayTimerTimeout();

signals:

	void jumped();

private:

	void TransmitCurrentFrame();

	CommonUI::ConnectionManager& mConnectionManager;
	std::unique_ptr<Common::HarddiskPlayer> mHarddiskPlayer;

	QTimer mPlayTimer;

};


}

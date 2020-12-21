// Copyright (C) 2020 twyleg
#pragma once

#include <QObject>
#include <QUrl>

#include <exception>
#include <filesystem>

namespace OpenDashboard::CommonUI {

class MapGenerator: public QObject {

	Q_OBJECT

public:

	MapGenerator(const std::filesystem::path& outputDirectory);

	QUrl GetMapViewFilePath() const;

public slots:

	QUrl generateMap(const QString& apiKeyString);

private:

	void InsertApiKey(std::string &templateString, const std::string& apiKeyString) const;
	std::string ReadTemplateFromFile(const QString& path) const;
	QUrl WriteMapViewToFile(const std::string &outputString) const;

	const std::filesystem::path mOutputDirectory;
	QUrl mMapViewFilePath;

};

}

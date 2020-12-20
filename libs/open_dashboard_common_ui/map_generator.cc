// Copyright (C) 2020 twyleg
#include "map_generator.h"
#include "helper.h"

#include <open_dashboard_common/helper.h>

#include <QFile>

#include <iostream>
#include <fstream>
#include <filesystem>

namespace OpenDashboard::CommonUI {

namespace {
constexpr size_t TEMPLATE_READ_BUFFER_SIZE = 64;
constexpr const char* TEMPLATE_FILE_QRC_PATH = ":/common_ui/html/map.html_template";
constexpr const char* OUTPUT_FILE_NAME = "map.html";
}

MapGenerator::MapGenerator(const std::filesystem::path& outputDirectory)
	: mOutputDirectory(outputDirectory)
{
	std::cout << "MapGenerator output directory: " << outputDirectory << std::endl;
}

QUrl MapGenerator::generateMap(const QString &apiKeyString)
{
	std::string templateString = ReadTemplateFromFile(TEMPLATE_FILE_QRC_PATH);
	InsertApiKey(templateString, apiKeyString.toStdString());
	mMapViewFilePath = WriteMapViewToFile(templateString);

	return mMapViewFilePath;
}

std::string MapGenerator::ReadTemplateFromFile(const QString& path) const
{
	char inputBuffer[TEMPLATE_READ_BUFFER_SIZE];
	std::string templateString;

	QFile templateFile(path);
	if (!templateFile.open(QIODevice::ReadOnly))
		throw std::invalid_argument(FORMAT("Unable to open input file: {}", path.toStdString()));

	while (qint64 readCount = templateFile.read(inputBuffer, TEMPLATE_READ_BUFFER_SIZE))
		templateString.append(inputBuffer, readCount);

	return std::move(templateString);
}

QUrl MapGenerator::WriteMapViewToFile(const std::string &outputString) const
{

	auto outputFilePath = mOutputDirectory / OUTPUT_FILE_NAME;
	std::ofstream outputFileStream(outputFilePath, std::ios::out);
	outputFileStream << outputString;
	outputFileStream.close();

	return QUrlFromPath(outputFilePath);
}

void MapGenerator::InsertApiKey(std::string& templateString, const std::string& apiKeyString) const
{
	const std::string searchString = "${api_key}";
	const size_t findResult = templateString.find(searchString);
	templateString.replace(findResult, searchString.size(), apiKeyString);
}

}

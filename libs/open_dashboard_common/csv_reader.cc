// Copyright (C) 2020 twyleg
#include "csv_reader.h"
#include "helper.h"

#include <boost/algorithm/string.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

namespace OpenDashboard::Common {

CsvReader::CsvReader(const std::filesystem::path& filepath, const std::string& charFilterString)
:	mInputFile(filepath.string()),
	mCharFilterString(charFilterString)
{}

void CsvReader::ReadHeader()
{
	std::string headerLine;
	std::getline(mInputFile, headerLine);

	std::vector<std::string> headerParts;

	boost::split(headerParts, headerLine, boost::is_any_of(","));

	for (std::size_t i=0; i<headerParts.size(); ++i)
	{
		std::string columnName = headerParts[i];
		boost::range::remove_erase_if(columnName, boost::is_any_of(mCharFilterString));
		mColumnNames.insert({columnName, i});
	}
}

bool CsvReader::IgnoreLine()
{
	std::string line;
	std::getline(mInputFile, line);
	return !line.empty();
}

void CsvReader::ReadFileToCache(bool readHeader, size_t ignoreLines)
{
	if (readHeader)
		ReadHeader();

	for (size_t i = 0; i<ignoreLines; ++i)
	{
		if (!IgnoreLine())
			return;
	}

	std::string line;
	std::getline(mInputFile, line);
	while (!line.empty())
	{
		boost::range::remove_erase_if(line, boost::is_any_of(mCharFilterString));
		mLineCache.push_back(line);
		std::getline(mInputFile, line);
	}
}

void CsvReader::ReadLine(const size_t lineNumber)
{
	THROW_IF(lineNumber > mLineCache.size(), "Reuqest line (line number={}) out of bounds", lineNumber);

	std::string line = mLineCache[lineNumber];
	std::vector<std::string> parts;

	boost::split(parts, line, boost::is_any_of(","));
	THROW_IF(parts.size() != mColumnNames.size(), "CSV line has unexpected number of columns: {}", parts.size());

	mCurrentLineContent = std::move(parts);
}

}

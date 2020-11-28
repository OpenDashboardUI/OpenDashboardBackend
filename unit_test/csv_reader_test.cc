// Copyright (C) 2020 twyleg
#include <gtest/gtest.h>

#include <open_dashboard_common/csv_reader.h>

#include <fmt/format.h>

#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstdlib>

namespace OpenDashboard::testing {

class CsvReaderTest : public ::testing::Test {

public:

using Line = std::vector<std::string>;

void SetUp() override
{
	std::filesystem::remove(mTestFile);
}

void WriteCsvFile(const std::filesystem::path& filepath, const std::vector<Line>& lines)
{
	std::ofstream ostream(filepath);

	for (auto& line: lines)
	{
		for (auto it=line.begin(); it!=line.end(); ++it)
		{
			if (it != line.begin())
				ostream << ',';
			ostream << *it;
		}
		ostream << std::endl;
	}

	ostream.close();
}

void PrepareValidCsvFile()
{
	WriteCsvFile(mTestFile,
			 {
					{"ColA","ColB","ColC"},
					{"double","int","string"},
					{"foo","foo","foo"},
					{"1.1","1","one"},
					{"2.1","2","two"},
					{"3.1","3","three"},
					{"4.1","4","four"}
			 }
	);

	mCsvReader = std::make_unique<Common::CsvReader>(mTestFile);
	mCsvReader->ReadFileToCache(true, 2);
}

void PrepareValidCsvFileWithSpecialCharacters()
{
	WriteCsvFile(mTestFile,
			 {
					{"ColA","ColB","ColC\r"},
					{"double","int","string\r"},
					{"foo","foo","foo\r"},
					{"1.1","1","one\r"},
					{"2.1","2","two\r"},
					{"3.1","3","three\r"},
					{"4.1","4","four\r"}
			 }
	);

	mCsvReader = std::make_unique<Common::CsvReader>(mTestFile);
	mCsvReader->ReadFileToCache(true, 2);
}

void PrepareInvalidCsvFile()
{
	WriteCsvFile(mTestFile,
			 {
					 {"ColA","ColB","ColC"},
					 {"double","int","string"},
					 {"foo","foo","foo"},
					 {"1.1","2","one"},
					 {"2.2","2","two","Reason for Error"},
			 }
	);

	mCsvReader = std::make_unique<Common::CsvReader>(mTestFile);
	mCsvReader->ReadFileToCache(true, 2);
}

void ExpectCorrectParsedCsvFile()
{
	mCsvReader->ReadLine(0);
	EXPECT_NEAR(mCsvReader->GetValue<double>("ColA"), 1.1, 0.001);
	EXPECT_EQ(mCsvReader->GetValue<int>("ColB"), 1);
	EXPECT_EQ(mCsvReader->GetValue<std::string>("ColC"), "one");

	mCsvReader->ReadLine(1);
	EXPECT_NEAR(mCsvReader->GetValue<double>("ColA"), 2.1, 0.001);
	EXPECT_EQ(mCsvReader->GetValue<int>("ColB"), 2);
	EXPECT_EQ(mCsvReader->GetValue<std::string>("ColC"), "two");

	mCsvReader->ReadLine(2);
	EXPECT_NEAR(mCsvReader->GetValue<double>("ColA"), 3.1, 0.001);
	EXPECT_EQ(mCsvReader->GetValue<int>("ColB"), 3);
	EXPECT_EQ(mCsvReader->GetValue<std::string>("ColC"), "three");

	mCsvReader->ReadLine(3);
	EXPECT_NEAR(mCsvReader->GetValue<double>("ColA"), 4.1, 0.001);
	EXPECT_EQ(mCsvReader->GetValue<int>("ColB"), 4);
	EXPECT_EQ(mCsvReader->GetValue<std::string>("ColC"), "four");
}

const std::filesystem::path mTestFile = "test_file.csv";
std::unique_ptr<Common::CsvReader> mCsvReader;

};


TEST_F(CsvReaderTest, ValidCsvFile_GetAllAttributes_Success)
{
	PrepareValidCsvFile();
	ExpectCorrectParsedCsvFile();
}

TEST_F(CsvReaderTest, ValidCsvFileWithSpecialCharacters_GetAllAttributes_Success)
{
	PrepareValidCsvFileWithSpecialCharacters();
	ExpectCorrectParsedCsvFile();
}

TEST_F(CsvReaderTest, ValidCsvFile_ReadUnknownAttribute_Throw)
{
	PrepareValidCsvFile();
	mCsvReader->ReadLine(0);
	EXPECT_THROW(mCsvReader->GetValue<double>("MissingColumn"), std::runtime_error);
}

TEST_F(CsvReaderTest, ValidCsvFile_ReadUnknownAttributeByIndex_Throw)
{
	PrepareValidCsvFile();
	mCsvReader->ReadLine(0);
	EXPECT_THROW(mCsvReader->GetValue<double>(4), std::runtime_error);
}

TEST_F(CsvReaderTest, InvalidCsvFileWith_ReadMalformedLine_Throw)
{
	PrepareInvalidCsvFile();
	mCsvReader->ReadLine(0);
	EXPECT_THROW(mCsvReader->ReadLine(1), std::runtime_error);
}

TEST_F(CsvReaderTest, ValidCsvFile_ReadMoreLinesThanAvailable_EndOfFileIndicated)
{
	PrepareValidCsvFile();
	EXPECT_EQ(mCsvReader->GetNumberOfLines(), 4);
}

}


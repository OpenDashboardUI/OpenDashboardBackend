// Copyright (C) 2020 twyleg
#include "config.h"
#include "helper.h"
#include "xml_reader.h"

#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

#include <cstdlib>

namespace OpenDashboard::Common {

namespace {

const std::string OPEN_DASHBOARD_XSD = R"(<?xml version="1.0"?>
<xs:schema
	   id="open_dashboard"
	   xmlns:xs="http://www.w3.org/2001/XMLSchema"
	   elementFormDefault="qualified">

	  <xs:complexType name="DynamicDataType">
		  <xs:sequence>
			  <xs:element name="File" type="xs:string"/>
			  <xs:element name="DataSource" type="xs:string"/>
			  <xs:element name="Vehicle" type="xs:string"/>
			  <xs:element name="SampleTime" type="xs:positiveInteger"/>
			  <xs:element name="Comment" type="xs:string"/>
		  </xs:sequence>
	  </xs:complexType>

	   <xs:complexType name="VideoDataType">
		   <xs:sequence>
			   <xs:element name="File" type="xs:string" minOccurs='0' maxOccurs='3'/>
		   </xs:sequence>
	   </xs:complexType>

	   <xs:element name="OpenDashboard">
		   <xs:complexType>
			   <xs:sequence>
					<xs:element name="TrackInformation"/>
				   <xs:element name="DynamicData" type="DynamicDataType"/>
				   <xs:element name="VideoData" type="VideoDataType"/>
			   </xs:sequence>
		   </xs:complexType>
	   </xs:element>
   </xs:schema>
)";

const std::string TRACK_INFORMATION_XSD = R"(<?xml version="1.0"?>
<xs:schema
	id="track_information"
	xmlns:xs="http://www.w3.org/2001/XMLSchema"
	elementFormDefault="qualified">

	<xs:simpleType name="LapTimeType">
		<xs:restriction base="xs:string">
			<xs:pattern value="\d{1,2}:\d{1,2}\.\d{1,3}"/>
		</xs:restriction>
	</xs:simpleType>

	<xs:simpleType name="PositiveDecimalType">
		<xs:restriction base="xs:decimal">
			<xs:minExclusive value="0.0"/>
		</xs:restriction>
	</xs:simpleType>

	<xs:element name="TrackInformation">
		<xs:complexType>
			<xs:sequence>
				<xs:element name="Name" type="xs:string"/>
				<xs:element name="Length" type="PositiveDecimalType"/>
				<xs:element name="LapRecord" type="LapTimeType"/>
			</xs:sequence>
		</xs:complexType>
	</xs:element>
</xs:schema>
)";


std::chrono::milliseconds ParseMilliseconds(const std::string& str)
{
	static const std::regex re(R"((\d{1,2}):(\d{1,2})\.(\d{1,3}))");
	std::smatch match;
	std::regex_search(str, match, re);
	int minutes = boost::lexical_cast<int>(match.str(1));
	int seconds = boost::lexical_cast<int>(match.str(2));
	int millis = boost::lexical_cast<int>(match.str(3));

	return std::chrono::minutes(minutes)
			+ std::chrono::seconds(seconds)
			+ std::chrono::milliseconds(millis);
}

std::filesystem::path GetAbsoluteSubconfigPath(const std::filesystem::path& parentConfigFilePath,
		const std::filesystem::path& subconfigFilePath)
{
	if (subconfigFilePath.is_absolute())
		return subconfigFilePath;
	else
		return parentConfigFilePath / subconfigFilePath;
}

void CheckFileExistence(const std::filesystem::path& filePath)
{
	THROW_IF(!std::filesystem::exists(filePath), "File does not exist: {}", filePath.string());
}

}

Config::TrackInformation ReadTrackInformation(const std::filesystem::path& filePath)
{
	Config::TrackInformation trackInformation;

	XmlReader trackInformationXmlReader(filePath, TRACK_INFORMATION_XSD, "track_information.xsd");
	const auto trackInformationElem = trackInformationXmlReader.GetDocumentElement();
	trackInformation.mName = XmlReader::GetContent<std::string>(
			XmlReader::GetFirstChildElement(trackInformationElem, "Name"));
	trackInformation.mLength = XmlReader::GetContent<double>(
			XmlReader::GetFirstChildElement(trackInformationElem, "Length"));
	const std::string lapRecord = XmlReader::GetContent<std::string>(
			XmlReader::GetFirstChildElement(trackInformationElem, "LapRecord"));
	trackInformation.mLapRecord = ParseMilliseconds(lapRecord);

	return trackInformation;
}

Config Config::ReadConfig(const std::filesystem::path& filePath)
{
	Config config;

	const std::filesystem::path configFileBaseDir = filePath.parent_path();
	XmlReader openDashboardXmlReader(filePath, OPEN_DASHBOARD_XSD, "open_dashboard.xsd");

	const auto openDashboardElem = openDashboardXmlReader.GetDocumentElement();


	const auto dynamicDataElem = XmlReader::GetFirstChildElement(openDashboardElem, "DynamicData");
	const auto dynamicDataPath = XmlReader::GetContent<std::string>(
			XmlReader::GetFirstChildElement(dynamicDataElem, "File"));
	const auto absoluteDynamicDataPath = GetAbsoluteSubconfigPath(configFileBaseDir, dynamicDataPath);
	CheckFileExistence(absoluteDynamicDataPath);
	config.mDynamicData.mFile = absoluteDynamicDataPath;

	config.mDynamicData.mDataSource = XmlReader::GetContent<std::string>(
				XmlReader::GetFirstChildElement(dynamicDataElem, "DataSource"));

	config.mDynamicData.mVehicle = XmlReader::GetContent<std::string>(
				XmlReader::GetFirstChildElement(dynamicDataElem, "Vehicle"));

	config.mDynamicData.mSampleTime = std::chrono::milliseconds(XmlReader::GetContent<int>(
			XmlReader::GetFirstChildElement(dynamicDataElem, "SampleTime")));

	config.mDynamicData.mComment = XmlReader::GetContent<std::string>(
				XmlReader::GetFirstChildElement(dynamicDataElem, "Comment"));


	const auto videoDataElem = XmlReader::GetFirstChildElement(openDashboardElem, "VideoData");
	auto videoFiles = videoDataElem->getElementsByTagName(XmlReader::Tc("File"));
	for (size_t i = 0; i<videoFiles->getLength(); ++i)
	{
		const auto videoFileElement = dynamic_cast<xercesc::DOMElement*>(videoFiles->item(i));
		const auto  videoFilePath = XmlReader::GetContent<std::string>(videoFileElement);
		const auto absoluteVideoFilePath = GetAbsoluteSubconfigPath(configFileBaseDir, videoFilePath);
		CheckFileExistence(absoluteVideoFilePath);
		config.mVideoData[i] = absoluteVideoFilePath;
	}

	//
	// Subconfigs
	//
	const auto trackInformationElem = XmlReader::GetFirstChildElement(openDashboardElem, "TrackInformation");
	const auto trackInformationFilePath = XmlReader::GetContent<std::string>(
			XmlReader::GetFirstChildElement(openDashboardElem, "TrackInformation"));
	const auto absoluteTrackInformationFilePath = GetAbsoluteSubconfigPath(configFileBaseDir, trackInformationFilePath);
	CheckFileExistence(absoluteTrackInformationFilePath);
	config.mTrackInformation = ReadTrackInformation(absoluteTrackInformationFilePath);

	return config;
}

}

// Copyright (C) 2020 twyleg
#include "config.h"
#include "helper.h"

#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/ErrorHandler.hpp>

#include <boost/lexical_cast.hpp>

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
			  <xs:element name="SampleTime" type="xs:positiveInteger"/>
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

struct XmlParseErrorHandler : public xercesc::ErrorHandler
{
	void warning (const xercesc::SAXParseException& exc) final { throw (exc); }
	void error (const xercesc::SAXParseException& exc) final { throw (exc);	}
	void fatalError (const xercesc::SAXParseException& exc) final { throw (exc); }
	void resetErrors () final {}
};

XMLCh* Tc(const char* str)
{
	return xercesc::XMLString::transcode(str);
}

char* Tc(const XMLCh* str)
{
	return xercesc::XMLString::transcode(str);
}

xercesc::DOMElement* GetFirstChildElement(const xercesc::DOMElement* elem, const std::string& childName)
{
	return dynamic_cast<xercesc::DOMElement*>(elem->getElementsByTagName(Tc(childName.c_str()))->item(0));
}

template <class T>
T GetContent(const xercesc::DOMElement* elem)
{
	return boost::lexical_cast<T>(Tc(elem->getTextContent()));
}

class XmlReader
{
public:

	XmlReader(const std::filesystem::path xmlFilePath, const std::string& xsdString, const std::string& xsdPseudoFilename)
	{
		mConfigFileParser.setErrorHandler(&mErrorHandler);
		mConfigFileParser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
		mConfigFileParser.setDoNamespaces(true);
		mConfigFileParser.setDoSchema(true);
		mConfigFileParser.setValidationConstraintFatal(true);
		mConfigFileParser.cacheGrammarFromParse(true);

		LoadXsdSchema(xsdString, xsdPseudoFilename);
		ParseXmlFile(xmlFilePath, xsdString, xsdPseudoFilename);
	}

	xercesc::DOMElement* GetDocumentElement()
	{
		return mConfigFileParser.getDocument()->getDocumentElement();
	}

private:

	std::string GetLineFromStream(std::istream& istream, const size_t requestedLineNumber)
	{
		std::ostringstream formattedOutput;
		size_t lineNumber = 1;
		while (!istream.eof())
		{
			std::string line;
			std::getline(istream, line);

			if (lineNumber == requestedLineNumber)
			{
				formattedOutput << "Line " << lineNumber << ": " << line;
				return formattedOutput.str();
			}
			++lineNumber;
		}
		throw std::runtime_error(FORMAT("Requested line number {} exceeds length of stream.", requestedLineNumber));
	}

	void LoadXsdSchema(const std::string& xsdString, const std::string& xsdPseudoFilename)
	{
		xercesc::MemBufInputSource inMemorySchemaSource(reinterpret_cast<const XMLByte*>(
				xsdString.c_str()), xsdString.size(), xsdPseudoFilename.c_str());
		try
		{
			mConfigFileParser.loadGrammar(inMemorySchemaSource, xercesc::Grammar::SchemaGrammarType, true);
		}
		catch (const xercesc::SAXParseException& exc)
		{
			std::cerr << "XmlReaderError - Invalid XSD file!" << std::endl;
			std::stringstream istream(xsdString, std::ios_base::in);
			std::cerr << "XmlReaderError " << GetLineFromStream(istream, exc.getLineNumber()) << std::endl;
			char* errorMessage = Tc(exc.getMessage());
			std::cerr << "XmlReaderError " << errorMessage << std::endl;
			std::free(errorMessage);
			throw exc;
		}
	}

	void ParseXmlFile(const std::filesystem::path xmlFilePath, const std::string& xsdString,
			const std::string& xsdPseudoFilename)
	{
		THROW_IF(!std::filesystem::exists(xmlFilePath), "XmlReaderError - File \"{}\" does not exist.",
				 xmlFilePath.string());
		try
		{
			mConfigFileParser.parse(xmlFilePath.generic_string().c_str());
		}
		catch (const xercesc::SAXParseException& exc)
		{
			std::cerr << "XmlReaderError - Invalid XML file!" << std::endl;
			std::ifstream istream(xmlFilePath, std::ios_base::in);
			std::cerr << "XmlReaderError " << GetLineFromStream(istream, exc.getLineNumber()) << std::endl;
			char* errorMessage = Tc(exc.getMessage());
			std::cerr << "XmlReaderError " << errorMessage << std::endl;
			std::free(errorMessage);
			throw exc;
		}

	}

	XmlParseErrorHandler mErrorHandler;
	xercesc::XercesDOMParser mConfigFileParser;
};

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
	trackInformation.mName = GetContent<std::string>(GetFirstChildElement(trackInformationElem, "Name"));
	trackInformation.mLength = GetContent<double>(GetFirstChildElement(trackInformationElem, "Length"));
	const std::string lapRecord = GetContent<std::string>(GetFirstChildElement(trackInformationElem, "LapRecord"));
	trackInformation.mLapRecord = ParseMilliseconds(lapRecord);

	return trackInformation;
}

Config Config::ReadConfig(const std::filesystem::path& filePath)
{
	Config config;

	const std::filesystem::path configFileBaseDir = filePath.parent_path();
	xercesc::XMLPlatformUtils::Initialize();
	XmlReader openDashboardXmlReader(filePath, OPEN_DASHBOARD_XSD, "open_dashboard.xsd");

	const auto openDashboardElem = openDashboardXmlReader.GetDocumentElement();


	const auto dynamicDataElem = GetFirstChildElement(openDashboardElem, "DynamicData");
	const auto dynamicDataPath = GetContent<std::string>(
			GetFirstChildElement(dynamicDataElem, "File"));
	const auto absoluteDynamicDataPath = GetAbsoluteSubconfigPath(configFileBaseDir, dynamicDataPath);
	CheckFileExistence(absoluteDynamicDataPath);
	config.mDynamicData.mFile = absoluteDynamicDataPath;
	config.mDynamicData.mSampleTime = std::chrono::milliseconds(GetContent<int>(
			GetFirstChildElement(dynamicDataElem, "SampleTime")));


	const auto videoDataElem = GetFirstChildElement(openDashboardElem, "VideoData");
	auto videoFiles = videoDataElem->getElementsByTagName(Tc("File"));
	for (size_t i = 0; i<videoFiles->getLength(); ++i)
	{
		const auto videoFileElement = dynamic_cast<xercesc::DOMElement*>(videoFiles->item(i));
		const auto  videoFilePath = GetContent<std::string>(videoFileElement);
		const auto absoluteVideoFilePath = GetAbsoluteSubconfigPath(configFileBaseDir, videoFilePath);
		CheckFileExistence(absoluteVideoFilePath);
		config.mVideoData[i] = absoluteVideoFilePath;
	}

	//
	// Subconfigs
	//
	const auto trackInformationElem = GetFirstChildElement(openDashboardElem, "TrackInformation");
	const auto trackInformationFilePath = GetContent<std::string>(GetFirstChildElement(openDashboardElem,
			"TrackInformation"));
	const auto absoluteTrackInformationFilePath = GetAbsoluteSubconfigPath(configFileBaseDir, trackInformationFilePath);
	CheckFileExistence(absoluteTrackInformationFilePath);
	config.mTrackInformation = ReadTrackInformation(absoluteTrackInformationFilePath);

	return config;
}

}

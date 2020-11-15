#include "xml_reader.h"
#include "helper.h"

#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include <exception>
#include <iostream>
#include <fstream>

namespace OpenDashboard::Common {

XmlReader::XmlReader(const std::filesystem::path xmlFilePath, const std::string& xsdString,
		const std::string& xsdPseudoFilename)
{
	mFileParser.setErrorHandler(&mErrorHandler);
	mFileParser.setValidationScheme(xercesc::XercesDOMParser::Val_Always);
	mFileParser.setDoNamespaces(true);
	mFileParser.setDoSchema(true);
	mFileParser.setValidationConstraintFatal(true);
	mFileParser.cacheGrammarFromParse(true);

	LoadXsdSchema(xsdString, xsdPseudoFilename);
	ParseXmlFile(xmlFilePath, xsdString, xsdPseudoFilename);
}

xercesc::DOMElement* XmlReader::GetDocumentElement()
{
	return mFileParser.getDocument()->getDocumentElement();
}

std::string XmlReader::GetLineFromStream(std::istream& istream, const size_t requestedLineNumber)
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

void XmlReader::LoadXsdSchema(const std::string& xsdString, const std::string& xsdPseudoFilename)
{
	xercesc::MemBufInputSource inMemorySchemaSource(reinterpret_cast<const XMLByte*>(
			xsdString.c_str()), xsdString.size(), xsdPseudoFilename.c_str());
	try
	{
		mFileParser.loadGrammar(inMemorySchemaSource, xercesc::Grammar::SchemaGrammarType, true);
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

void XmlReader::ParseXmlFile(const std::filesystem::path xmlFilePath, const std::string& xsdString,
		const std::string& xsdPseudoFilename)
{
	THROW_IF(!std::filesystem::exists(xmlFilePath), "XmlReaderError - File \"{}\" does not exist.",
			 xmlFilePath.string());
	try
	{
		mFileParser.parse(xmlFilePath.generic_string().c_str());
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

XMLCh* XmlReader::Tc(const char* str)
{
	return xercesc::XMLString::transcode(str);
}

char* XmlReader::Tc(const XMLCh* str)
{
	return xercesc::XMLString::transcode(str);
}

xercesc::DOMElement* XmlReader::GetFirstChildElement(const xercesc::DOMElement* elem, const std::string& childName)
{
	return dynamic_cast<xercesc::DOMElement*>(elem->getElementsByTagName(Tc(childName.c_str()))->item(0));
}

}

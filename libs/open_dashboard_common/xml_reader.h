// Copyright (C) 2020 twyleg
#pragma once

#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/sax/ErrorHandler.hpp>
#include <xercesc/sax/SAXParseException.hpp>

#include <boost/lexical_cast.hpp>

#include <filesystem>

namespace OpenDashboard::Common {

class XmlReader
{
public:

	struct XmlParseErrorHandler : public xercesc::ErrorHandler
	{
		void warning (const xercesc::SAXParseException& exc) final { throw (exc); }
		void error (const xercesc::SAXParseException& exc) final { throw (exc);	}
		void fatalError (const xercesc::SAXParseException& exc) final { throw (exc); }
		void resetErrors () final {}
	};

	XmlReader(const std::filesystem::path xmlFilePath, const std::string& xsdString, const std::string& xsdPseudoFilename);

	xercesc::DOMElement* GetDocumentElement();

	static XMLCh* Tc(const char* str);
	static char* Tc(const XMLCh* str);
	static xercesc::DOMElement* GetFirstChildElement(const xercesc::DOMElement* elem, const std::string& childName);

	template <class T>
	static T GetContent(const xercesc::DOMElement* elem)
	{
		return boost::lexical_cast<T>(Tc(elem->getTextContent()));
	}

private:

	struct Initializer
	{
		Initializer()
		{
			xercesc::XMLPlatformUtils::Initialize();
		}
	};

	std::string GetLineFromStream(std::istream& istream, const size_t requestedLineNumber);
	void LoadXsdSchema(const std::string& xsdString, const std::string& xsdPseudoFilename);
	void ParseXmlFile(const std::filesystem::path xmlFilePath, const std::string& xsdString,
			const std::string& xsdPseudoFilename);

	Initializer mInitializer;
	XmlParseErrorHandler mErrorHandler;
	xercesc::XercesDOMParser mFileParser;

};

}

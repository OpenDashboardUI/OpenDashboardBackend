#include "frontend_config.h"

#include <open_dashboard_common/xml_reader.h>
#include <open_dashboard_common/helper.h>

namespace OpenDashboard {

namespace {

const std::string OPEN_DASHBOARD_FRONTEND_CONFIG_XSD = R"(<?xml version="1.0"?>
<xs:schema
	   id="open_dashboard_frontend_config"
	   xmlns:xs="http://www.w3.org/2001/XMLSchema"
	   elementFormDefault="qualified">

	   <xs:complexType name="QmlModuleImportPathsType">
		   <xs:sequence>
			   <xs:element name="Directory" type="xs:string" minOccurs="0"/>
		   </xs:sequence>
	   </xs:complexType>

	   <xs:element name="OpenDashboardFrontendConfig">
		   <xs:complexType>
			   <xs:sequence>
				   <xs:element name="QmlMainFile" type="xs:string"/>
				   <xs:element name="QmlModuleImportPaths" type="QmlModuleImportPathsType"/>
			   </xs:sequence>
		   </xs:complexType>
	   </xs:element>
   </xs:schema>
)";

std::filesystem::path GetAbsolutePath(const std::filesystem::path& frontendConfigFilePath,
		const std::filesystem::path& relativePath)
{
	const std::filesystem::path frontendConfigFileBaseDir = std::filesystem::absolute(frontendConfigFilePath).parent_path();

	return frontendConfigFileBaseDir / relativePath;
}

void CheckDirectoryExistence(const std::filesystem::path& directoryPath)
{
	THROW_IF(!std::filesystem::exists(directoryPath), "Import directory does not exist: {}",
			directoryPath.generic_string());
}

}

FrontendConfig::FrontendConfig(const std::filesystem::path& frontendConfigFile)
{
	Common::XmlReader xmlReader(frontendConfigFile, OPEN_DASHBOARD_FRONTEND_CONFIG_XSD,
			"open_dashboard_frontend_config.xsd");

	const auto openDashboardFrontendConfigElem = xmlReader.GetDocumentElement();
	const auto qmlMainFilePath = Common::XmlReader::GetContent<std::string>(
			Common::XmlReader::GetFirstChildElement(openDashboardFrontendConfigElem, "QmlMainFile"));
	const auto absoluteQmlMainFilePath = GetAbsolutePath(frontendConfigFile, qmlMainFilePath);
	mQmlMainFile = absoluteQmlMainFilePath;

	const auto qmlModuleImportPathsElem = Common::XmlReader::GetFirstChildElement(openDashboardFrontendConfigElem,
			"QmlModuleImportPaths");
	auto importDirectoryElems = qmlModuleImportPathsElem->getElementsByTagName(Common::XmlReader::Tc("Directory"));
	for (size_t i = 0; i<importDirectoryElems->getLength(); ++i)
	{
		const auto importDirectoryElem = dynamic_cast<xercesc::DOMElement*>(importDirectoryElems->item(i));
		const auto importDirectoryPath = Common::XmlReader::GetContent<std::string>(importDirectoryElem);
		const auto absoluteImportDirectoryPath = GetAbsolutePath(frontendConfigFile, importDirectoryPath);
		CheckDirectoryExistence(absoluteImportDirectoryPath);
		mQmlImportPaths.emplace_back(absoluteImportDirectoryPath);
	}


}

}

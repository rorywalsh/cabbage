#pragma once

#include "Parameter.hpp"
#include <vector>

class XMLStringBuilder
{
public:
	XMLStringBuilder(std::vector<Parameter>& parameters, const std::string& name, const std::string& type, const std::string& id);
	~XMLStringBuilder();

	void CreateXMLTags(const std::string& name, const std::string& type,const std::string& id);
	void CreateXMLProperties(std::vector<Parameter>& parameters);

	std::string CreatePropertyTag(Parameter& p);
	std::string CreateUserInterFaceTag(Parameter& p);
	std::string CreateDefaultTag(Parameter& p);
	std::string CreatePropertyIDTag(Parameter& p);
	std::string CreateRestrictionsTag(Parameter& p);

	std::string GetXMLString();

private:
	std::string m_XMLHeader;
	std::string m_XMLProperties;
	std::string m_XMLFooter;
};
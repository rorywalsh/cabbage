#include "XMLStringBuilder.hpp"
#include <algorithm>

XMLStringBuilder::XMLStringBuilder(
	std::vector<Parameter>& parameters, 
	const std::string& name, 
	const std::string& type, 
	const std::string& id) 
{
	CreateXMLTags(name, type, id);
	CreateXMLProperties(parameters);
}

XMLStringBuilder::~XMLStringBuilder(){}

void XMLStringBuilder::CreateXMLTags(const std::string& name, const std::string& type,const std::string& id)
{
	m_XMLHeader = " <?xml version = \"1.0\" encoding=\"utf-8\"?>\n"
		"\t<PluginModule>\n"
		"\t\t<" + type + " Name=\"" + name + "\" CompanyID=\"64\" PluginID=\"" + id + "\">\n"
		"\t\t\t<PluginInfo>\n"
		"\t\t\t\t<PlatformSupport>\n"
		"\t\t\t\t\t<Platform Name=\"Any\"></Platform>\n"
		"\t\t\t\t</PlatformSupport>\n"
		"\t\t\t</PluginInfo>\n"
		"\t\t\t<Properties>\n";
		
	m_XMLFooter = 
		"\t\t\t</Properties>\n"
		"\t\t</" + type + ">\n"
		"\t</PluginModule>";
}

void XMLStringBuilder::CreateXMLProperties(std::vector<Parameter>& parameters)
{
	for (auto p : parameters)
	{
		m_XMLProperties += CreatePropertyTag(p);
	}
}

std::string XMLStringBuilder::CreatePropertyTag(Parameter& p)
{
	std::string property = 
		"\n\t<Property"
		" Name=\"" + std::string(p.GetName()) + "\"" +
		" Type=\"" + p.GetType() + "\"" +
		" SupportRTPCType=\"" + "Exclusive" + "\"" +
		" DisplayName=\"" + p.GetName() + "\">";

	property += CreateUserInterFaceTag(p);
	property += CreateDefaultTag(p);
	property += CreatePropertyIDTag(p);
	property += CreateRestrictionsTag(p);
	property += "\t</Property>\n";

	return property;
}

std::string XMLStringBuilder::CreateUserInterFaceTag(Parameter& p)
{
	int decimels = 0;
	std::string stepSize = p.GetRange().m_StepSize;
	stepSize.erase(std::remove(stepSize.begin(), stepSize.end(), ' '));

	if ((stepSize.find_last_of(".") != std::string::npos))
	{
		decimels = (stepSize.size() - stepSize.find_last_of(".")) - 1;
	}

	return
		"\n\t\t<UserInterface"
		" Step=\"" + stepSize + "\"" +
		" Fine=\"" + std::to_string(decimels + 1) + "\""
		" Decimals=\"" + std::to_string(decimels) + "\"/>\n";
}

std::string XMLStringBuilder::CreateDefaultTag(Parameter& p)
{
	return "\t\t<DefaultValue>" + std::to_string(p.GetRange().m_DefaultVal) + "</DefaultValue>\n";
}

std::string XMLStringBuilder::CreatePropertyIDTag(Parameter& p)
{
	return "\t\t<AudioEnginePropertyID>" + std::to_string(p.GetID()) + "</AudioEnginePropertyID>\n";
}

std::string XMLStringBuilder::CreateRestrictionsTag(Parameter& p)
{
	return
		"\t\t<Restrictions>\n\t\t\t<ValueRestriction>\n\t\t\t\t"
		"<Range Type=\"" + p.GetType() + "\">\n\t\t\t\t\t"
		"<Min>" + std::to_string(p.GetRange().m_Min) + "</Min>\n\t\t\t\t\t"
		"<Max>" + std::to_string(p.GetRange().m_Max) + "</Max>"
		"\n\t\t\t\t</Range>\n\t\t\t</ValueRestriction>\n\t\t</Restrictions>\n";
}

std::string XMLStringBuilder::GetXMLString()
{
	return m_XMLHeader + m_XMLProperties + m_XMLFooter;
}

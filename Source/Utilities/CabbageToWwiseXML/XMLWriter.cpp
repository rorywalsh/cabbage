#include "XMLWriter.hpp"
#include <sstream> 
#include <iostream>
#include <algorithm>

XMLWriter::XMLWriter(std::vector<Parameter>& parameters, std::string inFile) :
	m_Parameters(parameters),
	m_InFilePath("tone.xml"),
	m_OutFilePath("temp.xml"){}

XMLWriter::~XMLWriter() {}

void XMLWriter::WriteUserInterFaceTag(Parameter& p)
{
	int decimels = 0;
	std::string stepSize = p.GetRange().m_StepSize;
	stepSize.erase(std::remove(stepSize.begin(), stepSize.end(), ' '));

	if ((stepSize.find_last_of(".") != std::string::npos))
	{
		decimels = (stepSize.size() - stepSize.find_last_of(".")) - 1;
	}

	m_XMLOutFIle << "\t\t<UserInterface"
		" Step=\"" << stepSize << "\"" <<
		" Fine=\"" << (decimels + 1) << "\"" <<
		" Decimals=\"" << decimels << "\"/>" << std::endl;
}

void XMLWriter::WriteDefaultTag(Parameter& p)
{
	m_XMLOutFIle << "\t\t<DefaultValue>" + std::to_string(p.GetRange().m_DefaultVal) + "</DefaultValue>" << std::endl;
}

void XMLWriter::WritePropertyIDTag(Parameter& p)
{
	m_XMLOutFIle << "\t\t<AudioEnginePropertyID>" << p.GetID() << "</AudioEnginePropertyID>" << std::endl;
}

void XMLWriter::WriteRestrictionsTag(Parameter& p)
{
	m_XMLOutFIle <<
		"\t\t<Restrictions>\n\t\t\t<ValueRestriction>\n\t\t\t\t"
		"<Range Type=\"" + p.GetType() + "\">\n\t\t\t\t\t"
		"<Min>" + std::to_string(p.GetRange().m_Min) + "</Min>\n\t\t\t\t\t"
		"<Max>" + std::to_string(p.GetRange().m_Max) + "</Max>"
		"\n\t\t\t\t</Range>\n\t\t\t</ValueRestriction>\n\t\t</Restrictions>" << std::endl;
}

void XMLWriter::WritePropertyTag(Parameter& p)
{
	//Write Opening Line of Property Tag
	m_XMLOutFIle << "\n\t<Property"
		" Name=\"" + std::string(p.GetName()) + "\"" +
		" Type=\"" + p.GetType() + "\"" +
		" SupportRTPCType=\"" + "Exclusive" + "\"" +
		" DisplayName=\"" + p.GetName() + "\">" << std::endl;

	WriteUserInterFaceTag(p);
	WriteDefaultTag(p);
	WritePropertyIDTag(p);
	WriteRestrictionsTag(p);

	//Write Closing Property Tag
	m_XMLOutFIle << "\t</Property>" << std::endl;
}

void XMLWriter::WriteAllPropertyTags()
{
	for (auto p : m_Parameters)
	{
		WritePropertyTag(p);
	}
}

bool XMLWriter::WriteToXML()
{
	m_XMLInFIle.open(m_InFilePath.c_str());
	m_XMLOutFIle.open(m_OutFilePath.c_str());

	if (!m_XMLInFIle.is_open())
	{
		CreateXML();
		m_XMLInFIle.open(m_InFilePath.c_str());
	}

	bool in = m_XMLInFIle.is_open();
	bool out = m_XMLOutFIle.is_open();

	if (m_XMLInFIle.is_open() && m_XMLOutFIle.is_open())
	{
		std::string line;
		bool write = true;

		while (std::getline(m_XMLInFIle, line))
		{
			if (write)
			{
				//write to temp
				m_XMLOutFIle << line << std::endl;
				if (line.find("<Properties>") != std::string::npos)
				{
					WriteAllPropertyTags();
					write = false;
				}

			}
			else
			{
				if (line.find("</Properties>") != std::string::npos)
				{
					m_XMLOutFIle << line << std::endl;
					write = true;
				}
			}
		}

		m_XMLInFIle.close();
		m_XMLOutFIle.close();

		if (!std::remove(m_InFilePath.c_str()))
		{
			if (!std::rename(m_OutFilePath.c_str(), m_InFilePath.c_str()))
			{
				std::remove(m_OutFilePath.c_str());
				return true;
			}
			else
				std::cout << "Error Renameing XML FILE " << std::endl;

		}
		else
			std::cout << "Error Removing XML FILE " << std::endl;

		return false;
	}
	else
	{
		std::cout << "Error Opening XML FILE " << std::endl;
		return false;
	}

}

void XMLWriter::CreateXML()
{
	std::ofstream newXML(m_InFilePath.c_str());

	if (newXML.is_open())
	{
		std::string xml = " <?xml version = \"1.0\" encoding=\"utf-8\"?>\n"
								"\t<PluginModule>\n"
									"\t\t<SourcePlugin Name=\"CsoundWwise\" CompanyID=\"64\" PluginID=\"3000\">\n"
										"\t\t\t<PluginInfo>\n"
											"\t\t\t\t<PlatformSupport>\n"
												"\t\t\t\t\t<Platform Name=\"Any\"></Platform>\n"
											"\t\t\t\t</PlatformSupport>\n"
										"\t\t\t</PluginInfo>\n"
									"\t\t\t<Properties>\n"
								"\t\t\t</Properties>\n"
							"\t\t</SourcePlugin>\n"
						"\t</PluginModule>";

		newXML << xml << std::endl;
		newXML.close();
	}
	else
	{
		std::cout << "Error Creating new XML" << std::endl;
	}
}
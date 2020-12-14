#pragma once

#include <fstream>
#include "Parameter.hpp"
#include <vector>

class XMLWriter
{
public:
	XMLWriter(std::vector<Parameter>& parameters, std::string inFile);

	~XMLWriter();

	void WriteUserInterFaceTag(Parameter& p);
	void WriteDefaultTag(Parameter& p);
	void WritePropertyIDTag(Parameter& p);
	void WriteRestrictionsTag(Parameter& p);

	void WritePropertyTag(Parameter& p);
	void WriteAllPropertyTags();
	bool WriteToXML();

	void CreateXML();

private:
	std::vector<Parameter> m_Parameters;

	std::ifstream m_XMLInFIle;
	std::ofstream m_XMLOutFIle;

	std::string m_InFilePath;
	std::string m_OutFilePath;
};
#pragma once
#include <string>
#include <vector>
#include "Parameter.hpp"

class CSDParser
{
public:
	CSDParser(std::string CSDPath);
	~CSDParser();

	bool Parse();
	void ProcessLine(const std::string& line);

	std::string_view GetPropertyValue(std::string_view text, std::string_view property, char startDelimiter, char endDelimiter);
	Range GetRangeFromCSVString(const std::string& CSV);

	//bool PropertiesModified();
	//void WritePropertiesHash(std::size_t propertiesHash);

	std::vector<Parameter>& GetParameters() { return m_Parameters; };
private:

	std::vector<Parameter> m_Parameters;
	std::string m_CSDPath;
};
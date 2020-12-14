#include "CSDParser.hpp"
#include <iostream>
#include <fstream>
#include <sstream>


CSDParser::CSDParser(std::string CSDPath) : m_CSDPath(CSDPath){}

CSDParser::~CSDParser() {}

bool CSDParser::Parse()
{
    std::ifstream csd(m_CSDPath);

    if (csd.is_open())
    {
        std::string line;

        while (std::getline(csd, line))
        {
            ProcessLine(line);

            //break when we reach the closing cabbage tag
            if (line.find("</Cabbage>") != std::string::npos)
                break;
        }
        return true;
    }
    else
    {
        std::cout << "Could Not Open CSD" << std::endl;
        return false;
    }
}

void CSDParser::ProcessLine(const std::string& line)
{
    std::string_view first(line.c_str(), line.find(' '));

    // check if it is a valid widget
    // if the line is a property - create property object and add to the properties class

    if (first == "checkbox" ||
        first == "button")
    {
        std::string_view name = GetPropertyValue(line, "channel", '\"', '\"');
        std::string_view val = GetPropertyValue(line, "value", '(', ')');

           Parameter p(std::string(name).data(), Range(0,1, ((int)val[0] - '0'),1,"1"), "bool");
           m_Parameters.push_back(p);
    }
    else
    {
        first.remove_prefix(1);
        if (first == "slider")
        {
            std::string_view name = GetPropertyValue(line, "channel", '\"', '\"');
            std::string_view range = GetPropertyValue(line, "range", '(', ')');

            Range r = GetRangeFromCSVString(std::string(range));

            Parameter p(std::string(name).data(), r);
            m_Parameters.push_back(p);
        }

    }


}

std::string_view CSDParser::GetPropertyValue(std::string_view text, std::string_view property, char startDelimiter, char endDelimiter)
{
    size_t startIndex = text.find(startDelimiter, text.find(property)) + 1;
    size_t EndIndex = text.find(endDelimiter, startIndex + 1);

    return  text.substr(startIndex, EndIndex - startIndex);
}

Range CSDParser::GetRangeFromCSVString(const std::string& CSV)
{
    std::stringstream ss(CSV);
    std::vector<float> result;

    float f;

    while (ss >> f)
    {
        result.push_back(f);

        if (ss.peek() == ',')
            ss.ignore();
    }

    return Range(result[0], result[1], result[2], result[3], CSV.substr(CSV.find_last_of(",") + 1));
}

#include "Parameter.hpp"


Parameter::Parameter(const std::string& name, Range range, const std::string& type) :
    m_ID(s_IDCount++),
    m_Name(name),
    m_Range(range),
    m_Type(type) {}

Parameter::~Parameter(){}


unsigned int Parameter::GetID()
{
    return m_ID;
}

char* Parameter::GetName()
{
    return &m_Name[0];
}

float Parameter::GetValue()
{
    return m_Range.m_DefaultVal;
}

Range Parameter::GetRange()
{
    return m_Range;
}

const std::string& Parameter::GetType()
{
    return m_Type;
}


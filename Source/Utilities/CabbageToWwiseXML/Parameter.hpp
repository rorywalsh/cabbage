#pragma once
#include <string>

struct Range
{
	Range(float min, float max, float defaultVal, float skew, const std::string& stepSize)
		: m_Min(min)
		, m_Max(max)
		, m_DefaultVal(defaultVal)
		, m_Skew(skew)
		, m_StepSize(stepSize) {}

	float m_Min;
	float m_Max;
	float m_DefaultVal;
	float m_Skew;
	std::string m_StepSize;

};

class Parameter
{
public:
	Parameter(const std::string& name, Range range, const std::string& type = "Real32");
	~Parameter();

	unsigned int GetID();
	char* GetName();

	float GetValue();
	Range GetRange();
	const std::string& GetType();

private:
	unsigned int m_ID;

	std::string m_Name;
	std::string m_Type;

	Range m_Range;
	
protected:
	static inline unsigned long s_IDCount = 0;
};
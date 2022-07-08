#include "Uniform.h"

#include <map>

std::map<Uniform, const char*> UniformHelper::m_TypeName;
std::map<const char*, Uniform> UniformHelper::m_NameType;

void UniformHelper::initialize()
{
	m_TypeName.insert({ Uniform::Offset, "Offset" });
	m_NameType.insert({ "Offset", Uniform::Offset });
}

const char* UniformHelper::get_name(Uniform uniform)
{
	return m_TypeName[uniform];
}

Uniform UniformHelper::get_type(const char* name)
{
	return m_NameType[name];
}

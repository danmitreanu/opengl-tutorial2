#include "Uniform.h"

#include <array>

void UniformData::initialize()
{
	uniforms = std::array<std::pair<const char*, Uniform>, (std::size_t)Uniform::Count>
	{
		{{ "Offset", Uniform::Offset }}
	};
}

UniformData& UniformHelper::get_uniforms()
{
	static UniformData data;
	data.initialize();
 
	return data;
}

const char* UniformHelper::get_name(Uniform uniform)
{
	for (const auto& pair : get_uniforms().uniforms)
	{
		if (pair.second == uniform)
			return pair.first;
	}
}

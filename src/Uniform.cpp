#include "Uniform.h"

#include <array>

struct UniformData
{
	UniformData();

    std::array<std::pair<const char*, Uniform>, (std::size_t)Uniform::Count> uniforms;
};

UniformData::UniformData()
{
	uniforms = std::array<std::pair<const char*, Uniform>, (std::size_t)Uniform::Count>
	{{
		{ "Offset", Uniform::Offset },
		{ "Model", Uniform::Model },
		{ "View", Uniform::View },
		{ "Projection", Uniform::Projection },
		{ "MVP", Uniform::MVP }
	}};
}

UniformData& UniformHelper::get_uniforms()
{
	static UniformData data;
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

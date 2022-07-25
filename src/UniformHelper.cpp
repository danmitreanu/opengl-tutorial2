#include "UniformHelper.h"

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
		{ "MVP", Uniform::MVP },
		{ "Texture0", Uniform::Texture0 },
		{ "Texture1", Uniform::Texture1 },
		{ "Texture2", Uniform::Texture2 },
		{ "Texture3", Uniform::Texture3 },
		{ "Texture4", Uniform::Texture4 }
	}};
}

UniformData& get_uniforms()
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

#include "UniformHelper.h"

#include <array>

#include "utils/magic_enum.hpp"

struct UniformData
{
    UniformData();

    std::array<std::pair<std::string, Uniform>, (std::size_t)Uniform::Count> uniforms;
};

UniformData::UniformData()
{
    magic_enum::enum_for_each<Uniform>([this] (auto val)
    {
        constexpr Uniform unif = val;
        int index = magic_enum::enum_integer(unif);
        if (index == (int)Uniform::Count || index == (int)Uniform::None)
            return;

        auto name = magic_enum::enum_name(unif);
        this->uniforms[index] = std::pair<std::string, Uniform>(name, unif);
    });
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
            return pair.first.c_str();
    }
}

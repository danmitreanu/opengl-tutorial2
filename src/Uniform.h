#pragma once

#include <map>

enum class Uniform
{
    Offset = 0
};

struct UniformHelper
{
private:
    static std::map<Uniform, const char*> m_TypeName;
    static std::map<const char*, Uniform> m_NameType;

public:
    static void initialize();
    static const char* get_name(Uniform);
    static Uniform get_type(const char*);
};
#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Texture.h"
#include "ShaderProgram.h"

template <typename T>
struct Resource
{
    std::string name;
    std::shared_ptr<T> resource;
};

class ResourceManager
{
private:
    std::vector<Resource<Texture>> m_Textures;
    std::vector<Resource<ShaderProgram>> m_ShaderPrograms;

    void load_texture(const std::string&, std::shared_ptr<Texture>&);
    void load_shader(const std::string&, std::shared_ptr<ShaderProgram>&);

public:
    ResourceManager() = default;

    std::shared_ptr<Texture> get_texture(const char* tex_file);
    std::shared_ptr<ShaderProgram> get_shader(const char* name);
};
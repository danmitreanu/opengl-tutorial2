#include "ResourceManager.h"
#include "Texture.h"

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

void ResourceManager::load_texture(const std::string& name, std::shared_ptr<Texture>& out)
{
    auto path = m_TexturePath + name;

    auto texture = std::make_shared<Texture>();
    if (!texture->load(path.c_str()))
    {
        out = nullptr;
        return;
    }

    out = texture;
}

void ResourceManager::load_shader(const std::string& name, std::shared_ptr<ShaderProgram>& out)
{
    auto path = m_ShadersPath + name;
    auto v_path = path + ".vs";
    auto f_path = path + ".fs";

    auto shader = std::make_shared<ShaderProgram>();

    if (!shader->create(v_path.c_str(), f_path.c_str()))
    {
        out = nullptr;
        return;
    }

    out = shader;
}

std::shared_ptr<Texture> ResourceManager::get_texture(const std::string& texture_name)
{
    if (m_Textures.count(texture_name))
        return m_Textures[texture_name];

    std::shared_ptr<Texture> texture;
    load_texture(texture_name, texture);

    return texture;
}

std::shared_ptr<ShaderProgram> ResourceManager::get_shader(const std::string& shader_name)
{
    if (m_ShaderPrograms.count(shader_name))
        return m_ShaderPrograms[shader_name];

    std::shared_ptr<ShaderProgram> shader;
    load_shader(shader_name, shader);

    return shader;
}

#include "ResourceManager.h"
#include "Texture.h"

#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

std::shared_ptr<ResourceManager> ResourceManager::m_Singleton;
ResourceManagerPaths ResourceManager::m_Paths;

ResourceManagerPaths::ResourceManagerPaths()
{
#if DEBUG
    current_dir = std::filesystem::current_path().parent_path();
#else
    current_dir = std::filesystem::current_path();
#endif

    textures_path = current_dir / "resources";
    shaders_path = current_dir / "shaders";
}

void ResourceManager::initialize()
{
    m_Singleton = std::make_shared<ResourceManager>();
}

ResourceManager* ResourceManager::get_instance()
{
    if (!m_Singleton)
        m_Singleton = std::make_shared<ResourceManager>();

    return m_Singleton.get();
}

void ResourceManager::load_texture(const std::string& name, std::shared_ptr<Texture>& out)
{
    auto fs_path = m_Paths.textures_path / name;
    auto path = fs_path.string();

    auto texture = std::make_shared<Texture>();
    if (!texture->load(path.c_str()))
    {
        std::cout << "ResourceManager: Could not load texture " << path << "." << std::endl;
        out = nullptr;
        return;
    }

    out = texture;
}

void ResourceManager::load_shader(const std::string& name, std::shared_ptr<ShaderProgram>& out)
{
    auto fs_path = m_Paths.shaders_path / name;
    auto path = fs_path.string();
    auto v_path = path + ".vs";
    auto f_path = path + ".fs";

    auto shader = std::make_shared<ShaderProgram>();

    if (!shader->create(v_path.c_str(), f_path.c_str()))
    {
        std::cout << "ResourceManager: Could not load shader " << path << "." << std::endl;
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

    m_Textures.insert({ texture_name, texture });

    return texture;
}

std::shared_ptr<ShaderProgram> ResourceManager::get_shader(const std::string& shader_name)
{
    if (m_ShaderPrograms.count(shader_name))
        return m_ShaderPrograms[shader_name];

    std::shared_ptr<ShaderProgram> shader;
    load_shader(shader_name, shader);

    m_ShaderPrograms.insert({ shader_name, shader });

    return shader;
}

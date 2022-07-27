#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "Texture.h"
#include "ShaderProgram.h"

struct ResourceManagerPaths
{
    std::filesystem::path current_dir;
    std::filesystem::path textures_path;
    std::filesystem::path shaders_path;

    ResourceManagerPaths();
};

class ResourceManager
{
private:
    static ResourceManagerPaths m_Paths;

    std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_ShaderPrograms;

    void load_texture(const std::string&, std::shared_ptr<Texture>&);
    void load_shader(const std::string&, std::shared_ptr<ShaderProgram>&);

public:
    ResourceManager() = default;

    std::shared_ptr<Texture> get_texture(const std::string& tex_file);
    std::shared_ptr<ShaderProgram> get_shader(const std::string& name);
};
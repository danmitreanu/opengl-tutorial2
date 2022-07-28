#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

#include "Texture.h"
#include "HeightMap.h"
#include "ShaderProgram.h"

struct ResourceManagerPaths
{
    std::filesystem::path current_dir;
    std::filesystem::path resources_path;
    std::filesystem::path shaders_path;

    ResourceManagerPaths();
};

class ResourceManager
{
private:
    static ResourceManagerPaths m_Paths;

    std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    std::unordered_map<std::string, std::shared_ptr<HeightMap>> m_HeightMaps;
    std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_ShaderPrograms;

    void load_texture(const std::string&, std::shared_ptr<Texture>&);
    void load_heightmap(const std::string&, std::shared_ptr<HeightMap>&);
    void load_shader(const std::string&, std::shared_ptr<ShaderProgram>&);

public:
    ResourceManager() = default;

    std::shared_ptr<Texture> get_texture(const std::string& tex_file);
    std::shared_ptr<HeightMap> get_heightmap(const std::string& heightmap_file);
    std::shared_ptr<ShaderProgram> get_shader(const std::string& name);
};
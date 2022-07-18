#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include "Texture.h"
#include "ShaderProgram.h"

class ResourceManager
{
private:
    static inline const std::string m_TexturePath = "/Users/danm3/opengl/cmake/resources/";
    static inline const std::string m_ShadersPath = "/Users/danm3/opengl/cmake/shaders/";

    std::map<std::string, std::shared_ptr<Texture>> m_Textures;
    std::map<std::string, std::shared_ptr<ShaderProgram>> m_ShaderPrograms;

    void load_texture(const std::string&, std::shared_ptr<Texture>&);
    void load_shader(const std::string&, std::shared_ptr<ShaderProgram>&);

public:
    ResourceManager() = default;

    std::shared_ptr<Texture> get_texture(const std::string& tex_file);
    std::shared_ptr<ShaderProgram> get_shader(const std::string& name);
};
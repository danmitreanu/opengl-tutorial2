#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "Texture.h"
#include "ShaderProgram.h"

class ResourceManager
{
private:
#ifndef _WIN32
    static inline const std::string m_TexturePath = "/Users/danm3/opengl/cmake/resources/";
    static inline const std::string m_ShadersPath = "/Users/danm3/opengl/cmake/shaders/";
#else
    static inline const std::string m_TexturePath = "X:\\opengl-tutorial2\\resources\\";
    static inline const std::string m_ShadersPath = "X:\\opengl-tutorial2\\shaders\\";
#endif

    std::unordered_map<std::string, std::shared_ptr<Texture>> m_Textures;
    std::unordered_map<std::string, std::shared_ptr<ShaderProgram>> m_ShaderPrograms;

    void load_texture(const std::string&, std::shared_ptr<Texture>&);
    void load_shader(const std::string&, std::shared_ptr<ShaderProgram>&);

public:
    ResourceManager() = default;

    std::shared_ptr<Texture> get_texture(const std::string& tex_file);
    std::shared_ptr<ShaderProgram> get_shader(const std::string& name);
};
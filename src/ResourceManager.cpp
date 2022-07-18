#include "ResourceManager.h"
#include "Texture.h"

#include <memory>
#include <string>
#include <vector>



void ResourceManager::load_texture(const std::string& name, std::shared_ptr<Texture>& out)
{
    /*jResource<Texture> r;
    r.name= filename;
    r.resource = std::make_shared<Texture>();
    r.resource->load(filename.c_str());
 
    m_Textures.push_back(r);

    out = r.resource;*/

    auto path = m_TexturePath + name;
    out = std::make_shared<Texture>();
    out->load(path.c_str());
}

void ResourceManager::load_shader(const std::string& name, std::shared_ptr<ShaderProgram>& out)
{
    /*Resource<ShaderProgram> r;
    r.name = name;
    r.resource = std::make_shared<ShaderProgram>();

    m_ShaderPrograms.push_back(r);

    out = r.resource;*/

    auto path = m_ShadersPath + name;
    auto v_path = path + ".vs";
    auto f_path = path + ".fs";

    out = std::make_shared<ShaderProgram>();
    out->create(v_path.c_str(), f_path.c_str());
}

std::shared_ptr<Texture> ResourceManager::get_texture(const std::string& texture_name)
{
    /*std::string filename = tex_file;
 
    for (const auto& r : m_Textures)
    {
        if (r.name == filename)
            return r.resource;
    }

    std::shared_ptr<Texture> texture;
    load_texture(filename, texture);

    return texture;*/

    if (m_Textures.count(texture_name))
        return m_Textures[texture_name];

    std::shared_ptr<Texture> texture;
    load_texture(texture_name, texture);

    return texture;
}

std::shared_ptr<ShaderProgram> ResourceManager::get_shader(const std::string& shader_name)
{
    /*std::string name = shader_name;

    for (const auto& r : m_ShaderPrograms)
    {
        if (r.name == name)
            return r.resource;
    }

    std::shared_ptr<ShaderProgram> shader;
    load_shader(name, shader);

    return shader;*/

    if (m_ShaderPrograms.count(shader_name))
        return m_ShaderPrograms[shader_name];

    std::shared_ptr<ShaderProgram> shader;
    load_shader(shader_name, shader);

    return shader;
}

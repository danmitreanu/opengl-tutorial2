#include "ResourceManager.h"
#include "Texture.h"

#include <memory>
#include <string>
#include <vector>

void ResourceManager::load_texture(const std::string& filename, std::shared_ptr<Texture>& out)
{
    Resource<Texture> r;
    r.name= filename;
    r.resource = std::make_shared<Texture>();
    r.resource->load(filename.c_str());
 
    m_Textures.push_back(r);

    out = r.resource;
}

void ResourceManager::load_shader(const std::string& name, std::shared_ptr<ShaderProgram>& out)
{
    Resource<ShaderProgram> r;
    r.name = name;
    r.resource = std::make_shared<ShaderProgram>();

    m_ShaderPrograms.push_back(r);

    out = r.resource;
}

std::shared_ptr<Texture> ResourceManager::get_texture(const char* tex_file)
{
    std::string filename = tex_file;
 
    for (const auto& r : m_Textures)
    {
        if (r.name == filename)
            return r.resource;
    }

    std::shared_ptr<Texture> texture;
    load_texture(filename, texture);

    return texture;
}

std::shared_ptr<ShaderProgram> ResourceManager::get_shader(const char* shader_name)
{
    std::string name = shader_name;

    for (const auto& r : m_ShaderPrograms)
    {
        if (r.name == name)
            return r.resource;
    }

    std::shared_ptr<ShaderProgram> shader;
    load_shader(name, shader);

    return shader;
}

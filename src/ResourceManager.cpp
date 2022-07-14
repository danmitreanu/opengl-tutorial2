#include "ResourceManager.h"
#include "Texture.h"

#include <memory>
#include <string>
#include <vector>

void ResourceManager::load_texture(const std::string& filename, std::shared_ptr<Texture>& out)
{
    Resource<Texture> r;
    r.filename = filename;
    r.resource = std::make_shared<Texture>();
    r.resource->load(filename.c_str());
 
    m_Textures.push_back(r);

    out = r.resource;
}

std::shared_ptr<Texture> ResourceManager::get_texture(const char* tex_file)
{
    std::string filename = tex_file;
 
    for (const auto& r : m_Textures)
    {
        if (r.filename == filename)
            return r.resource;
    }

    std::shared_ptr<Texture> texture;
    load_texture(filename, texture);

    return texture;
}
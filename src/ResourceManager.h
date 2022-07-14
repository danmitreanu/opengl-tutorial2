#pragma once

#include <memory>
#include <string>
#include <vector>

#include "Texture.h"

template <typename T>
struct Resource
{
    std::string filename;
    std::shared_ptr<T> resource;
};

class ResourceManager
{
    std::vector<Resource<Texture>> m_Textures;

    void load_texture(const std::string&, std::shared_ptr<Texture>&);

public:
    ResourceManager() = default;
    std::shared_ptr<Texture> get_texture(const char* tex_file);
};
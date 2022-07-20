#pragma once

#include <string>
#include <functional>

#include "OpenGL.h"
#include "ShaderProgram.h"

class Texture
{
private:
    GLuint m_Texture;
    uint64_t m_Hash = 0;

    static std::hash<std::string> m_HashObj;

public:
    Texture();
    ~Texture();

    bool load(const char* filename);
    void bind(int slot);

    inline uint64_t get_hash() { return m_Hash; }
};

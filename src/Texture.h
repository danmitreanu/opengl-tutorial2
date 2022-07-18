#pragma once

#include <string>

#include "OpenGL.h"
#include "ShaderProgram.h"

class Texture
{
private:
    GLuint m_Texture;

public:
    Texture();
    ~Texture();

    bool load(const char* filename);
    void bind(int slot);
};
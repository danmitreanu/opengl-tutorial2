#include "Texture.h"

#include "OpenGL.h"
#include "stb_image.h"
#include "ShaderProgram.h"

Texture::Texture()
{
    glGenTextures(1, &m_Texture);
}

void Texture::load(const char* filename)
{
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 3);

    assert((bool)data);
    assert(channels == 3);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
}

void Texture::bind(int slot)
{
    assert(slot >= 0 && slot <= 8);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

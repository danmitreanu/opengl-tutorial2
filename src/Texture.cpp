#include "Texture.h"

#include "OpenGL.h"
#include "StbWrapper.h"
#include "ShaderProgram.h"

Texture::Texture()
{
    glGenTextures(1, &m_Texture);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_Texture);
}

bool Texture::load(const char* filename)
{
    glBindTexture(GL_TEXTURE_2D, m_Texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char* data = StbWrapper::stbi_wrapper_load(filename, &width, &height, &channels, 4);

    if (!data || (channels != 3 && channels != 4))
        return false;

    GLint internal_format = channels == 3 ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    StbWrapper::stbi_wrapper_free(data);

    m_Hash = std::hash<std::string>{}(filename);

    return true;
}

void Texture::bind(int slot)
{
    //assert(slot >= 0 && slot <= 8);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}

#pragma once

#include <string>
#include <vector>
#include <OpenGL/gl3.h>

struct ShaderFile
{
    ShaderFile() : type(GL_VERTEX_SHADER) {}
    const char* name;
    GLenum type;
};

class Shaders
{
private:
    std::vector<ShaderFile> m_Files;
    GLuint m_ShaderProgram;
    std::vector<GLuint> m_ShaderObjects;

    void add_shader(GLuint, const char*, GLenum);
    void compile_shaders();
    void attach_shaders();
    void link_program();

    static bool read_file(const char*, std::string&);

public:
    Shaders() = default;

    void add_file(ShaderFile);
    void use_shaders();
};

#pragma once

#include <string>
#include <array>
#include <OpenGL/gl3.h>

#include "Math3D.h"
#include "Uniform.h"

class Shaders
{
private:
    GLuint m_ShaderProgram;

    std::string m_VertexFile;
    std::string m_FragmentFile;
    GLuint m_VertexObject;
    GLuint m_FragmentObject;

    void init_program();

    // Makes sure the shader program is bound
    void ensure_bound();

    static bool read_file(const std::string&, std::string&);
    static GLuint create_shader_object(const std::string&, GLenum);
    static void link_shader_program(GLuint program);

public:
    Shaders() = default;

    // Create with vertex shader and frag shader files
    void create(const char*, const char*);
 
    void bind();

    void set_uniform(Uniform, float);
    void set_uniform(Uniform, Vector2f);
};

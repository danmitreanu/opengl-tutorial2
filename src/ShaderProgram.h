#pragma once

#include <string>
#include <array>
#include "OpenGL.h"

#include "Math3D.h"
#include "UniformHelper.h"

class ShaderProgram
{
private:
    GLuint m_ShaderProgram;

    std::string m_VertexFile;
    std::string m_FragmentFile;
    GLuint m_VertexObject;
    GLuint m_FragmentObject;

    std::array<GLuint, (std::size_t)Uniform::Count> m_Uniforms;

    void init_program();
    void init_uniforms();
    void init_attributes();

    std::size_t get_attribute_count();

    // Makes sure the shader program is bound
    void ensure_bound();

    static bool read_file(const std::string&, std::string&);
    static GLuint create_shader_object(const std::string&, GLenum);
    static void link_shader_program(GLuint program);

public:
    ShaderProgram() = default;

    inline GLuint get_program_id() const { return m_ShaderProgram; }

    // Create with vertex shader and frag shader files.
    void create(const char*, const char*);
 
    void bind() const;

    GLint get_uniform_pos(Uniform);
    void set_uniform(Uniform, int);
    void set_uniform(Uniform, float);
    void set_uniform(Uniform, const Vector2f&);
    void set_uniform(Uniform, const Matrix4f&);
};

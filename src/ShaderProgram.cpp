#include <iostream>
#include <sstream>
#include <fstream>

#include "Math3D.h"
#include "ShaderProgram.h"

void ShaderProgram::init_program()
{
    m_ShaderProgram = glCreateProgram();

    if (m_ShaderProgram == 0)
    {
        std::cout << "Could not create shader program." << std::endl;
        return;
    }
    //return;
    std::string vertex_text;
    std::string frag_text;

    if (!read_file(m_VertexFile, vertex_text) || !read_file(m_FragmentFile, frag_text))
    {
        std::cout << "Could not read one or more shader files." << std::endl;
        return;
    }

    m_VertexObject = create_shader_object(vertex_text, GL_VERTEX_SHADER);
    m_FragmentObject = create_shader_object(frag_text, GL_FRAGMENT_SHADER);

    glAttachShader(m_ShaderProgram, m_VertexObject);
    glAttachShader(m_ShaderProgram, m_FragmentObject);

    link_shader_program(m_ShaderProgram);
}

GLuint ShaderProgram::create_shader_object(const std::string& text, GLenum shader_type)
{
    GLuint shader_object = glCreateShader(shader_type);

    if (shader_object == 0)
    {
        std::cout << "Could not create shader object." << std::endl;
        return 0;
    }
    
    const GLchar* shader_texts[1];
    shader_texts[0] = text.c_str();

    GLint shader_lengths[1];
    shader_lengths[0] = (GLint)text.size();

    glShaderSource(shader_object, 1, shader_texts, shader_lengths);
    glCompileShader(shader_object);

    GLint success;
    glGetShaderiv(shader_object, GL_COMPILE_STATUS, &success);

    if (success == 0)
    {
        GLchar error_log[1024];
        glGetShaderInfoLog(shader_object, sizeof(error_log), NULL, error_log);
        std::cout << "Could not compile shader object." << std::endl;
    }

    return shader_object;
}

void ShaderProgram::link_shader_program(GLuint program)
{
    GLint success;
    GLchar error_log[1024];

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(program, sizeof(error_log), NULL, error_log);
        std::cout << "Could not link shader program." << std::endl;
        std::cout << error_log << std::endl;
        return;
    }

    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(program, sizeof(error_log), NULL, error_log);
        std::cout << "Could not validate program." << std::endl;
        std::cout << error_log << std::endl;
        return;
    }
}

bool ShaderProgram::read_file(const std::string& filename, std::string& out)
{
    std::ifstream read(filename);
    if (!read)
    {
        std::cout << "Could not open file " << filename << std::endl;
        return false;
    }

    std::stringstream ss;
    ss << read.rdbuf();

    out = ss.str();

    return true;
}

void ShaderProgram::create(const char* vertex_shader_file, const char* frag_shader_file)
{
    m_VertexFile = vertex_shader_file;
    m_FragmentFile = frag_shader_file;

    init_program();
}

void ShaderProgram::bind()
{
    glUseProgram(m_ShaderProgram);
}

void ShaderProgram::ensure_bound()
{
/*  GLuint  */
    GLint bound_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &bound_program);

    if ((GLuint)bound_program != m_ShaderProgram)
        this->bind();
}

GLint ShaderProgram::get_uniform_pos(Uniform uniform)
{
    const char* name = UniformHelper::get_name(uniform);
    return glGetUniformLocation(m_ShaderProgram, name);
}

void ShaderProgram::set_uniform(Uniform uniform, float value)
{
    this->ensure_bound();
    GLint pos = get_uniform_pos(uniform);
    glUniform1f(pos, value);
}

void ShaderProgram::set_uniform(Uniform uniform, Vector2f vec2f)
{
    this->ensure_bound();
    GLint pos = get_uniform_pos(uniform);
    glUniform2f(pos, vec2f.x, vec2f.y);
}

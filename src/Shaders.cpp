#include <iostream>
#include <sstream>
#include <fstream>

#include "Shaders.h"

ShaderFile::ShaderFile(const char* name, GLenum type)
{
    this->name = name;
    this->type = type;
}

void Shaders::compile_shaders()
{
    m_ShaderProgram = glCreateProgram();

    if (m_ShaderProgram == 0)
    {
        std::cout << "Could not create shader program." << std::endl;
        return;
    }

    for (auto& shader_file : m_Files)
    {
        std::string shader_text;
        if (!this->read_file(shader_file.name, shader_text))
        {
            std::cout << "Could not read shader file." << std::endl;
            continue;
        }

        this->add_shader(m_ShaderProgram, shader_text.c_str(), shader_file.type);
    }
}

void Shaders::add_shader(GLuint program, const char* text, GLenum shader_type)
{
    GLuint shader_object = glCreateShader(shader_type);

    if (shader_object == 0)
    {
        std::cout << "Could not create shader object." << std::endl;
        return;
    }

    const GLchar* shader_texts[1];
    shader_texts[0] = text;

    GLint shader_lengths[1];
    shader_lengths[0] = (GLint)strlen(text);

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

    m_ShaderObjects.push_back(shader_object);
}

void Shaders::attach_shaders()
{
    for (auto& obj : m_ShaderObjects)
        glAttachShader(m_ShaderProgram, obj);
}

void Shaders::link_program()
{
    GLint success;
    GLchar error_log[1024];

    glLinkProgram(m_ShaderProgram);
    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(m_ShaderProgram, sizeof(error_log), NULL, error_log);
        std::cout << "Could not link shader program." << std::endl;
        std::cout << error_log << std::endl;
        return;
    }

    glValidateProgram(m_ShaderProgram);
    glGetProgramiv(m_ShaderProgram, GL_VALIDATE_STATUS, &success);
    if (success == 0)
    {
        glGetProgramInfoLog(m_ShaderProgram, sizeof(error_log), NULL, error_log);
        std::cout << "Could not validate program." << std::endl;
        std::cout << error_log << std::endl;
        return;
    }

   
}

bool Shaders::read_file(const char* filename, std::string& out)
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

void Shaders::create_shaders()
{
    this->compile_shaders();
    this->attach_shaders();
    this->link_program();
}

void Shaders::bind()
{
    glUseProgram(m_ShaderProgram);
}

void Shaders::add_file(ShaderFile file)
{
    m_Files.push_back(file);
}

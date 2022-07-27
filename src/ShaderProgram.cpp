#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>

#include "Math3D.h"
#include "AttributeHelper.h"
#include "ShaderProgram.h"

std::hash<std::string> ShaderProgram::m_HashObj;

bool BlendingState::equals(const BlendingState& cmp) const
{
    return
        enabled == cmp.enabled &&
        source_func == cmp.source_func &&
        dest_func == cmp.dest_func;
}

bool BlendingState::operator==(const BlendingState& other) const
{
    return this->equals(other);
}

bool BlendingState::operator!=(const BlendingState& other) const
{
    return !this->equals(other);
}

bool BlendingState::operator<(const BlendingState& other) const
{
    if (enabled != other.enabled)
        return enabled < other.enabled;

    if (source_func != other.source_func)
        return source_func < other.source_func;

    return dest_func < other.dest_func;
}

bool ShaderProgram::init_program()
{
    m_ShaderProgram = glCreateProgram();

    if (m_ShaderProgram == 0)
    {
        std::cout << "Could not create shader program." << std::endl;
        return false;
    }
    //return;
    std::string vertex_text;
    std::string frag_text;

    if (!read_file(m_VertexFile, vertex_text) || !read_file(m_FragmentFile, frag_text))
    {
        std::cout << "Could not read one or more shader files." << std::endl;
        return false;
    }

    m_VertexObject = create_shader_object(vertex_text, GL_VERTEX_SHADER);
    m_FragmentObject = create_shader_object(frag_text, GL_FRAGMENT_SHADER);

    glAttachShader(m_ShaderProgram, m_VertexObject);
    glAttachShader(m_ShaderProgram, m_FragmentObject);

    
    init_attributes();
    link_shader_program(m_ShaderProgram);

    return true;
}

void ShaderProgram::init_uniforms()
{
    auto count = (std::size_t)Uniform::Count;

    for (std::size_t i = 0; i < count; i++)
    {
        const char* name = UniformHelper::get_name((Uniform)i);
        int pos = glGetUniformLocation(m_ShaderProgram, name);
        if (pos == -1)
        {
            m_Uniforms[i] = -1;
            continue;
        }

        m_Uniforms[i] = (GLuint)pos;
    }
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
}

bool ShaderProgram::read_file(const std::string& filename, std::string& out)
{
    std::ifstream read(filename);
    if (!read)
    {
        std::cout << "Could not open file " << filename << "." << std::endl;
        return false;
    }

    std::stringstream ss;
    ss << read.rdbuf();

    out = ss.str();

    return true;
}

bool ShaderProgram::create(const char* vertex_shader_file, const char* frag_shader_file)
{
    m_VertexFile = vertex_shader_file;
    m_FragmentFile = frag_shader_file;

    auto hash_str = std::string(m_VertexFile) + m_FragmentFile;
    m_Hash = m_HashObj(hash_str);

    if (!init_program())
        return false;

    init_uniforms();

    return true;
}

void ShaderProgram::bind() const
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

void ShaderProgram::set_uniform(Uniform uniform, int value)
{
    glUniform1i(m_Uniforms[(std::size_t)uniform], value);
}

void ShaderProgram::set_uniform(Uniform uniform, float value)
{
    glUniform1f(m_Uniforms[(std::size_t)uniform], value);
}

void ShaderProgram::set_uniform(Uniform uniform, const Vector2f& vec2f)
{
    glUniform2f(m_Uniforms[(std::size_t)uniform], vec2f.x, vec2f.y);
}

void ShaderProgram::set_uniform(Uniform uniform, const Vector3f& vec3f)
{
    glUniform3f(m_Uniforms[(std::size_t)uniform], vec3f.x, vec3f.y, vec3f.z);
}

void ShaderProgram::set_uniform(Uniform uniform, const Matrix3f& matrix3)
{
    glUniformMatrix3fv(m_Uniforms[(std::size_t)uniform], 1, GL_TRUE, &matrix3.m[0][0]);
}

void ShaderProgram::set_uniform(Uniform uniform, const Matrix4f& matrix4)
{
    glUniformMatrix4fv(m_Uniforms[(std::size_t)uniform], 1, GL_TRUE, &matrix4.m[0][0]);
}

std::size_t ShaderProgram::get_attribute_count()
{
    GLint count;
    glGetProgramiv(m_ShaderProgram, GL_ACTIVE_ATTRIBUTES, &count);
    return (std::size_t)count;
}

void ShaderProgram::init_attributes()
{
    const std::size_t namebuf_size = 16;
    GLchar namebuf[namebuf_size];
    GLint size;
    GLenum type;
    GLsizei length;

    //const std::size_t count = get_attribute_count();

    for (std::size_t i = 0; i < (std::size_t)AttributeType::Count; i++)
    {
       //glGetActiveAttrib(m_ShaderProgram, i, namebuf_size, &length, &size, &type, namebuf);

        //std::size_t type_index = (std::size_t)AttributeHelper::get_type(namebuf);

        //assert(type_index != (std::size_t)AttributeType::Count);

        glBindAttribLocation(m_ShaderProgram, int(i), AttributeHelper::get_name((AttributeType)i));
    }
}

bool ShaderProgram::operator<(const ShaderProgram& other)
{
    if (m_BlendingState != other.m_BlendingState)
        return m_BlendingState < other.m_BlendingState;

    return m_Hash < other.m_Hash;
}
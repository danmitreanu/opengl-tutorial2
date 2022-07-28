#pragma once

#include <string>
#include <array>
#include <functional>
#include "OpenGL.h"

#include "Math3D.h"
#include "UniformHelper.h"

enum class BlendingFunc
{
    NONE = 0,

    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA
};

struct BlendingState
{
    bool enabled = false;

    BlendingFunc source_func = BlendingFunc::NONE;
    BlendingFunc dest_func = BlendingFunc::NONE;

    bool equals(const BlendingState&) const;

    bool operator==(const BlendingState&) const;
    bool operator!=(const BlendingState&) const;
    bool operator<(const BlendingState&) const;
};

class ShaderProgram
{
private:
    GLuint m_ShaderProgram;
    uint64_t m_Hash = 0;

    BlendingState m_BlendingState;

    std::array<GLuint, (std::size_t)Uniform::Count> m_Uniforms;

    bool init_program(const char*, const char*);
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
    ~ShaderProgram();

    // Create with vertex shader and frag shader files.
    bool create(const char*, const char*);
 
    void bind() const;

    inline void set_blending_state(BlendingState& blend) { m_BlendingState = blend; }
    inline const BlendingState& get_blending_state() { return m_BlendingState; }

    GLint get_uniform_pos(Uniform);
    void set_uniform(Uniform, int);
    void set_uniform(Uniform, float);
    void set_uniform(Uniform, const Vector2f&);
    void set_uniform(Uniform, const Vector3f&);
    void set_uniform(Uniform, const Matrix3f&);
    void set_uniform(Uniform, const Matrix4f&);

    bool operator<(const ShaderProgram&);
};

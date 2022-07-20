#pragma once

#include "ShaderProgram.h"
#include "UniformHelper.h"
#include "Math3D.h"

struct IUniformNode
{
    IUniformNode* next = nullptr;

    Uniform uniform;
    virtual void set_uniform(ShaderProgram*) = 0;
};

struct UniformIntegerNode : public IUniformNode
{
    int value;
    void set_uniform(ShaderProgram*) override;
};

struct UniformFloatNode : public IUniformNode
{
    float value;
    void set_uniform(ShaderProgram*) override;
};

struct UniformVector2fNode : public IUniformNode
{
    Vector2f value;
    void set_uniform(ShaderProgram*) override;
};

struct UniformVector3fNode : public IUniformNode
{
    Vector3f value;
    void set_uniform(ShaderProgram*) override;
};

struct UniformMatrix3fNode : public IUniformNode
{
    Matrix3f value;
    void set_uniform(ShaderProgram*) override;
};

struct UniformMatrix4fNode : public IUniformNode
{
    Matrix4f value;
    void set_uniform(ShaderProgram*) override;
};
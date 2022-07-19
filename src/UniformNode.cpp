#pragma once

#include "UniformNode.h"
#include "ShaderProgram.h"

void UniformIntegerNode::set_uniform(ShaderProgram* shader)
{
    shader->set_uniform(uniform, value);
}

void UniformFloatNode::set_uniform(ShaderProgram* shader)
{
    shader->set_uniform(uniform, value);
}

void UniformVector2fNode::set_uniform(ShaderProgram* shader)
{
    shader->set_uniform(uniform, value);
}

void UniformVector3fNode::set_uniform(ShaderProgram* shader)
{
    shader->set_uniform(uniform, value);
}

void UniformMatrix3fNode::set_uniform(ShaderProgram* shader)
{
    shader->set_uniform(uniform, value);
}

void UniformMatrix4fNode::set_uniform(ShaderProgram* shader)
{
    shader->set_uniform(uniform, value);
}
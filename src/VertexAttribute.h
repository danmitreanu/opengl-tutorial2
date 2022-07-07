#pragma once

#include <iostream>

#include "AttributeHelper.h"

struct VertexAttribute
{
    AttributeType type;
    std::size_t number_of_floats = 0;
};
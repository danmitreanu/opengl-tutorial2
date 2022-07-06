#pragma once

#include <iostream>

struct VertexAttribute
{
    VertexAttribute() : number_of_floats(0) {}

    const char* name;
    std::size_t number_of_floats;
};
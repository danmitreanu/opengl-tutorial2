#pragma once

#include <iostream>
#include "UniformNode.h"
#include "TextureNode.h"

template <typename T>
class Pool
{
private:
    std::size_t m_SizeUnit = 100;
    std::size_t m_Reserved = 0;
    std::size_t m_Index = 0;

    T* m_Array;

    void extend(std::size_t magnitude = 1);

public:
    Pool(std::size_t = 100);
    ~Pool();

    void reset();
    T* allocate(std::size_t count);
    inline T* allocate_one() { return allocate(1); }
};
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

    void extend(std::size_t magnitude = 1)
    {
        std::size_t extend_count = magnitude * m_SizeUnit;

        T* temp = new T[m_Reserved + extend_count];
        for (std::size_t i = 0; i < m_Reserved; i++)
            temp[i] = m_Array[i];

        m_Reserved += extend_count;
        delete [] m_Array;
        m_Array = temp;
    }

public:
    Pool(std::size_t initial_size = 100)
    {
        m_SizeUnit = initial_size;
        m_Reserved = initial_size;

        m_Array = new T[m_Reserved];
    }

    ~Pool()
    {
        delete [] m_Array;
    }

    void reset()
    {
        m_Index = 0;
    }

    T* add(T& item)
    {
        std::size_t needed_size = m_Index + 1;
        if (needed_size >= m_Reserved)
            extend();

        m_Array[m_Index] = item;
        return m_Array + m_Index++;
    }
};

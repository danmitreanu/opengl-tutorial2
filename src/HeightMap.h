#pragma once

#include "StbWrapper.h"
#include "Math3D.h"

#include <iostream>
#include <array>

class HeightMap
{
private:
    static const std::size_t m_TexWidth = 2048;
    static const std::size_t m_TexHeight = 2048;

    std::array<float, m_TexWidth * m_TexHeight> m_Height = { 0 };

    void read_heights(unsigned char*, const short);

public:
    HeightMap() = default;

    bool load(const char*);

    inline const std::size_t get_tex_width() { return m_TexWidth; }
    inline const std::size_t get_tex_height() { return m_TexHeight; }

    float get_height(std::size_t x, std::size_t y);
};

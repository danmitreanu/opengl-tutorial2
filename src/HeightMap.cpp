#include "HeightMap.h"
#include "StbWrapper.h"

#include <iostream>

bool HeightMap::load(const char* filename)
{
    int width, height, channels;
    unsigned char* data = StbWrapper::stbi_wrapper_load(filename, &width, &height, &channels, 3);

    if (!data)
        return false;

    read_heights(data, channels);

    StbWrapper::stbi_wrapper_free(data);

    return true;
}

void HeightMap::read_heights(unsigned char* data, const short bytes_per_pixel)
{
    for (std::size_t i = 0; i < m_TexWidth; i++)
    for (std::size_t j = 0; j < m_TexHeight; j++)
    {
        const unsigned char* offset = data + (i + m_TexWidth * j) * bytes_per_pixel;
        unsigned char red = offset[0];

        m_Height[i][j] = 1.0f - red / 255.0f; // white is full height
    }
}

float HeightMap::get_height(std::size_t x, std::size_t y)
{
    assert(x < m_TexWidth && y < m_TexHeight);
    return m_Height[x][y];
}

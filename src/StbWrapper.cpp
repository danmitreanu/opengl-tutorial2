#include "StbWrapper.h"
#include "utils/stb_image.h"

unsigned char* StbWrapper::stbi_wrapper_load(const char* filename,
    int* width,
    int* height,
    int* channels,
    int desired_channels)
{
    return stbi_load(filename, width, height, channels, desired_channels);
}

void StbWrapper::stbi_wrapper_free(void* data)
{
    stbi_image_free(data);
}

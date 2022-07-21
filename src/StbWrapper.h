#pragma once

struct StbWrapper
{
    static unsigned char* stbi_wrapper_load(const char*, int*, int*, int*, int);
    static void stbi_wrapper_free(void*);
};

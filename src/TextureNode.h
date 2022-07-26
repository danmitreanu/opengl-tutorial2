#pragma once

#include "Texture.h"

struct TextureNode
{
    TextureNode* next = nullptr;
    Texture* value;
    Uniform  target;
    uint64_t hash = 0;

    uint32_t    slot;
};

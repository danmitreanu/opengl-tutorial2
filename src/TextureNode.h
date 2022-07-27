#pragma once

#include "Texture.h"

struct TextureNode
{
    TextureNode* next = nullptr;
    Texture* value = nullptr;
    Uniform  target = Uniform::None;
    uint64_t hash = 0;

    uint8_t slot;
};

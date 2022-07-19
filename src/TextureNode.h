#pragma once

#include "Texture.h"

struct TextureNode
{
    TextureNode* next = nullptr;

    Texture* value;
};

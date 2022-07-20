#pragma once

#include "Texture.h"

struct TextureNode
{
    TextureNode* next = nullptr;
    Texture* value;
    uint64_t hash = 0;

    void xor_hash(uint64_t);
    void set_value(Texture*);
    void add_texture_node(TextureNode*);
};

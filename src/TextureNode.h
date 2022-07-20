#pragma once

#include "Texture.h"

struct TextureNode
{
    TextureNode* next = nullptr;
    Texture* value;
    Uniform  target;
    uint64_t hash = 0;
    
    uint32_t    slot;
    

    void xor_hash(uint64_t);
    void set_value(Texture*);
    void add_texture_node(TextureNode*);
};

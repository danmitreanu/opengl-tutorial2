#include "TextureNode.h"
#include "Texture.h"

void TextureNode::xor_hash(uint64_t val)
{
    hash ^= val;
}

void TextureNode::set_value(Texture* tex)
{
    hash ^= tex->get_hash();

    value = tex;
}

void TextureNode::add_texture_node(TextureNode* add_node)
{
    auto add_hash = add_node->hash;

    auto node = this;
    while (node->next != nullptr)
    {
        node->xor_hash(add_hash);
        node = node->next;
    }

    node->next = add_node;
}

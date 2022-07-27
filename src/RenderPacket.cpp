#include "RenderPacket.h"

bool RenderPacket::compare(const RenderPacket& other) const
{
    if (priority != other.priority)
        return priority > other.priority;

    if (shader != other.shader)
        return *shader < *(other.shader);

    if (textures != other.textures)
    {
        uint64_t ha = textures != nullptr ? textures->hash : 0;
        uint64_t hb = other.textures != nullptr ? other.textures->hash : 0;
        return ha > hb;
    }

    if (vbo != other.vbo)
        return vbo < other.vbo;

    if (ibo != other.ibo)
        return ibo < other.ibo;

    return primitive_start < other.primitive_start;
}

bool RenderPacket::operator<(const RenderPacket& other) const
{
    return compare(other);
}

#include "RenderPacket.h"

bool BlendingState::equals(const BlendingState& cmp) const
{
    return
        enabled == cmp.enabled &&
        source_func == cmp.source_func &&
        dest_func == cmp.dest_func;
}

bool BlendingState::operator==(const BlendingState& other) const
{
    return this->equals(other);
}

bool BlendingState::operator!=(const BlendingState& other) const
{
    return !this->equals(other);
}

bool BlendingState::operator<(const BlendingState& other) const
{
    if (enabled != other.enabled)
        return enabled < other.enabled;

    if (source_func != other.source_func)
        return source_func < other.source_func;

    return dest_func < other.dest_func;
}

bool RenderPacket::compare(const RenderPacket& other) const
{
    if (priority != other.priority)
        return priority > other.priority;

    if (shader != other.shader)
        return shader < other.shader;

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

    if (blend != other.blend)
        return blend < other.blend;

    return primitive_start < other.primitive_start;
}

bool RenderPacket::operator<(const RenderPacket& other) const
{
    return compare(other);
}

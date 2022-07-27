#include "RenderPacket.h"

bool BlendingState::equals(const BlendingState& cmp) const
{
    return
        enabled == cmp.enabled &&
        source_func == cmp.source_func &&
        dest_func == cmp.dest_func;
}

bool operator==(BlendingState const& state1, BlendingState const& state2)
{
    return state1.equals(state2);
}

bool operator!=(BlendingState const& state1, BlendingState const& state2)
{
    return !state1.equals(state2);
}

bool operator<(BlendingState const& state1, BlendingState const& state2)
{
    if (state1.enabled != state2.enabled)
        return state1.enabled < state2.enabled;

    if (state1.source_func != state2.source_func)
        return state1.source_func < state2.source_func;

    return state1.dest_func < state2.dest_func;
}

bool RenderPacket::compare(RenderPacket const& packet1, RenderPacket const& packet2)
{
    if (packet1.priority != packet2.priority)
        return packet1.priority > packet2.priority;

    if (packet1.blend!= packet2.blend)
        return packet1.blend < packet2.blend;

    if (packet1.shader != packet2.shader)
        return packet1.shader < packet2.shader;

    if (packet1.textures != packet2.textures)
    {
        uint64_t ha = packet1.textures != nullptr ? packet1.textures->hash : 0;
        uint64_t hb = packet2.textures != nullptr ? packet2.textures->hash : 0;
        return ha > hb;
    }

    if (packet1.vbo != packet2.vbo)
        return packet1.vbo < packet2.vbo;

    if (packet1.ibo != packet2.ibo)
        return packet1.ibo < packet2.ibo;

    return packet1.textures < packet2.textures;
}

bool operator<(RenderPacket const& packet1, RenderPacket const& packet2)
{
    return RenderPacket::compare(packet1, packet2);
}

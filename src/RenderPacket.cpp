#include "RenderPacket.h"

bool BlendingState::equals(const BlendingState& cmp) const
{
    return
        enabled == cmp.enabled &&
        source_func == cmp.source_func &&
        dest_func == cmp.dest_func;
}

bool RenderPacket::compare(RenderPacket const& packet1, RenderPacket const& packet2)
{
    if (packet1.priority != packet2.priority)
        return packet1.priority > packet2.priority;

    if (packet1.blend.enabled != packet2.blend.enabled)
        return !packet1.blend.enabled;

    if (packet1.vbo != packet2.vbo)
        return packet1.vbo < packet2.vbo;

    return packet1.textures < packet2.textures;
}

bool operator==(BlendingState const& state1, BlendingState const& state2)
{
    return state1.equals(state2);
}

bool operator!=(BlendingState const& state1, BlendingState const& state2)
{
    return !state1.equals(state2);
}

bool operator<(RenderPacket const& packet1, RenderPacket const& packet2)
{
    return RenderPacket::compare(packet1, packet2);
}

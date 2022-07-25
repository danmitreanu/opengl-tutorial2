#include "RenderPacket.h"

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

bool operator<(RenderPacket const& packet1, RenderPacket const& packet2)
{
    return RenderPacket::compare(packet1, packet2);
}

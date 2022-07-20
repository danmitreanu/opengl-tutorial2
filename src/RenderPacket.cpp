#include "RenderPacket.h"

bool operator<(RenderPacket const& packet1, RenderPacket const& packet2)
{
    if (packet1.vbo < packet2.vbo)
        return true;

    return packet1.textures < packet2.textures;
}
#pragma once
#include <windows.h>
#include <Xinput.h>
#include <cstdint>
#include <cstring>

#pragma pack(push,1)
struct LabInputPacket {
    std::uint32_t magic;
    std::uint16_t version, size;
    std::uint32_t sequence;
    std::uint64_t expiresFileTime;
    std::uint16_t buttons;
    std::uint8_t leftTrigger, rightTrigger;
    std::int16_t leftX, leftY, rightX, rightY;
    std::uint64_t reserved;
};
#pragma pack(pop)
static_assert(sizeof(LabInputPacket)==40);
inline bool decodeLabInput(const LabInputPacket& packet, std::uint64_t now, XINPUT_GAMEPAD& pad) {
    pad = {};
    if (packet.magic != 0x49433244 || packet.version != 1 || packet.size != sizeof(packet) ||
        packet.reserved || !packet.sequence || packet.expiresFileTime <= now ||
        packet.expiresFileTime-now > 600000000 || (packet.buttons & ~0xf3ff)) return false;
    pad.wButtons=packet.buttons;
    pad.bLeftTrigger=packet.leftTrigger; pad.bRightTrigger=packet.rightTrigger;
    pad.sThumbLX=packet.leftX; pad.sThumbLY=packet.leftY;
    pad.sThumbRX=packet.rightX; pad.sThumbRY=packet.rightY;
    return true;
}

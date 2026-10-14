#pragma once
#include <cstdint>
#include <cstring>
#include <windows.h>

// In-memory patches for the inspected executable. No desktop-control worker.
// The renderer continues to create and move its own native window.
inline bool installLabPolicy(unsigned char* image) {
    const auto* dos = reinterpret_cast<IMAGE_DOS_HEADER*>(image);
    const auto* nt = reinterpret_cast<IMAGE_NT_HEADERS64*>(image + dos->e_lfanew);
    if (nt->FileHeader.TimeDateStamp != 0x67a50cde || nt->OptionalHeader.SizeOfImage != 68988928) return false;
    struct Patch { std::uint32_t rva; unsigned char expected[40]; unsigned char replacement[40]; SIZE_T size; };
    Patch patches[] = {
        // Native joystick input is permitted without window focus. Change this
        // cvar's own constructor default; shared literal strings stay untouched.
        {0x135322b,{0x4c,0x8d,0x05,0x3e,0xb7,0x76,0x00},{0x4c,0x8d,0x05,0x86,0xbd,0x76,0x00},7},
        // Do not open the pause menu when WM_KILLFOCUS is received.
        {0xea353, {0x74,0x4e}, {0xeb,0x4e}, 2},
        // Native resolution selection may update dimensions, but preserve the
        // configured window coordinates instead of writing centered values.
        {0x34594d,{0xe8,0x8e,0xcf,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x34596b,{0xe8,0x70,0xcf,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x34597c,{0xe8,0x5f,0xcf,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x348cc1,{0xe8,0x1a,0x9c,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x348cdc,{0xe8,0xff,0x9b,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x348db1,{0xe8,0x2a,0x9b,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x348dcd,{0xe8,0x0e,0x9b,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x348dee,{0xe8,0xed,0x9a,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x348dfa,{0xe8,0xe1,0x9a,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
        {0x348e0b,{0xe8,0xd0,0x9a,0xdb,0xff},{0x90,0x90,0x90,0x90,0x90},5},
    };
    for (const auto& patch : patches) if (std::memcmp(image + patch.rva, patch.expected, patch.size)) return false;
    // Exact renderer centering sequence: replace with reads of the native X/Y cvars.
    const unsigned char centered[] = {
        0x41,0x8b,0x40,0x08,0x2b,0xc3,0x99,0x2b,0xc2,0xd1,0xf8,0x8b,0xf0,
        0x41,0x8b,0x40,0x0c,0x41,0x03,0x70,0x10,0x2b,0xc7,0x99,0x2b,0xc2,
        0x8b,0xd6,0xd1,0xf8,0x44,0x8b,0xf0,0x45,0x03,0x70,0x14};
    constexpr std::uint32_t positionRva = 0x347814;
    if (std::memcmp(image + positionRva, centered, sizeof(centered))) return false;
    unsigned char configured[sizeof(centered)];
    std::memset(configured, 0x90, sizeof(configured));
    configured[0]=0x8b; configured[1]=0x35; // mov esi,[rip+r_windowPosX.integer]
    const std::int32_t xDisplacement = 0x32fb378 - (positionRva + 6);
    std::memcpy(configured+2,&xDisplacement,4);
    configured[6]=0x44; configured[7]=0x8b; configured[8]=0x35;
    const std::int32_t yDisplacement = 0x32fb3f8 - (positionRva + 13);
    std::memcpy(configured+9,&yDisplacement,4);
    configured[13]=0x8b; configured[14]=0xd6; // mov edx,esi
    auto apply = [&](std::uint32_t rva, const unsigned char* bytes, SIZE_T size) {
        DWORD old=0, unused=0;
        if (!VirtualProtect(image+rva,size,PAGE_EXECUTE_READWRITE,&old)) return false;
        std::memcpy(image+rva,bytes,size);
        FlushInstructionCache(GetCurrentProcess(),image+rva,size);
        return VirtualProtect(image+rva,size,old,&unused) != FALSE;
    };
    for (const auto& patch : patches) if (!apply(patch.rva,patch.replacement,patch.size)) return false;
    return apply(positionRva,configured,sizeof(configured));
}

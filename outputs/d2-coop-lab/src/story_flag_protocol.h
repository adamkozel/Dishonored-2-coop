#pragma once
#include <cstdint>
#include <string>
// 1214kj (#220): D2SF, the host's bool story flags by name hash (story_flag_mirror.h).
constexpr std::uint32_t kStoryFlagMagic=0x46533244; // "D2SF"
constexpr unsigned kStoryFlagMax=256;
#pragma pack(push,1)
struct StoryFlagEntry {std::uint32_t decl=0,name=0;std::uint8_t value=0;};
struct StoryFlagPacket {
    std::uint32_t magic=kStoryFlagMagic;std::uint16_t version=1,size=sizeof(StoryFlagPacket);
    std::uint32_t role=0,count=0;std::uint64_t session=0,sequence=0;
    StoryFlagEntry entries[kStoryFlagMax]{};
};
#pragma pack(pop)
static_assert(sizeof(StoryFlagPacket)==32+9*kStoryFlagMax);
inline std::uint32_t storyFlagHash(const std::string& s){std::uint32_t h=2166136261u;for(unsigned char c:s){h^=c;h*=16777619u;}return h;}
// Only the host sends; every used entry is a bool with a name, and the unused tail is zero.
inline bool validStoryFlags(const StoryFlagPacket& p){
    if(p.magic!=kStoryFlagMagic||p.version!=1||p.size!=sizeof(p)||p.role!=0||p.count>kStoryFlagMax||!p.sequence)return false;
    for(unsigned i=0;i<kStoryFlagMax;++i){const auto& e=p.entries[i];
        if(i<p.count?(e.value>1||(!e.decl&&!e.name)):(e.decl||e.name||e.value))return false;}
    return true;
}

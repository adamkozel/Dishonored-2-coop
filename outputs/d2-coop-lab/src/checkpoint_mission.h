#pragma once
#include <bcrypt.h>
// 1147 mission checkpoints. On a mission map each peer restarts from its OWN
// last native autosave (per-character worlds), through the game's own
// RestartFromCheckpoint -> RestartFromMemoryCheckpoint path (1146, confirmed
// runtime). The shared identity is not a file hash: Corvo and Emily saves
// differ by character, time and checksum. It is the autosave's authored
// mapName/chapter/mission of the current map (1147b), so both peers must hold
// a checkpoint of the same mission.
struct CheckpointMissionScope {
    bool mission=false;
    std::uint64_t mapId=0;
    char map[192]{};
    int seenSlot=-1,slot=-1;
    std::uint64_t seenWrite=0;
    unsigned generation=0;
    char details[256]{};
};
CheckpointMissionScope checkpointMission{};
inline bool missionDetailsField(const char* text,const char* key,char* out,std::size_t size) {
    const auto length=std::strlen(key);
    for(const char* line=text;line&&*line;) {
        const char* end=std::strchr(line,'\n');const std::size_t n=end?static_cast<std::size_t>(end-line):std::strlen(line);
        if(n>length&&!std::strncmp(line,key,length)&&line[length]=='=') {
            const auto value=n-length-1;if(!value||value>=size)return false;
            std::memcpy(out,line+length+1,value);out[value]=0;return true;
        }
        line=end?end+1:nullptr;
    }
    return false;
}
// Reads the native last autosave slot and its game.details beside the lab's
// fixed checkpoint directory (the isolated fs_savepath profile). No packet
// controls a path or slot.
struct MissionAutosaveDetails {int slot=-1;std::uint64_t stamp=0;char map[192]{},chapter[16]{},mission[32]{};};
bool readMissionAutosave(MissionAutosaveDetails& d) {
    const auto& m=checkpointMission;
    if(!*checkpointDirectory)return false;
    std::uintptr_t game=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!memoryRead(base+0x39a5290,&game,8)||!game||!memoryRead(game+0x2e75d8+0x1c,&d.slot,4)||d.slot<0||d.slot>99)return false;
    char root[MAX_PATH]{};std::snprintf(root,sizeof(root),"%s",checkpointDirectory);
    char* cut=std::strrchr(root,'\\');char* cut2=std::strrchr(root,'/');if(cut2>cut)cut=cut2;if(!cut)return false;*cut=0;
    char path[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s\\SLOT%02d\\game.details",root,d.slot)>=static_cast<int>(sizeof(path)))return false;
    HANDLE file=CreateFileA(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,nullptr,OPEN_EXISTING,0,nullptr);
    if(file==INVALID_HANDLE_VALUE)return false;
    FILETIME write{};char text[1024]{};DWORD count=0;
    const bool read=GetFileTime(file,nullptr,nullptr,&write)&&ReadFile(file,text,sizeof(text)-1,&count,nullptr)&&count>0;CloseHandle(file);
    if(!read)return false;text[count]=0;
    char type[8]{};
    if(!missionDetailsField(text,"mapName",d.map,sizeof(d.map))||!missionDetailsField(text,"chapter",d.chapter,sizeof(d.chapter))||
       !missionDetailsField(text,"mission",d.mission,sizeof(d.mission))||!missionDetailsField(text,"slotType",type,sizeof(type))||std::strcmp(type,"3"))return false;
    // 1147b: the peer's last autosave must belong to the current map; right
    // after a load it may still be the previous map until the mission-start
    // autosave lands (1148: client still on its Dunwall autosave).
    if(std::strcmp(d.map,m.map))return false;
    d.stamp=(static_cast<std::uint64_t>(write.dwHighDateTime)<<32)|write.dwLowDateTime;
    return true;
}
// 1153: the autosave observer for paired saves uses the same native reader.
bool missionAutosaveStamp(int& slot,std::uint64_t& stamp){MissionAutosaveDetails d{};if(!readMissionAutosave(d))return false;slot=d.slot;stamp=d.stamp;return true;}
bool missionCheckpointDigest(unsigned char result[32]) {
    auto& m=checkpointMission;MissionAutosaveDetails d{};
    if(!readMissionAutosave(d))return false;
    const int slot=d.slot;const std::uint64_t stamp=d.stamp;const char* map=d.map;const char* chapter=d.chapter;const char* mission=d.mission;
    // Write generations are diagnostic; save pairing is autosave_pair.h.
    if(m.seenSlot>=0&&(slot!=m.seenSlot||stamp!=m.seenWrite))++m.generation;
    m.seenSlot=slot;m.seenWrite=stamp;m.slot=slot;
    std::snprintf(m.details,sizeof(m.details),"%s|%s|%s",map,chapter,mission);
    // 1214bm: the identity is the map and chapter. The autosave's mission field
    // depends on whether the mission-start autosave landed before or after the
    // map's mission activation, so after a level transition the two worlds
    // differ (take route-eotw-boatdeath-v1302k: -254924741 / -464858806) and
    // the peers rejected each other's hello. Details keep all three fields.
    char identity[sizeof(m.details)]{};std::snprintf(identity,sizeof(identity),"%s|%s",map,chapter);
    BCRYPT_ALG_HANDLE algorithm=nullptr;BCRYPT_HASH_HANDLE hash=nullptr;bool ok=false;
    if(BCryptOpenAlgorithmProvider(&algorithm,BCRYPT_SHA256_ALGORITHM,nullptr,0)>=0) {
        if(BCryptCreateHash(algorithm,&hash,nullptr,0,nullptr,0,0)>=0) {
            ok=BCryptHashData(hash,reinterpret_cast<PUCHAR>(identity),static_cast<ULONG>(std::strlen(identity)),0)>=0&&BCryptFinishHash(hash,result,32,0)>=0;
            BCryptDestroyHash(hash);
        }
        BCryptCloseAlgorithmProvider(algorithm,0);
    }
    return ok;
}
// Arm-time scope: the current map becomes the session map; Dunwall keeps the
// fixed SLOT82 fixture unchanged.
bool armCheckpointMission() {
    auto& m=checkpointMission;m={};
    std::uintptr_t game=0,name=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!memoryRead(base+0x39a5290,&game,8)||!game||!memoryRead(game+0x337138,&name,8)||!nativeName(name,m.map,sizeof(m.map)))return false;
    m.mission=std::strcmp(m.map,kGuardMap)!=0;m.mapId=m.mission?outgoing.mapId:coopMapId(kGuardMap);
    return true;
}

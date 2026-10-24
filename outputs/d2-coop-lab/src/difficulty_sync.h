#pragma once
// 1214kb (#177): the partner's world plays the host's difficulty. A joining partner's game kept its own profile's preset
// (chapter 109: co-op saves run Medium, the lab's normal saves Easy).
// Native path (d2-ghidra, confirmed static; the layout confirmed runtime in difficulty-override-v1881a):
// - the difficulty manager is embedded in the game at +0x1f7b90: dword +0 is 1 once initialized, +4 the preset index
//   (0 Easy, 1 Medium, 2 Hard, 3 Very Hard, 4 Custom), +8 the arkDeclDifficulty, +0x10 the active preset;
// - FUN_140648f20(manager, index) is the setter that the profile load (FUN_140648590) calls. It removes every registered
//   entity's difficulty modifiers, takes preset [index] of the decl (an index past the decl's presets takes the static
//   custom preset at 0x1439889f0, whose fields +0x68..+0xc4 are the 22 custom values), re-applies the modifiers, sets
//   ark_difficulty_current and writes the index (for Custom also the custom values) to the profile keys of the current
//   campaign slot ("difficultyPresetIndex_%02d", FUN_1406492c0, under the profile write lock). The partner plays on co-op
//   saves, whose profile is its own (savegame_coop/profile.bin, chapter 107), so her single-player campaigns keep theirs.
// The host publishes its index and custom values (D2DF) every 2 s and on a change. The partner applies a different value
// in gameplay, at most every 5 s. difficultySaveModel (0x143988aa4) is written for every preset and stays the partner's.
// Lab: coop_difficulty_sync on|off, and coop_difficulty_set <0-4> calls the same setter in the local game (tests).
#pragma pack(push,1)
struct DifficultyPacket {
    std::uint32_t magic=0x46443244; // "D2DF"
    std::uint16_t version=1,size=sizeof(DifficultyPacket);
    std::uint32_t role=0;std::int32_t index=-1;
    std::uint64_t session=0,sequence=0;
    std::int32_t custom[22]{};
};
#pragma pack(pop)
static_assert(sizeof(DifficultyPacket)==120);
constexpr std::uintptr_t kDifficultyManagerOffset=0x1f7b90,kDifficultySetterRva=0x648f20;
// The custom values in the profile's order (difficultyEnemyDamage ... difficultyLimitedLean, then MergeHealthMana,
// LightVisibility, RealTimeHud and the last custom key), image RVAs.
constexpr std::uintptr_t kDifficultyCustomRvas[22]={0x3988a58,0x3988a5c,0x3988a60,0x3988a64,0x3988a68,0x3988a6c,0x3988a70,
    0x3988a74,0x3988a78,0x3988a7c,0x3988a80,0x3988a84,0x3988a88,0x3988a8c,0x3988a90,0x3988a94,0x3988a98,0x3988a9c,0x3988ab4,
    0x3988aa0,0x3988aa8,0x3988aac};
struct DifficultySync {
    bool enabled=true;int hostIndex=-1,localIndex=-1,ownIndex=-1,labSet=-1;std::int32_t hostCustom[22]{};
    std::uint64_t sequence=0,peerSequence=0,sent=0,received=0,refused=0,applied=0,failures=0,labSets=0;
    ULONGLONG sentAt=0,receivedAt=0,appliedAt=0;int sentIndex=-1;const char* result="idle";
};
DifficultySync difficultySync{};
// The manager's address once the game has initialized it, else 0.
std::uintptr_t difficultyManager(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t game=0,decl=0;std::uint32_t ready=0;
    if(!memoryRead(base+0x39a5290,&game,8)||!game)return 0;const auto manager=game+kDifficultyManagerOffset;
    if(!memoryRead(manager,&ready,4)||ready!=1||!memoryRead(manager+8,&decl,8)||!decl)return 0;
    return manager;
}
bool readDifficultyCustom(std::int32_t (&out)[22]){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    for(unsigned i=0;i<22;++i)if(!memoryRead(base+kDifficultyCustomRvas[i],&out[i],4))return false;
    return true;
}
// Engine thread only: the native setter, preceded for Custom by the host's custom values.
bool applyDifficulty(std::uintptr_t manager,int index,const std::int32_t* custom){
    if(!manager||index<0||index>4)return false;
    if(index==4&&custom){const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        for(unsigned i=0;i<22;++i)*reinterpret_cast<std::int32_t*>(base+kDifficultyCustomRvas[i])=custom[i];}
    reinterpret_cast<void(*)(void*,int)>(gameImage+kDifficultySetterRva)(reinterpret_cast<void*>(manager),index);
    std::int32_t now=-1;return memoryRead(manager+4,&now,4)&&now==index;
}
void receiveDifficulty(const DifficultyPacket& p){
    auto& d=difficultySync;++d.received;
    if(p.magic!=0x46443244||p.version!=1||p.size!=sizeof(p)||p.role!=0||localRole!=1||p.session!=remote.session||p.sequence<=d.peerSequence||
       p.index<0||p.index>4){++d.refused;return;}
    d.peerSequence=p.sequence;d.hostIndex=p.index;std::memcpy(d.hostCustom,p.custom,sizeof(d.hostCustom));d.receivedAt=GetTickCount64();
}
void serviceDifficultySync(ULONGLONG tick){
    auto& d=difficultySync;const auto manager=difficultyManager();
    d.localIndex=-1;if(manager)memoryRead(manager+4,&d.localIndex,4);
    if(d.labSet>=0){const int index=d.labSet;d.labSet=-1;
        if(manager&&outgoing.gameLogicState==1){std::int32_t custom[22]{};const bool own=readDifficultyCustom(custom);
            if(applyDifficulty(manager,index,own?custom:nullptr))++d.labSets;else ++d.failures;}}
    if(!d.enabled){d.result="off";return;}
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000;
    if(!connected){d.result="no_session";return;}
    if(!manager){d.result="no_manager";return;}
    if(localRole==0){
        if(d.localIndex<0||d.localIndex>4){d.result="host_index_invalid";return;}
        if(d.localIndex==d.sentIndex&&tick-d.sentAt<2000)return;
        DifficultyPacket p{};p.index=d.localIndex;p.session=outgoing.session;p.sequence=++d.sequence;
        if(p.index==4&&!readDifficultyCustom(p.custom)){d.result="custom_unreadable";return;}
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){++d.sent;d.sentAt=tick;d.sentIndex=p.index;d.result="publishing";}
        return;
    }
    if(d.ownIndex<0)d.ownIndex=d.localIndex;
    if(d.hostIndex<0||!d.receivedAt||tick-d.receivedAt>10000){d.result="waiting_host";return;}
    bool same=d.localIndex==d.hostIndex;
    if(same&&d.hostIndex==4){std::int32_t custom[22]{};same=readDifficultyCustom(custom)&&!std::memcmp(custom,d.hostCustom,sizeof(custom));}
    if(same){d.result="matched";return;}
    if(outgoing.gameLogicState!=1||outgoing.pendingGameLogicState){d.result="waiting_gameplay";return;}
    if(d.appliedAt&&tick-d.appliedAt<5000){d.result="applying";return;}
    d.appliedAt=tick;
    if(applyDifficulty(manager,d.hostIndex,d.hostCustom)){++d.applied;d.result="applied";}else{++d.failures;d.result="apply_failed";}
}
void writeDifficultySync(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.difficulty.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& d=difficultySync;
    fprintf(f,"{\"enabled\":%s,\"role\":%u,\"result\":\"%s\",\"local\":%d,\"host\":%d,\"own\":%d,\"sent\":%llu,\"received\":%llu,\"refused\":%llu,\"applied\":%llu,\"failures\":%llu,\"labSets\":%llu}\n",
        d.enabled?"true":"false",localRole,d.result,d.localIndex,d.hostIndex,d.ownIndex,d.sent,d.received,d.refused,d.applied,d.failures,d.labSets);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

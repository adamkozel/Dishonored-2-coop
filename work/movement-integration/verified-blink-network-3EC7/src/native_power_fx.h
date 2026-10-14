#pragma once
// Two whitelisted, finite native Doppelganger particle/sound declarations.
// No native power activation, damage, target preview or screen-effect calls.
struct NativePowerFx {
    std::uintptr_t game=0,actor=0,declaration=0,record=0;unsigned owner=0,id=~0u,kind=0;
    std::uint64_t epoch=0,streamToken=0,peerToken=0;ULONGLONG expires=0;
};
std::array<NativePowerFx,8> nativePowerEffects{};
unsigned powerFxRequest=0;
std::uint64_t powerFxStarted=0,powerFxStopped=0,powerFxFinished=0,powerFxRefused=0,powerFxRetired=0;
const char* powerFxResult="idle";
const char* powerFxName(unsigned kind){
    return kind==1?"models/effects/gameplay/powers/doppelganger/decoy/blink/dganger_blink_start_01.fx":
        kind==2?"models/effects/gameplay/powers/doppelganger/decoy/blink/dganger_blink_out_01.fx":nullptr;
}
bool nativePowerFxApi(){
    const unsigned char start[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x83,0xec,0x30,0x48,0x83,0x7c,0x24,0x68,0x00,0x8b,0x99,0xa8};
    const unsigned char lookup[]={0x89,0x54,0x24,0x10,0x57,0x48,0x83,0xec,0x30,0x48,0xc7,0x44,0x24,0x20,0xfe,0xff,0xff,0xff,0x48,0x89,0x5c,0x24,0x40,0x48};
    const unsigned char stop[]={0x48,0x89,0x5c,0x24,0x08,0x89,0x54,0x24,0x10,0x57,0x48,0x83,0xec,0x20,0x48,0x8b,0xd9,0xe8,0x9a,0xd2,0xff,0xff,0x48,0x8b};
    const unsigned char getter[]={0x48,0x8d,0x81,0x30,0x84,0x1f,0x00,0xc3};
    return nativeMutationAllowed&&GetCurrentThreadId()==engineThread.load()&&powerFixtureApiMatches()&&
        !std::memcmp(gameImage+0x87cb90,start,sizeof(start))&&!std::memcmp(gameImage+0x1a04a0,lookup,sizeof(lookup))&&
        !std::memcmp(gameImage+0x1a31f0,stop,sizeof(stop))&&!std::memcmp(gameImage+0x8a8a70,getter,sizeof(getter));
}
std::uintptr_t nativePowerFxManager(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t game=0,vt=0,getter=0;
    if(!nativePowerFxApi()||!memoryRead(base+0x39a5290,&game,8)||game!=observedGame||!game||
       !memoryRead(game,&vt,8)||vt!=base+0x1c940a8||!memoryRead(vt+0x388,&getter,8)||getter!=base+0x8a8a70)return 0;
    return game+0x1f8430; // Exact native getter is LEA RAX,[RCX+1f8430]; RET.
}
std::uintptr_t nativePowerFxDefinition(unsigned kind){
    const auto name=powerFxName(kind);if(!name||!nativePowerFxManager())return 0;
    const unsigned char typeBytes[]={0x48,0x89,0x6c,0x24,0x18,0x56,0x48,0x83,0xec,0x20,0x48,0x8b,0xea,0x48,0x8b,0xf1};
    const unsigned char findBytes[]={0x48,0x85,0xd2,0x75,0x03,0x33,0xc0,0xc3,0x45,0x33,0xc9,0xe9,0x60,0xa3,0x01,0x00};
    if(std::memcmp(gameImage+0x1ea30,typeBytes,sizeof(typeBytes))||std::memcmp(gameImage+0x3cc0,findBytes,sizeof(findBytes)))return 0;
    auto type=reinterpret_cast<std::uintptr_t(*)(void*,const char*)>(gameImage+0x1ea30)(gameImage+0x24367e0,"fx");
    std::uintptr_t text=0,vt=0;std::string value;
    if(!type||!memoryRead(type+0x78,&text,8)||!worldIdentityString(memoryRead,text,value)||value!="fx")return 0;
    auto def=reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,const char*,bool)>(gameImage+0x3cc0)(type,name,false);
    if(!def||!memoryRead(def,&vt,8)||vt!=reinterpret_cast<std::uintptr_t>(gameImage)+0x1ab3138||
       !memoryRead(def+8,&text,8)||!worldIdentityString(memoryRead,text,value)||value!=name)return 0;
    return def;
}
std::uintptr_t nativePowerFxRecord(const NativePowerFx& e){
    const auto manager=nativePowerFxManager();if(!manager||e.game!=observedGame||e.epoch!=outgoing.worldEpoch||e.id==~0u)return 0;
    auto record=reinterpret_cast<std::uintptr_t(*)(std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t)>(gameImage+0x1a04a0)(manager,e.id,0,0);
    unsigned owner=0,id=0;std::uintptr_t declaration=0;
    if(!record||!memoryRead(record,&owner,4)||owner!=e.owner||!memoryRead(record+4,&id,4)||id!=e.id||
       !memoryRead(record+8,&declaration,8)||declaration!=e.declaration||(e.record&&e.record!=record))return 0;
    return record;
}
bool startNativePowerFx(unsigned kind,const float* origin,ULONGLONG tick,bool network=false){
    const auto reject=[&](const char* reason){++powerFxRefused;powerFxResult=reason;return false;};
    if(!nativePowerFxApi()||std::strcmp(mapName,kGuardMap)||!observerIsolation.ready||!observerIsolation.movementFixture||
       !coopGameplayReady(outgoing)||!coopGameplayReady(remote)||!probeDoppelganger||!avatarAiHeld||!probeFollowExact||
       avatarAiTarget.actor!=probeActor)return reject("unsupported_scope");
    NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSwordEquipment(memoryRead,base,probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||
       sword.actor!=probeActor||sword.game!=observedGame)return reject("avatar_identity_changed");
    NativePowerFx* slot=nullptr;for(auto& e:nativePowerEffects)if(!e.expires){slot=&e;break;}if(!slot)return reject("effect_slots_full");
    float point[3]{};if(origin)std::memcpy(point,origin,12);else if(!memoryRead(probeActor+0x300,point,12))return reject("origin_unavailable");
    for(float v:point)if(!std::isfinite(v)||std::fabs(v)>100000)return reject("invalid_origin");
    NativePowerFx e{};e.game=observedGame;e.actor=probeActor;e.owner=probeHandle;e.epoch=outgoing.worldEpoch;e.kind=kind;
    if(network){if(!shadowStreamCurrent(tick)||!shadowStream.peerReady)return reject("power_stream_changed");e.streamToken=shadowStream.token;e.peerToken=shadowStream.peerToken;}
    e.declaration=nativePowerFxDefinition(kind);if(!e.declaration)return reject("definition_unavailable");
    // ABI verified from machine instructions. Args 3..5 are unused. Explicit
    // origin makes this finite effect stay at the endpoint, not follow a body.
    using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
    reinterpret_cast<Start>(gameImage+0x87cb90)(probeActor,&e.id,0,0,0,e.declaration,point,nullptr,0,0,0,0);
    if(e.id==~0u)return reject("native_start_failed");
    e.expires=tick+1600;e.record=nativePowerFxRecord(e);*slot=e;++powerFxStarted;
    powerFxResult=e.record?"native_effect_owned":"native_effect_ownership_unconfirmed";return e.record!=0;
}
void serviceNativePowerFx(ULONGLONG tick){
    if(!nativePowerFxApi())return;const unsigned request=powerFxRequest;powerFxRequest=0;
    for(auto& e:nativePowerEffects)if(e.expires){
        if(e.game!=observedGame||e.epoch!=outgoing.worldEpoch){e={};++powerFxRetired;continue;}
        if(!coopGameplayReady(outgoing))continue; // Resume cleanup after native pause.
        if(!nativePowerFxRecord(e)){e={};++powerFxFinished;continue;}
        if(request==3||tick>=e.expires||!observerIsolation.ready||!observerIsolation.movementFixture||
           (e.streamToken&&(!shadowStreamCurrent(tick)||shadowStream.token!=e.streamToken||shadowStream.peerToken!=e.peerToken))){
            reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),e.id);
            ++powerFxStopped;e={};powerFxResult="owned_effect_stopped";
        }
    }
    if(request==1||request==2)startNativePowerFx(request,nullptr,tick);
}
void writeNativePowerFx(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.power-fx.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;unsigned active=0;for(const auto& e:nativePowerEffects)if(e.expires)++active;
    fprintf(f,"{\"result\":\"%s\",\"active\":%u,\"started\":%llu,\"stopped\":%llu,\"finished\":%llu,\"refused\":%llu,\"retired\":%llu,\"effects\":[",powerFxResult,active,powerFxStarted,powerFxStopped,powerFxFinished,powerFxRefused,powerFxRetired);
    bool first=true;for(const auto& e:nativePowerEffects)if(e.expires){fprintf(f,"%s{\"id\":%u,\"owner\":%u,\"kind\":%u,\"record\":\"0x%llx\",\"declaration\":\"0x%llx\"}",first?"":",",e.id,e.owner,e.kind,(unsigned long long)e.record,(unsigned long long)e.declaration);first=false;}
    fprintf(f,"]}\n");fclose(f);MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

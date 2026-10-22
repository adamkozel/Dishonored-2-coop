#pragma once
// 1214ie (#160): a Corvo partner's Bend Time acts on the host's NPCs. Her world's NPCs are replicas that follow the host's
// NPC stream, so her own time request slows nothing that matters: the host's NPCs keep real time and her copies with them.
// - Each side samples its own Bend Time: the player's power component at +0x378 (+0xd0 active, +0xc0 its request id;
//   FUN_14061c8f0), and that request's factor in the time manager (game+0x1f52e8: the clock-3 request list at +0, entries
//   of 12 bytes {id, factor, persist}, the count in the low 24 bits at +8, the smallest factor last and in force).
//   Chapter 108, confirmed static.
// - The pose flags carry it: bit 10 while it runs, the factor's percent in bits 16-22 (Stop Time is 0; pose version 13).
// - The host adds the same factor as a request of its own, as the game's non-power users do (FUN_14049a250):
//   FUN_14080eb40(tm, &id, factor, 1.0, blend, persist 0), so it is never saved. Both players keep real time through
//   the manager's own exemption FUN_14080d8c0(tm, entity, 1e30), as the power exempts its caster.
// - When her Bend Time ends, or the stream, the world or the session does, FUN_14080ede0(tm, id, blend) removes it, and
//   FUN_14080dab0 returns both players to world time unless the host's own Bend Time runs.
// - The screen effect, sounds, events and mana stay with the caster: the power's own toggle (14061cd60) is never called.
// 1214if: partner-bendtime-v1797a froze the host player too (0 m walks in her Stop Time). The manager's update rebends
// every exempted entity while byte +0x1a0 of the engine time object (game+0x2e7428) is 0 (0x14080f18e, confirmed
// static); what sets it is not known. The status records it with both players' clocks (entity+0x2ac: 0 world, 1
// unbent) and the host player's exemption seconds (+0x2a8). The lab switch coop_time_bend unbent70 gives the players'
// clock the native unbent scale (decl m_unbentTimeScale 70) to test whether a scaled clock 4 is what sets it. The blend
// times are her native ones (her declaration: in 0.5 s, out 1.0 s; partner-bendtime-v1797a).
constexpr std::uint32_t kTimeBendActive=0x400u;constexpr float kTimeBendBlendIn=.5f,kTimeBendBlendOut=1.f;
struct TimeBendHost {int request=-1;std::uintptr_t game=0,avatar=0;std::uint64_t epoch=0;unsigned percent=0;};
TimeBendHost timeBendHost{};bool timeBendEnabled=true;const char* timeBendResult="idle";float timeBendUnbentScale=1.f;ULONGLONG timeBendRemoteSeenAt=0;
std::uint64_t timeBendAdded=0,timeBendRemoved=0,timeBendGone=0,timeBendExempted=0,timeBendReleased=0;
bool timeBendLocalActive=false,timeBendRemoteActive=false;unsigned timeBendLocalPercent=0,timeBendRemotePercent=0;int timeBendLocalRequest=-1;
float timeBendLocalBlend[2]{};
bool timeBendFindRequest(std::uintptr_t game,int id,float* factor){
    std::uintptr_t list=0;std::uint32_t count=0;const auto tm=game+0x1f52e8;
    if(!game||id<0||!memoryRead(tm,&list,8)||!memoryRead(tm+8,&count,4))return false;count&=0xffffff;
    if(!list||count>64)return false;
    for(std::uint32_t i=0;i<count;++i){int rid=0;if(!memoryRead(list+i*12,&rid,4))return false;
        if(rid==id){float f=1;if(!memoryRead(list+i*12+4,&f,4)||!std::isfinite(f))return false;if(factor)*factor=f;return true;}}
    return false;}
float timeBendWorldFactor(std::uintptr_t game,std::uint32_t* requests){
    std::uintptr_t list=0;std::uint32_t count=0;float f=1;const auto tm=game+0x1f52e8;if(requests)*requests=0;
    if(!game||!memoryRead(tm,&list,8)||!memoryRead(tm+8,&count,4))return 1;count&=0xffffff;if(requests)*requests=count;
    if(!list||!count||count>64||!memoryRead(list+(count-1)*12+4,&f,4)||!std::isfinite(f))return 1;return f;}
int timeBendClock(std::uintptr_t entity){int c=-1;if(!entity||!memoryRead(entity+0x2ac,&c,4))return -1;return c;}
float timeBendExemptSeconds(std::uintptr_t entity){float s=-2;if(!entity||!memoryRead(entity+0x2a8,&s,4)||!std::isfinite(s))return -2;return s;}
bool timeBendOwnActive(){std::uintptr_t c=0;unsigned char a=0;return observedPlayer&&memoryRead(observedPlayer+0x378,&c,8)&&c&&memoryRead(c+0xd0,&a,1)&&a==1;}
void sampleLocalTimeBend(){
    std::uint32_t flags=0;std::uintptr_t c=0,decl=0;unsigned char a=0;int id=-1;float factor=1;
    timeBendLocalActive=false;timeBendLocalRequest=-1;
    if(observedGame&&observedPlayer&&memoryRead(observedPlayer+0x378,&c,8)&&c&&memoryRead(c+0xd0,&a,1)&&a==1&&memoryRead(c+0xc0,&id,4)&&
       timeBendFindRequest(observedGame,id,&factor)&&factor>=0&&factor<1){
        unsigned percent=static_cast<unsigned>(factor*100.f+.5f);if(percent>99)percent=99;
        flags=kTimeBendActive|(percent<<16);timeBendLocalActive=true;timeBendLocalPercent=percent;timeBendLocalRequest=id;
        if(memoryRead(c+8,&decl,8)&&decl){memoryRead(decl+0x19c,&timeBendLocalBlend[0],4);memoryRead(decl+0x1a0,&timeBendLocalBlend[1],4);}}
    coopTimeBendFlags.store(flags);}
void timeBendExempt(std::uintptr_t entity){
    if(!entity||!observedGame)return;
    reinterpret_cast<void(*)(std::uintptr_t,std::uintptr_t,float)>(gameImage+0x80d8c0)(observedGame+0x1f52e8,entity,1e30f);++timeBendExempted;}
void timeBendRelease(const char* why){
    auto& h=timeBendHost;
    if(h.request>=0&&h.game==observedGame&&timeBendFindRequest(observedGame,h.request,nullptr)){
        const auto tm=observedGame+0x1f52e8;
        reinterpret_cast<void(*)(std::uintptr_t,int,float)>(gameImage+0x80ede0)(tm,h.request,kTimeBendBlendOut);++timeBendRemoved;
        if(!timeBendOwnActive()){const auto rebend=reinterpret_cast<void(*)(std::uintptr_t,std::uintptr_t)>(gameImage+0x80dab0);
            if(observedPlayer){rebend(tm,observedPlayer);++timeBendReleased;}
            if(h.avatar&&h.avatar==probeActor&&probeGame==observedGame){rebend(tm,h.avatar);++timeBendReleased;}}}
    else if(h.request>=0)++timeBendGone;
    h={};timeBendResult=why;}
void serviceTimeBendHost(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    sampleLocalTimeBend();
    auto& h=timeBendHost;
    timeBendRemoteActive=(remote.flags&kTimeBendActive)!=0;timeBendRemotePercent=(remote.flags>>16)&0x7fu;
    if(h.request>=0&&(h.game!=observedGame||h.epoch!=outgoing.worldEpoch))timeBendRelease("world_changed");
    const bool fresh=lastReceive&&tick-lastReceive<1000;
    // 1214if: her Stop Time re-issues its request on a second press, so for a frame she has none (v1797a: the host
    // released and added again); the host keeps its request through 300 ms without her bit.
    const bool active=timeBendRemoteActive&&timeBendRemotePercent<=99;if(active)timeBendRemoteSeenAt=tick;
    const bool held=active||(h.request>=0&&timeBendRemoteSeenAt&&tick-timeBendRemoteSeenAt<300);
    const bool want=timeBendEnabled&&localRole==0&&observedGame&&observedPlayer&&fresh&&coopWorldPairReady(outgoing,remote)&&
        coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&held;
    if(!want){if(h.request>=0)timeBendRelease(!fresh?"stream_stale":!held?"partner_ended":"not_ready");return;}
    if(h.request>=0&&active&&h.percent!=timeBendRemotePercent)timeBendRelease("factor_changed");
    if(h.request<0&&active){
        int id=-1;
        reinterpret_cast<int*(*)(std::uintptr_t,int*,float,float,float,bool)>(gameImage+0x80eb40)(observedGame+0x1f52e8,&id,timeBendRemotePercent*.01f,timeBendUnbentScale,kTimeBendBlendIn,false);
        h.request=id;h.game=observedGame;h.epoch=outgoing.worldEpoch;h.percent=timeBendRemotePercent;++timeBendAdded;timeBendResult="applied";
        timeBendExempt(observedPlayer);}
    if(h.request>=0&&probeActor&&probeGame==observedGame&&h.avatar!=probeActor){timeBendExempt(probeActor);h.avatar=probeActor;}}
void writeTimeBend(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.time-bend.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& h=timeBendHost;std::uint32_t requests=0;const float world=timeBendWorldFactor(observedGame,&requests);
    unsigned char gtm[5]{};float unbentClock=-1;const std::uintptr_t avatar=probeGame==observedGame?probeActor:0;
    if(observedGame){memoryRead(observedGame+0x2e7428+0x1a0,gtm,5);memoryRead(observedGame+0x2e7428+0x24+4*0x50,&unbentClock,4);}
    fprintf(f,"{\"enabled\":%s,\"role\":%u,\"local\":{\"active\":%s,\"percent\":%u,\"request\":%d,\"blendIn\":%.3f,\"blendOut\":%.3f,\"flags\":%u},\"remote\":{\"active\":%s,\"percent\":%u},"
        "\"host\":{\"request\":%d,\"percent\":%u,\"avatar\":%s,\"unbentScale\":%.2f},\"clocks\":{\"player\":%d,\"avatar\":%d,\"playerExempt\":%.3g,\"avatarExempt\":%.3g,\"unbent\":%.3f,\"gtm\":[%u,%u,%u,%u,%u]},"
        "\"worldFactor\":%.3f,\"requests\":%u,\"ownActive\":%s,\"added\":%llu,\"removed\":%llu,\"gone\":%llu,\"exempted\":%llu,\"released\":%llu,\"result\":\"%s\"}\n",
        timeBendEnabled?"true":"false",localRole,timeBendLocalActive?"true":"false",timeBendLocalPercent,timeBendLocalRequest,timeBendLocalBlend[0],timeBendLocalBlend[1],coopTimeBendFlags.load(),
        timeBendRemoteActive?"true":"false",timeBendRemotePercent,h.request,h.percent,h.avatar?"true":"false",timeBendUnbentScale,
        timeBendClock(observedPlayer),timeBendClock(avatar),timeBendExemptSeconds(observedPlayer),timeBendExemptSeconds(avatar),unbentClock,gtm[0],gtm[1],gtm[2],gtm[3],gtm[4],world,requests,timeBendOwnActive()?"true":"false",
        static_cast<unsigned long long>(timeBendAdded),static_cast<unsigned long long>(timeBendRemoved),static_cast<unsigned long long>(timeBendGone),
        static_cast<unsigned long long>(timeBendExempted),static_cast<unsigned long long>(timeBendReleased),timeBendResult);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);}

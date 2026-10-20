#pragma once
#include "native_player_choke_scene.h"
#include <cstdio>
// 1214ga (#140): why a partner's takedown is not recognized as a paired scene. Emily's kill of Ramsey (takes
// route-dunwall-lead-v1702a/c on 1214fz) never set localPaired, so no PairedHold reached the host. The host's AI
// walked him about 10 m during her scene, and her capture then refused the kill as hit_range (8.55 m), so he lived on
// in Corvo's world. The probe records her active states, the takedown state checks and the scenes the scene manager
// holds (asset, subscene, phase, duration and the census variant they map to). It runs when one of her active states
// is a takedown state (Assassination13, Choke41, Versus31) but no recognized scene names its target, and when her
// capture sees an assassination or fatality. Diagnostic only: read-only and rate-limited, kept in a ring of 8.
struct PlayerSceneProbeEntry {char asset[96]{},sub[48]{};unsigned phase=0,duration=0,variant=0;};
struct PlayerSceneProbe {
    unsigned long long tick=0;const char* reason="";unsigned logic=0,stance=0,checks=0,stateCount=0,sceneCount=0;bool defaultMode=false;
    std::uint32_t states[8]{};std::uintptr_t vtables[8]{};PlayerSceneProbeEntry scenes[6]{};
};
PlayerSceneProbe playerSceneProbes[8]{};unsigned playerSceneProbeCount=0;unsigned long long playerSceneProbeAt=0;
inline unsigned playerTakedownChecks(const NativePlayerStates& s,std::uintptr_t base){
    return (nativePlayerGroundChokeStates(s,base)?1u:0u)|(nativePlayerGroundAssassinationStates(s,base)?2u:0u)|(nativePlayerSlideTakedownStates(s,base)?4u:0u)|
           (nativeFatalityStates(s,base)?8u:0u)|(nativePlayerVersusStates(s,base)?16u:0u)|(nativePlayerShadowWalkAssassinationStates(s,base)?32u:0u)|
           (nativePlayerShadowWalkChokeStates(s,base)?64u:0u);
}
inline bool playerTakedownStateActive(const NativePlayerStates& s){
    for(const auto& a:s.active)if(a.id==13||a.id==41||a.id==31)return true;
    return false;
}
inline void playerSceneProbeCopy(char* out,std::size_t size,const std::string& text){
    const char* from=text.size()>=size?text.c_str()+(text.size()-(size-1)):text.c_str();std::size_t i=0;
    for(;from[i]&&i+1<size;++i){const char c=from[i];out[i]=(c=='"'||c=='\\'||c<32||c>126)?'_':c;}out[i]=0;
}
template<class Read> void probePlayerScene(Read read,std::uintptr_t base,const NativePlayerStates& states,const char* reason,unsigned long long tick,bool force=false){
    if(!force&&tick-playerSceneProbeAt<250)return;playerSceneProbeAt=tick;
    PlayerSceneProbe p{};p.tick=tick;p.reason=reason;p.logic=states.player.logic;p.stance=states.player.current;p.defaultMode=states.defaultMode;
    p.checks=playerTakedownChecks(states,base);
    for(const auto& a:states.active){if(p.stateCount>=8)break;p.vtables[p.stateCount]=a.vtable?a.vtable-base:0;p.states[p.stateCount++]=a.id;}
    const auto manager=states.player.game+0x1f5338;
    for(unsigned field=0;field<2&&p.sceneCount<6;++field){std::uintptr_t list=0;unsigned flags=0;const auto at=manager+(field?0x610:0);
        if(!read(at,&list,8)||!read(at+8,&flags,4)||(flags&0xffffff)>512||!list)continue;
        for(unsigned i=0;i<(flags&0xffffff)&&p.sceneCount<6;++i){std::uintptr_t c=0,d=0,vt=0,name=0,sc=0;unsigned phase=0,duration=0;std::string asset,sub;
            if(!read(list+i*8,&c,8)||!c||!read(c+0x27c,&phase,4))continue;
            if(!read(c,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1affbc8||!read(d+8,&name,8)||!worldIdentityString(read,name,asset))continue;
            if(phase!=3&&asset.find("/player/")==std::string::npos)continue;
            if(read(c+8,&sc,8)&&sc&&read(sc,&vt,8)&&vt==base+0x1b00198&&read(sc+0x28,&duration,4)&&read(sc+0x30,&name,8))worldIdentityString(read,name,sub);
            auto& e=p.scenes[p.sceneCount++];playerSceneProbeCopy(e.asset,sizeof(e.asset),asset);playerSceneProbeCopy(e.sub,sizeof(e.sub),sub);
            e.phase=phase;e.duration=duration;e.variant=nativeChokeVariant(asset,sub,duration);}}
    playerSceneProbes[playerSceneProbeCount++%8]=p;
}
inline void writePlayerSceneProbes(FILE* f){
    fputs(",\"sceneProbes\":[",f);const unsigned first=playerSceneProbeCount>8?playerSceneProbeCount-8:0;
    for(unsigned i=first;i<playerSceneProbeCount;++i){const auto& p=playerSceneProbes[i%8];
        fprintf(f,"%s{\"tick\":%llu,\"reason\":\"%s\",\"logic\":%u,\"stance\":%u,\"defaultMode\":%s,\"checks\":%u,\"states\":[",i==first?"":",",p.tick,p.reason,p.logic,p.stance,p.defaultMode?"true":"false",p.checks);
        for(unsigned k=0;k<p.stateCount;++k)fprintf(f,"%s{\"id\":%u,\"vtableRva\":\"0x%llx\"}",k?",":"",p.states[k],static_cast<unsigned long long>(p.vtables[k]));
        fputs("],\"scenes\":[",f);
        for(unsigned k=0;k<p.sceneCount;++k){const auto& e=p.scenes[k];
            fprintf(f,"%s{\"asset\":\"%s\",\"sub\":\"%s\",\"phase\":%u,\"duration\":%u,\"variant\":%u}",k?",":"",e.asset,e.sub,e.phase,e.duration,e.variant);}
        fputs("]}",f);}
    fputs("]",f);
}

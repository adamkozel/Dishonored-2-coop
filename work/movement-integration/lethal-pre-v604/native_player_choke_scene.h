#pragma once
#include "native_player_states.h"
#include "native_player_arm.h"
#include "world_identity.h"
#include <array>
// Exact native579 rear choke: state41 and four reciprocal bindings. Native
// first-person and victim animations continue untouched on the source.
struct NativePlayerChokeScene {
    std::uintptr_t instance=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0,character=0;
    NativeWorldNpc target{};std::array<NativePlayerArm,2> arms{};
};
inline unsigned nativeChokeVariant(const std::string& asset,const std::string& sub,unsigned duration){
    if(sub!="Npc")return 0;
    const std::string prefix="midnight/gameplay/player/base/choke/choke_";
    if(duration==600&&asset==prefix+"back.midnightscene")return 1;
    if(duration==1400&&asset==prefix+"loop.midnightscene")return 2;
    if(duration==1800&&asset==prefix+"win.midnightscene")return 3;
    // Native591: releasing input early eventually lets the guard break free.
    // This is not the separate, still unverified cancel400 scene.
    if(duration==1500&&asset==prefix+"combat_lose.midnightscene")return 4;
    return 0;
}
template<class Read> bool readNativePlayerChokeScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerChokeScene& out){
    if(!nativePlayerGroundChokeStates(states,base))return false;
    NativePlayerChokeScene s{};const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    std::uintptr_t declaration=0,scene=0,bindings=0;unsigned bindingFlags=0;
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&flags[field],4)||(flags[field]&0xffffff)>512||((flags[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(flags[field]&0xffffff);++i){
            std::uintptr_t c=0,d=0,vt=0,name=0,sc=0;unsigned phase=0,duration=0;std::string asset,sub;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x27c,&phase,4))return false;
            if(phase!=3)continue;
            if(!read(c,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1affbc8||!read(d+8,&name,8)||!worldIdentityString(read,name,asset))return false;
            if(asset.find("midnight/gameplay/player/base/choke/")!=0)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||!read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const auto variant=nativeChokeVariant(asset,sub,duration);if(!variant)return false;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    unsigned clocks[3]{};
    if(!s.instance||!read(s.instance+0x15c,clocks,sizeof(clocks))||clocks[0]!=clocks[1]||clocks[2]||clocks[1]>s.durationMs||
       !read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x10,&bindings,8)||!bindings||
       !read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=4)return false;
    s.timeMs=clocks[1];std::array<std::uintptr_t,4> actors{};unsigned players=0,targets=0,arms=0,sides=0;
    for(unsigned i=0;i<4;++i){unsigned handle=0;std::uintptr_t vt=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i]||!read(actors[i],&vt,8))return false;
        if(actors[i]==states.player.player){if(++players>1||handle!=1)return false;}
        else if(vt==base+0x1c5e108){
            if(++targets>1||!readWorldNpcIdentity(read,base,game,handle,s.target)||!s.target.unique||!s.target.reciprocal||s.target.actor!=actors[i])return false;
        }else{
            NativePlayerArm arm{};
            if(arms>=2||!readNativePlayerArm(read,base,game,handle,arm)||arm.actor!=actors[i]||arm.player!=states.player.player||
               (sides&(1u<<arm.side))||(s.character&&s.character!=arm.character))return false;
            s.arms[arms++]=arm;sides|=1u<<arm.side;s.character=arm.character;
        }
    }
    if(players!=1||targets!=1||arms!=2||sides!=6)return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<4;++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;
        NativePlayerArm arm{};if(!readNativePlayerArm(read,base,game,s.arms[i].handle,arm)||!sameNativePlayerArm(s.arms[i],arm))return false;}
    NativePlayerStates last{};NativeWorldNpc target{};
    if(!readNativePlayerStates(read,base,last)||!nativePlayerGroundChokeStates(last,base)||last.player.game!=game||last.player.player!=states.player.player||
       last.component!=states.component||last.mode!=states.mode||last.player.current!=states.player.current||last.active[0].pointer!=states.active[0].pointer||
       !readWorldNpcIdentity(read,base,game,s.target.handle,target)||!target.unique||target.actor!=s.target.actor||target.spawner!=s.target.spawner||target.component!=s.target.component||
       target.definition!=s.target.definition||target.spawnerName!=s.target.spawnerName)return false;
    out=std::move(s);return true;
}

#pragma once
#include "native_player_placeable.h"
#include "native_player_states.h"
// Native568: owned left-arm scenes; placement adds item+player, empty adds
// player. Both gadget types share clocks. No placement/damage events replayed.
struct NativePlayerTrapScene {
    NativePlayerPlaceable trap{};NativePlayerArm arm{};
    std::uintptr_t instance=0;unsigned handle=0,variant=0,timeMs=0,durationMs=0;
    std::string asset,subscene;
};
inline unsigned nativeTrapVariant(const std::string& asset,const std::string& sub,unsigned duration){
    const std::string prefix="midnight/gameplay/player/base/gadgets/gadget_stunmine_";
    if(sub=="Scene_1"){
        if(duration==500&&asset==prefix+"equip.midnightscene")return 1;
        if(duration==500&&asset==prefix+"unequip.midnightscene")return 2;
        if(duration==300&&asset==prefix+"place.midnightscene")return 5;
        if(duration==333&&asset==prefix+"ooa.midnightscene")return 6;
        if(duration==866&&asset==prefix+"reload.midnightscene")return 7; // Native Emily572 binds arm+player+item even after stock reaches0.
    }
    if(asset==prefix+"ready.midnightscene"){
        if(duration==200&&sub=="In")return 3;
        if(duration==666&&sub=="Looping")return 4;
    }
    return 0; // Ready-out/slide are not inferred from unobserved scenes.
}
inline bool nativeTrapLocalClock(unsigned variant,unsigned duration,const unsigned (&clocks)[3],unsigned& local){
    if(clocks[0]>3600000||clocks[0]!=clocks[1]+clocks[2]||clocks[1]>duration)return false;
    if(variant==4){if(clocks[2]<200||(clocks[2]-200)%666)return false;}
    else if(clocks[2])return false;
    local=clocks[1];return true;
}
template<class Read> bool readNativePlayerTrapScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerTrapScene& out){
    NativePlayerTrapScene s{};
    unsigned motion=0;
    if(!nativePlayerMotion(states,base,motion)||motion>1||states.player.current!=motion)return false;
    for(const auto& a:states.active)if(a.id!=0&&a.id!=6&&a.id!=53&&a.id!=64&&a.id!=60&&a.id!=61&&a.id!=81)return false;
    const auto game=states.player.game,manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> counts{};
    std::uintptr_t declaration=0,scene=0,bindings=0;unsigned bindingFlags=0;
    for(unsigned field=0;field<2;++field){const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&counts[field],4)||(counts[field]&0xffffff)>512||((counts[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(counts[field]&0xffffff);++i){
            std::uintptr_t c=0,d=0,vt=0,name=0,sc=0;unsigned phase=0,duration=0;std::string asset,sub;
            if(!read(lists[field]+i*8,&c,8)||!c||!read(c+0x27c,&phase,4))return false;
            if(phase!=3)continue;
            if(!read(c,&d,8)||!d||!read(d,&vt,8)||vt!=base+0x1affbc8||!read(d+8,&name,8)||!worldIdentityString(read,name,asset))return false;
            if(asset.find("midnight/gameplay/player/base/gadgets/gadget_stunmine_")!=0)continue;
            if(!read(c+8,&sc,8)||!sc||!read(sc,&vt,8)||vt!=base+0x1b00198||!read(sc+0x28,&duration,4)||
               !read(sc+0x30,&name,8)||!worldIdentityString(read,name,sub))return false;
            const unsigned variant=nativeTrapVariant(asset,sub,duration);if(!variant)return false;
            if(s.instance){if(s.instance==c)continue;return false;}
            s.instance=c;s.variant=variant;s.asset=std::move(asset);s.subscene=std::move(sub);s.durationMs=duration;declaration=d;scene=sc;
        }
    }
    if(!s.variant)return false;
    // Prefer the actual selected left-slot item. An unselected holster scene
    // is accepted only with one unique acquired trap; ambiguous inventories
    // fall back to grounded presentation instead of guessing an item kind.
    NativePlayerPlaceable selected{},unique{};unsigned selectedCount=0,acquiredCount=0;
    for(unsigned kind=1;kind<=2;++kind){NativePlayerPlaceable p{};
        if(!readNativePlayerPlaceable(read,base,kind,p))return false;
        if(p.component){unique=p;++acquiredCount;if(p.equipped){selected=p;++selectedCount;}}
    }
    if(selectedCount==1)s.trap=selected;
    else if(!selectedCount&&s.variant==2&&acquiredCount==1)s.trap=unique;
    else return false;
    if(s.trap.player.sword.game!=states.player.game||s.trap.player.sword.actor!=states.player.player)return false;
    const unsigned expectedItems=(s.variant==5||s.variant==7)?1u:0u;
    const unsigned expectedPlayers=(s.variant==5||s.variant==6||s.variant==7)?1u:0u;
    const unsigned expectedBindings=1+expectedItems+expectedPlayers;
    // Probe clockWords begins at +15c: total +15c, local +160, start +164.
    // Native568 loop records include {217,17,200} and {910,44,866}.
    unsigned clocks[3]{};
    if(!read(s.instance+0x15c,clocks,sizeof(clocks))||!nativeTrapLocalClock(s.variant,s.durationMs,clocks,s.timeMs))return false;
    if(!s.instance||!read(s.instance+0x158,&s.handle,4)||!s.handle||s.timeMs>s.durationMs||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=expectedBindings)return false;
    std::array<std::uintptr_t,3> actors{};unsigned armCount=0,itemCount=0,playerCount=0;
    for(unsigned i=0;i<expectedBindings;++i){unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==s.trap.entity){if(++itemCount>1||handle!=s.trap.handle)return false;}
        else if(actors[i]==states.player.player){if(++playerCount>1||handle!=1)return false;}
        else if(++armCount>1||!readNativePlayerArm(read,base,game,handle,s.arm)||s.arm.actor!=actors[i]||s.arm.player!=states.player.player||s.arm.side!=1||s.arm.character!=s.trap.player.character)return false;
    }
    if(armCount!=1||itemCount!=expectedItems||playerCount!=expectedPlayers)return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<expectedBindings;++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=counts[i])return false;}
    NativePlayerStates finalStates{};NativePlayerPlaceable finalItem{};NativePlayerArm finalArm{};
    if(!readNativePlayerStates(read,base,finalStates)||finalStates.player.game!=game||finalStates.player.player!=states.player.player||
       finalStates.player.current!=states.player.current||finalStates.component!=states.component||finalStates.mode!=states.mode||
       !finalStates.defaultMode||finalStates.active.size()!=states.active.size()||!readNativePlayerPlaceable(read,base,s.trap.kind,finalItem)||
       finalItem.entry!=s.trap.entry||finalItem.component!=s.trap.component||finalItem.entity!=s.trap.entity||finalItem.handle!=s.trap.handle||
       finalItem.quantity!=s.trap.quantity||finalItem.flags!=s.trap.flags||finalItem.equipped!=s.trap.equipped||
       !readNativePlayerArm(read,base,game,s.arm.handle,finalArm)||!sameNativePlayerArm(s.arm,finalArm))return false;
    for(unsigned i=0;i<states.active.size();++i)if(states.active[i].id!=finalStates.active[i].id||states.active[i].pointer!=finalStates.active[i].pointer||states.active[i].vtable!=finalStates.active[i].vtable)return false;
    out=std::move(s);return true;
}

#pragma once
#include "native_player_states.h"
#include "native_player_arm.h"
#include <array>

// Read-only native source observation, not a corpse attachment or authority
// implementation. Carry45 owns scene+24 / corpse+f8 (140829030, 14082a110).
// Drop46 copies that corpse to +38 and owns its release scene (140831fe0,
// 140832770). Both paths were correlated against actual Dunwall scenes.
struct NativePlayerCarryScene {
    std::uintptr_t statePointer=0,instance=0,corpse=0;
    unsigned stateId=0,handle=0,corpseHandle=0,variant=0,phase=0;
    unsigned timeMs=0,durationMs=0,character=0,stance=0;
    std::array<unsigned char,3> releaseFlags{};
    std::array<float,3> throwVelocity{};
    std::string asset,subscene;
};
// Native-pickup-geometry-*-v1 and crouched-corvo-v20 show Carry45 phase 1 while the
// equip overlay holsters the sword. No Midnight scene exists yet. Keep only
// grounded presentation alive; this is not evidence of a lifting animation.
struct NativePlayerCarryPreparation {
    std::uintptr_t statePointer=0,corpse=0;
    unsigned corpseHandle=0;
};
template<class Read> bool readNativePlayerCarryPreparation(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerCarryPreparation& out){
    if(!states.defaultMode||states.player.logic!=1||states.player.current>1||states.active.size()!=3)return false;
    NativePlayerCarryPreparation s{};unsigned mask=0;
    for(const auto& a:states.active){
        unsigned bit=0;
        if((states.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (states.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))bit=1;
        if(a.id==45&&a.vtable==base+0x1c77748){bit=2;s.statePointer=a.pointer;}
        if(a.id==53&&a.vtable==base+0x1c79cf8)bit=4;
        if(!bit||(mask&bit))return false;mask|=bit;
    }
    unsigned scene=0,phase=0;
    const auto game=states.player.game;
    if(mask!=7||!read(s.statePointer+0x24,&scene,4)||scene||
       !read(s.statePointer+0x110,&phase,4)||phase!=1||
       !read(s.statePointer+0xf8,&s.corpseHandle,4)||!s.corpseHandle||s.corpseHandle==1)return false;
    s.corpse=worldIdentityEntity(read,game,s.corpseHandle);std::uintptr_t vt=0;
    if(!s.corpse||!read(s.corpse,&vt,8)||vt!=base+0x1c5e108)return false;
    NativePlayerStates last{};
    if(!readNativePlayerStates(read,base,last)||last.player.game!=game||last.player.player!=states.player.player||
       last.component!=states.component||last.mode!=states.mode||!last.defaultMode||last.player.current!=states.player.current||
       last.active.size()!=states.active.size())return false;
    for(unsigned i=0;i<last.active.size();++i){
        const auto& a=states.active[i];const auto& b=last.active[i];
        if(a.id!=b.id||a.pointer!=b.pointer||a.vtable!=b.vtable)return false;
    }
    unsigned number=0;
    if(worldIdentityEntity(read,game,s.corpseHandle)!=s.corpse||!read(s.corpse,&vt,8)||vt!=base+0x1c5e108||
       !read(s.statePointer+0x24,&number,4)||number||!read(s.statePointer+0x110,&number,4)||number!=1||
       !read(s.statePointer+0xf8,&number,4)||number!=s.corpseHandle)return false;
    out=s;return true;
}
// Variants are source semantics only. Their presence is not playback coverage.
inline unsigned nativeCarryVariant(const std::string& asset,bool observeHighThrow=false){
    constexpr const char* names[]={"in","loop","walk","drop_low_stand","drop_fetal_pose","drop_low_sneak"};
    for(unsigned i=0;i<6;++i)if(asset==std::string("midnight/gameplay/player/base/carrycorpse/carry_corpse_")+names[i]+".midnightscene")return i+1;
    // Variant7 is preparation. High throw requires explicit opt-in; current
    // authored carrier/physics integration is an isolated experiment.
    if(observeHighThrow&&asset=="midnight/gameplay/player/base/carrycorpse/carry_corpse_drop_high.midnightscene")return 8;
    return 0;
}
inline bool nativePlayerCarryStates(const NativePlayerStates& s,std::uintptr_t base,unsigned& id,std::uintptr_t& pointer){
    if(!s.defaultMode||s.player.logic!=1||s.player.current>1||s.active.size()<2||s.active.size()>3)return false;
    unsigned baseCount=0,carryCount=0,overlayCount=0;bool seen[90]{};
    for(const auto& a:s.active){
        if(a.id>=90||seen[a.id])return false;seen[a.id]=true;
        if((s.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||
           (s.player.current==1&&a.id==6&&a.vtable==base+0x1c84258)){++baseCount;continue;}
        if((a.id==45&&a.vtable==base+0x1c77748)||(a.id==46&&a.vtable==base+0x1c790c8)){
            ++carryCount;id=a.id;pointer=a.pointer;continue;
        }
        if((a.id==53&&a.vtable==base+0x1c79cf8)||(a.id==81&&a.vtable==base+0x1c7df88)){++overlayCount;continue;}
        return false;
    }
    return baseCount==1&&carryCount==1&&overlayCount<=1;
}
template<class Read> bool readNativePlayerCarryScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerCarryScene& out,bool observeHighThrow=false){
    NativePlayerCarryScene s{};
    if(!nativePlayerCarryStates(states,base,s.stateId,s.statePointer))return false;
    s.stance=states.player.current;const auto game=states.player.game;
    const unsigned corpseOffset=s.stateId==45?0xf8:0x38,phaseOffset=s.stateId==45?0x110:0x54;
    if(!read(s.statePointer+0x24,&s.handle,4)||!s.handle||!read(s.statePointer+corpseOffset,&s.corpseHandle,4)||
       !read(s.statePointer+phaseOffset,&s.phase,4)||!s.corpseHandle||s.corpseHandle==1)return false;
    s.corpse=worldIdentityEntity(read,game,s.corpseHandle);std::uintptr_t vt=0;
    if(!s.corpse||!read(s.corpse,&vt,8)||vt!=base+0x1c5e108)return false;
    const auto manager=game+0x1f5338;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    for(unsigned field=0;field<2;++field){
        const auto at=manager+(field?0x610:0);
        if(!read(at,&lists[field],8)||!read(at+8,&flags[field],4)||(flags[field]&0xffffff)>512||((flags[field]&0xffffff)&&!lists[field]))return false;
        for(unsigned i=0;i<(flags[field]&0xffffff);++i){
            std::uintptr_t candidate=0;unsigned id=0;
            if(!read(lists[field]+i*8,&candidate,8)||!candidate||!read(candidate+0x158,&id,4))return false;
            if(id==s.handle){if(s.instance&&s.instance!=candidate)return false;s.instance=candidate;}
        }
    }
    std::uintptr_t declaration=0,scene=0,name=0,subName=0,bindings=0;unsigned scenePhase=0,bindingFlags=0;
    if(!s.instance||!read(s.instance+0x27c,&scenePhase,4)||scenePhase!=3||!read(s.instance,&declaration,8)||
       !read(declaration,&vt,8)||vt!=base+0x1affbc8||!read(declaration+8,&name,8)||!worldIdentityString(read,name,s.asset)||
       !(s.variant=nativeCarryVariant(s.asset,observeHighThrow))||!read(s.instance+8,&scene,8)||!read(scene,&vt,8)||vt!=base+0x1b00198||
       !read(scene+0x28,&s.durationMs,4)||!read(scene+0x30,&subName,8)||!worldIdentityString(read,subName,s.subscene)||
       !read(s.instance+0x160,&s.timeMs,4)||!read(s.instance+0x10,&bindings,8)||!bindings||
       !read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)<3||(bindingFlags&0xffffff)>4)return false;
    constexpr unsigned durations[]={0,1000,3000,1600,1000,666,1000,0,666};
    if(s.durationMs!=durations[s.variant])return false;
    if(s.variant<=3){if(s.stateId!=45||s.phase!=(s.variant==1?2u:3u))return false;}
    else if(s.stateId!=46||s.phase!=(s.variant==4?0u:s.variant==5?4u:s.variant==8?2u:1u))return false;
    if(s.variant==8&&(s.stance!=0||s.timeMs>s.durationMs||
       !read(s.statePointer+0x50,s.releaseFlags.data(),s.releaseFlags.size())||
       s.releaseFlags!=std::array<unsigned char,3>{1,0,1}))return false;
    std::uintptr_t throwDecl=0,camera=0;std::array<float,3> direction{};
    if(s.variant==8){
        float strength=0;std::uintptr_t owner=0;float length=0;
        if(!read(s.statePointer+0x18,&throwDecl,8)||!throwDecl||!read(throwDecl,&vt,8)||vt!=base+0x1c78f98||
           !read(throwDecl+0xa0,&strength,4)||strength!=4.f||
           !read(states.component+0x310,&camera,8)||!camera||!read(camera,&vt,8)||vt!=base+0x1c72218||
           !read(camera+0x10,&owner,8)||owner!=states.player.player||!read(camera+0x25288,direction.data(),12))return false;
        for(unsigned i=0;i<3;++i){if(!std::isfinite(direction[i]))return false;length+=direction[i]*direction[i];s.throwVelocity[i]=strength*direction[i];}
        if(length<.99f||length>1.01f)return false;
    }
    unsigned playerCount=0,corpseCount=0,armCount=0,sideBits=0;std::array<NativePlayerArm,2> arms{};
    std::array<std::uintptr_t,4> actors{};
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i){
        unsigned handle=0;
        if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handle,4)||worldIdentityEntity(read,game,handle)!=actors[i])return false;
        if(actors[i]==states.player.player){if(++playerCount>1||handle!=1)return false;}
        else if(actors[i]==s.corpse){if(++corpseCount>1||handle!=s.corpseHandle)return false;}
        else{
            NativePlayerArm arm{};
            if(armCount>=2||!readNativePlayerArm(read,base,game,handle,arm)||arm.actor!=actors[i]||arm.player!=states.player.player||
               (sideBits&(1u<<arm.side))||(s.character&&s.character!=arm.character))return false;
            arms[armCount++]=arm;sideBits|=1u<<arm.side;s.character=arm.character;
        }
    }
    if(playerCount!=1||corpseCount!=1||!(sideBits&4)||armCount!=(s.variant==1?2u:1u))return false;
    // Loop/walk Scene_1 is Emily; Scene_2 is Corvo. One-shots use Scene_1.
    if(s.subscene!=((s.variant==2||s.variant==3)&&s.character==1?"Scene_2":"Scene_1"))return false;
    std::uintptr_t again=0;unsigned number=0;
    if(!read(base+0x39a5290,&again,8)||again!=game||worldIdentityEntity(read,game,1)!=states.player.player||
       worldIdentityEntity(read,game,s.corpseHandle)!=s.corpse||!read(s.corpse,&again,8)||again!=base+0x1c5e108||
       !read(s.statePointer+0x24,&number,4)||number!=s.handle||!read(s.statePointer+corpseOffset,&number,4)||number!=s.corpseHandle||
       !read(s.statePointer+phaseOffset,&number,4)||number!=s.phase||!read(s.instance+0x158,&number,4)||number!=s.handle||
       !read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance,&again,8)||again!=declaration||!read(s.instance+8,&again,8)||again!=scene||
       !read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags)return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;}
    for(unsigned i=0;i<(bindingFlags&0xffffff);++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;
    for(unsigned i=0;i<armCount;++i){NativePlayerArm finalArm{};if(!readNativePlayerArm(read,base,game,arms[i].handle,finalArm)||!sameNativePlayerArm(arms[i],finalArm))return false;}
    NativePlayerStates last{};unsigned id=0;std::uintptr_t statePointer=0;
    if(!readNativePlayerStates(read,base,last)||last.player.game!=game||last.player.player!=states.player.player||
       last.component!=states.component||last.mode!=states.mode||last.player.current!=s.stance||
       !nativePlayerCarryStates(last,base,id,statePointer)||id!=s.stateId||statePointer!=s.statePointer)return false;
    if(s.variant==8){
        std::array<unsigned char,3> flagsAgain{};unsigned clockAgain=0;
        std::array<float,3> directionAgain{};float strength=0;
        if(last.active.size()!=states.active.size()||
           !read(s.statePointer+0x50,flagsAgain.data(),flagsAgain.size())||flagsAgain!=s.releaseFlags||
           !read(s.instance+0x160,&clockAgain,4)||clockAgain<s.timeMs||clockAgain>s.durationMs||
           !read(s.statePointer+0x18,&again,8)||again!=throwDecl||!read(throwDecl,&again,8)||again!=base+0x1c78f98||
           !read(throwDecl+0xa0,&strength,4)||strength!=4.f||!read(states.component+0x310,&again,8)||again!=camera||
           !read(camera,&again,8)||again!=base+0x1c72218||!read(camera+0x10,&again,8)||again!=states.player.player||
           !read(camera+0x25288,directionAgain.data(),12)||directionAgain!=direction)return false;
        for(unsigned i=0;i<last.active.size();++i){
            const auto& a=states.active[i];const auto& b=last.active[i];
            if(a.id!=b.id||a.pointer!=b.pointer||a.vtable!=b.vtable)return false;
        }
    }
    // Preserve the native source clock; consumers can modulo only looping clips.
    out=std::move(s);return true;
}

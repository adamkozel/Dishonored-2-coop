#pragma once
#include "native_player_states.h"
#include "native_player_arm.h"
#include <array>
struct NativePlayerKeyholeScene {
    std::uintptr_t instance=0,statePointer=0,door=0;
    unsigned handle=0,doorHandle=0,variant=0,timeMs=0,durationMs=0,character=0,endStance=0;
    NativePlayerArm rightArm{},leftArm{};
    std::string asset,subscene;
};
// Exact body + right arm + left arm + door-child scene, observed on both
// characters and both sides. Instance+15c is cumulative; +160 is phase time.
// No scene/event execution and no camera/position changes occur here.
inline unsigned nativeKeyholeVariant(const std::string& asset,const std::string& sub,unsigned duration,unsigned crouch,unsigned side){
    if(crouch>1||side>1)return 0;
    const std::string direction=side?"right":"left",prefix="Keyhole_"+direction;
    if(asset!="midnight/gameplay/player/base/interaction/keyhole_"+direction+".midnightscene")return 0;
    const unsigned offset=side?5:0;
    if(duration==1500&&sub==(crouch?"Sneak_":"")+prefix+"_in")return offset+(crouch?10:7);
    if(duration==500&&sub==prefix+"_loop")return offset+8;
    if(duration==1000&&sub==(crouch?"Sneak_":"")+prefix+"_out")return offset+(crouch?11:9);
    return 0;
}
template<class Read> bool readNativePlayerKeyholeScene(Read read,std::uintptr_t base,const NativePlayerStates& states,NativePlayerKeyholeScene& out,unsigned* stage=nullptr){
    NativePlayerKeyholeScene s{};
    if(stage)*stage=1;
    if(states.player.logic!=1||!states.defaultMode||states.active.size()!=1||states.active[0].id!=24||states.active[0].vtable!=base+0x1c7c4a8)return false;
    if(stage)*stage=2;s.statePointer=states.active[0].pointer;unsigned char crouch=0,side=0;
    if(!read(s.statePointer+0x24,&s.handle,4)||!s.handle||!read(s.statePointer+0x1f0,&s.doorHandle,4)||!s.doorHandle||
       !read(s.statePointer+0x221,&crouch,1)||crouch>1||!read(s.statePointer+0x223,&side,1)||side>1)return false;
    s.endStance=crouch;const auto game=states.player.game,manager=game+0x1f5338;s.door=worldIdentityEntity(read,game,s.doorHandle);if(!s.door)return false;
    if(stage)*stage=3;std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    for(unsigned f=0;f<2;++f){const auto at=manager+(f?0x610:0);
        if(!read(at,&lists[f],8)||!read(at+8,&flags[f],4)||(flags[f]&0xffffff)>512||((flags[f]&0xffffff)&&!lists[f]))return false;
        for(unsigned i=0;i<(flags[f]&0xffffff);++i){std::uintptr_t c=0;unsigned id=0;if(!read(lists[f]+i*8,&c,8)||!c||!read(c+0x158,&id,4))return false;
            if(id==s.handle){if(s.instance&&s.instance!=c)return false;s.instance=c;}}
    }
    if(stage)*stage=4;std::uintptr_t declaration=0,scene=0,vt=0,name=0,subName=0,bindings=0;unsigned phase=0,bindingFlags=0,total=0,local=0;
    if(!s.instance||!read(s.instance+0x27c,&phase,4)||phase!=3||!read(s.instance,&declaration,8)||!read(declaration,&vt,8)||vt!=base+0x1affbc8||
       !read(declaration+8,&name,8)||!worldIdentityString(read,name,s.asset)||!read(s.instance+8,&scene,8)||!read(scene,&vt,8)||vt!=base+0x1b00198||
       !read(scene+0x28,&s.durationMs,4)||!read(scene+0x30,&subName,8)||!worldIdentityString(read,subName,s.subscene))return false;
    if(stage)*stage=5;s.variant=nativeKeyholeVariant(s.asset,s.subscene,s.durationMs,crouch,side);if(!s.variant)return false;
    if(stage)*stage=6;if(!read(s.instance+0x15c,&total,4)||!read(s.instance+0x160,&local,4)||total>3600000||local>s.durationMs||
       !read(s.instance+0x10,&bindings,8)||!bindings||!read(s.instance+0x18,&bindingFlags,4)||(bindingFlags&0xffffff)!=4)return false;
    const bool loop=s.variant==8||s.variant==13;
    if(loop&&total<1500)return false;s.timeMs=loop?total-1500:local;
    if(stage)*stage=7;std::array<std::uintptr_t,4> actors{};std::array<unsigned,4> handles{};NativePlayerArm right{},left{};
    for(unsigned i=0;i<4;++i)if(!read(bindings+i*0x20+8,&actors[i],8)||!actors[i]||!read(actors[i]+0xa8,&handles[i],4)||worldIdentityEntity(read,game,handles[i])!=actors[i])return false;
    if(stage)*stage=8;if(actors[0]!=states.player.player||handles[0]!=1||!readNativePlayerArm(read,base,game,handles[1],right)||right.actor!=actors[1]||right.side!=2||
       !readNativePlayerArm(read,base,game,handles[2],left)||left.actor!=actors[2]||left.side!=1||left.character!=right.character)return false;
    if(stage)*stage=9;s.character=right.character;unsigned parent=0;
    // ArkDoor's spawned scene child has a generation-checked parent handle;
    // it is not in the FP arms' reciprocal +b0 child array (observed empty).
    if(!read(actors[3]+0xac,&parent,4)||parent!=s.doorHandle||!read(s.door,&vt,8)||vt!=base+0x1cb7c68)return false;
    if(stage)*stage=11;std::uintptr_t again=0;unsigned number=0;unsigned char byte=0;std::string text;
    if(!read(s.instance+0x158,&number,4)||number!=s.handle||!read(s.instance+0x27c,&number,4)||number!=3||!read(s.instance,&again,8)||again!=declaration||
       !read(declaration,&again,8)||again!=base+0x1affbc8||!read(declaration+8,&again,8)||again!=name||!worldIdentityString(read,name,text)||text!=s.asset||
       !read(s.instance+8,&again,8)||again!=scene||!read(scene,&again,8)||again!=base+0x1b00198||!read(scene+0x28,&number,4)||number!=s.durationMs||
       !read(scene+0x30,&again,8)||again!=subName||!worldIdentityString(read,subName,text)||text!=s.subscene||
       !read(s.instance+0x10,&again,8)||again!=bindings||!read(s.instance+0x18,&number,4)||number!=bindingFlags||
       !read(s.statePointer+0x24,&number,4)||number!=s.handle||!read(s.statePointer+0x1f0,&number,4)||number!=s.doorHandle||
       !read(s.statePointer+0x221,&byte,1)||byte!=crouch||!read(s.statePointer+0x223,&byte,1)||byte!=side||
       !read(actors[3]+0xac,&number,4)||number!=parent||!read(s.door,&again,8)||again!=base+0x1cb7c68)return false;
    for(unsigned i=0;i<4;++i)if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i]||worldIdentityEntity(read,game,handles[i])!=actors[i])return false;
    for(unsigned i=0;i<2;++i){const auto at=manager+(i?0x610:0);if(!read(at,&again,8)||again!=lists[i]||!read(at+8,&number,4)||number!=flags[i])return false;}
    if(stage)*stage=12;NativePlayerStates last{};NativePlayerArm arm{};
    if(!readNativePlayerStates(read,base,last)||last.player.game!=game||last.player.player!=states.player.player||last.component!=states.component||last.mode!=states.mode||
       !last.defaultMode||last.active.size()!=1||last.active[0].id!=24||last.active[0].pointer!=s.statePointer||last.active[0].vtable!=base+0x1c7c4a8||
       !readNativePlayerArm(read,base,game,right.handle,arm)||!sameNativePlayerArm(right,arm)||!readNativePlayerArm(read,base,game,left.handle,arm)||!sameNativePlayerArm(left,arm)||
       worldIdentityEntity(read,game,s.doorHandle)!=s.door)return false;
    s.rightArm=right;s.leftArm=left;out=std::move(s);return true;
}

// Native Peeking out-scene retires before ID24, leaving a roughly 200ms
// listener/state-machine handoff (observed on holstered crouch exits).
// Hold only the confirmed exit endpoint for a bounded 400ms after its native
// completion time. A present-but-invalid scene, changed owner/door/arms,
// unrelated state or expired deadline cannot use this continuation.
struct NativeKeyholeHandoff {
    NativePlayerStates owner{};NativePlayerKeyholeScene scene{};
    std::uint64_t deadline=0,cancelDeadline=0;
    void observe(const NativePlayerStates& s,const NativePlayerKeyholeScene& k,std::uint64_t now){
        owner=s;scene=k;cancelDeadline=now+400;
        deadline=0;
        if((k.variant==9||k.variant==11||k.variant==14||k.variant==16)&&k.durationMs==1000&&k.timeMs>=800&&k.timeMs<=1000){
            owner=s;scene=k;deadline=now+(1000-k.timeMs)+400;
        }
    }
};
template<class Read> bool readNativeKeyholeHandoff(Read read,std::uintptr_t base,const NativePlayerStates& s,const NativeKeyholeHandoff& h,std::uint64_t now,NativePlayerKeyholeScene& out,bool cancelled=false){
    const auto deadline=cancelled?h.cancelDeadline:h.deadline;
    // Early B cancellation clears +24 immediately and leaves phase 3 for
    // about 200ms. Preserve the last confirmed pose during that native
    // handoff; do not invent an out scene or restart its clock.
    if(!deadline||now>deadline||deadline-now>400||s.player.logic!=1||!s.defaultMode||s.active.size()!=1||
       s.active[0].id!=24||s.active[0].vtable!=base+0x1c7c4a8||s.active[0].pointer!=h.scene.statePointer||
       s.player.game!=h.owner.player.game||s.player.player!=h.owner.player.player||s.player.physics!=h.owner.player.physics||
       s.component!=h.owner.component||s.mode!=h.owner.mode)return false;
    const auto state=h.scene.statePointer,game=s.player.game,manager=game+0x1f5338;
    unsigned handle=0,door=0,phase=0;unsigned char crouch=0,side=0;std::uintptr_t vt=0;
    if(!read(state+0x24,&handle,4)||handle!=(cancelled?0u:h.scene.handle)||!read(state+0x1f0,&door,4)||door!=h.scene.doorHandle||
       !read(state+0xec,&phase,4)||(cancelled?phase!=3:(phase!=0&&phase!=3))||!read(state+0x221,&crouch,1)||crouch!=h.scene.endStance||
       !read(state+0x223,&side,1)||side!=(h.scene.variant>=12?1:0)||worldIdentityEntity(read,game,door)!=h.scene.door||
       !read(h.scene.door,&vt,8)||vt!=base+0x1cb7c68)return false;
    std::array<std::uintptr_t,2> lists{};std::array<unsigned,2> flags{};
    for(unsigned f=0;f<2;++f){const auto at=manager+(f?0x610:0);
        if(!read(at,&lists[f],8)||!read(at+8,&flags[f],4)||(flags[f]&0xffffff)>512||((flags[f]&0xffffff)&&!lists[f]))return false;
        for(unsigned i=0;i<(flags[f]&0xffffff);++i){std::uintptr_t instance=0;unsigned id=0;
            if(!read(lists[f]+i*8,&instance,8)||!instance||!read(instance+0x158,&id,4)||id==h.scene.handle)return false;
        }
    }
    NativePlayerArm arm{};
    if(!readNativePlayerArm(read,base,game,h.scene.rightArm.handle,arm)||!sameNativePlayerArm(arm,h.scene.rightArm)||
       !readNativePlayerArm(read,base,game,h.scene.leftArm.handle,arm)||!sameNativePlayerArm(arm,h.scene.leftArm))return false;
    NativePlayerStates last{};unsigned number=0;unsigned char byte=0;std::uintptr_t again=0;
    if(!readNativePlayerStates(read,base,last)||last.player.game!=game||last.player.player!=s.player.player||last.component!=s.component||last.mode!=s.mode||
       !last.defaultMode||last.active.size()!=1||last.active[0].id!=24||last.active[0].pointer!=state||last.active[0].vtable!=base+0x1c7c4a8||
       !read(state+0x24,&number,4)||number!=handle||!read(state+0x1f0,&number,4)||number!=door||!read(state+0xec,&number,4)||number!=phase||
       !read(state+0x221,&byte,1)||byte!=crouch||!read(state+0x223,&byte,1)||byte!=side||worldIdentityEntity(read,game,door)!=h.scene.door||
       !read(h.scene.door,&again,8)||again!=base+0x1cb7c68)return false;
    for(unsigned f=0;f<2;++f){const auto at=manager+(f?0x610:0);if(!read(at,&again,8)||again!=lists[f]||!read(at+8,&number,4)||number!=flags[f])return false;}
    out=h.scene;if(!cancelled)out.timeMs=1000;return true;
}

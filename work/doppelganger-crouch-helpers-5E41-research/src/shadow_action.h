#pragma once
// Bounded developer-only native AnimBasic playback on the owned clone.
// All clip data, rigs and playback routines are native. The isolated graph
// changes the hitReact blend from additive to a gated full-body LERP.
struct ShadowActionState {
    std::uintptr_t game=0,actor=0,anim=0,blend=0;
    std::uint32_t handle=0;
    std::uint64_t epoch=0,remoteEpoch=0,session=0;
    ULONGLONG start=0,until=0;
    bool holdLastFrame=false;
    bool crouchPose=false;
};
ShadowActionState shadowAction{};
std::atomic<std::uintptr_t> shadowActionAnim{0},shadowActionBlend{0};
std::atomic<ULONGLONG> shadowActionDeadline{0};
std::uintptr_t shadowActionParameter(std::uintptr_t anim) {
    return shadowActionAnim.load(std::memory_order_acquire)==anim&&GetTickCount64()<shadowActionDeadline.load()?shadowActionBlend.load():0;
}
bool shadowActionApiMatches(){
    const unsigned char play[]={0x48,0x83,0xec,0x68,0x41,0x80,0xf8,0xff};
    const unsigned char stop[]={0x48,0x83,0xec,0x68,0x48,0x63,0xc2,0x48};
    return !std::memcmp(gameImage+0x53eef0,play,sizeof(play))&&!std::memcmp(gameImage+0x540a00,stop,sizeof(stop));
}
bool shadowActionSameActor(){
    std::uintptr_t current=0,owner=0,vt=0;
    return shadowAction.actor&&shadowAction.epoch==outgoing.worldEpoch&&shadowAction.game==observedGame&&
        memoryRead(reinterpret_cast<std::uintptr_t>(gameImage)+0x39a5290,&current,8)&&current==shadowAction.game&&
        worldIdentityEntity(memoryRead,current,shadowAction.handle)==shadowAction.actor&&
        memoryRead(shadowAction.anim+0x10,&owner,8)&&owner==shadowAction.actor&&
        memoryRead(shadowAction.anim,&vt,8)&&vt==reinterpret_cast<std::uintptr_t>(gameImage)+0x1b80408;
}
void stopShadowAction(const char* reason){
    if(!shadowAction.actor)return;
    shadowActionDeadline.store(0);shadowActionAnim.store(0,std::memory_order_release);
    if(shadowActionSameActor()&&shadowActionApiMatches())
        reinterpret_cast<bool(*)(void*,int)>(gameImage+0x540a00)(reinterpret_cast<void*>(shadowAction.anim),0);
    consoleOutput("COOP: shadow action ended %s\n",reason);shadowAction={};
}
void serviceShadowAction(ULONGLONG tick){
    if(!shadowAction.actor)return;
    if(tick>=shadowAction.until||!shadowActionSameActor()||!avatarAiHeld||!probeFollow||!locomotionEnabled||
       !coopGameplayReady(outgoing)||!coopGameplayReady(remote)||!lastReceive||tick-lastReceive>=250||
       shadowAction.remoteEpoch!=remote.worldEpoch||shadowAction.session!=remote.session){stopShadowAction("expired_or_scope_changed");return;}
    if(!shadowAction.holdLastFrame&&tick-shadowAction.start>150&&!reinterpret_cast<bool(*)(void*,int)>(gameImage+0x53e770)(reinterpret_cast<void*>(shadowAction.anim),0))
        stopShadowAction("native_clip_finished");
}
std::uintptr_t shadowNamedInput(std::uintptr_t anim,const char* expected,unsigned expectedType) {
    std::uintptr_t wrapper=anim+0x338,tree=0,data=0,decl=0,buckets=0;std::uint32_t mask=0;
    if(!memoryRead(wrapper,&tree,8)||!tree||!memoryRead(tree,&data,8)||!data||
       !memoryRead(wrapper+8,&decl,8)||!decl||!memoryRead(decl+0x70,&buckets,8)||!buckets||
       !memoryRead(decl+0x80,&mask,4)||mask>4095)return 0;
    for(unsigned bucket=0;bucket<=mask;++bucket){std::uintptr_t entry=0;if(!memoryRead(buckets+bucket*8,&entry,8))return 0;
        for(unsigned depth=0;entry&&depth<128;++depth){std::uintptr_t name=0,next=0;unsigned type=0;int offset=-1;std::string text;
            if(!memoryRead(entry,&name,8)||!worldIdentityString(memoryRead,name,text)||!memoryRead(entry+8,&type,4)||
               !memoryRead(entry+12,&offset,4)||!memoryRead(entry+0x970,&next,8)||offset<0||offset>0x100000)return 0;
            if(text==expected&&type==expectedType)return data+offset;
            entry=next;
        }
    }return 0;
}
bool playShadowAction(const char* action){
    if(!std::strcmp(action,"off")){stopShadowAction("command");return true;}
    if(bodyStreamEnabled||shadowAction.actor||!shadowActionApiMatches()||!avatarAiHeld||!probeFollow||!locomotionEnabled||
       !coopGameplayReady(outgoing)||!coopGameplayReady(remote)||!lastReceive||GetTickCount64()-lastReceive>=250)return false;
    const char* alias=!std::strcmp(action,"attack")?"sword_attack_fasta":!std::strcmp(action,"parry")?"sword_parry_ll_in":
        !std::strcmp(action,"recover")?"sword_parry_ll_out":!std::strcmp(action,"idle")?"sword_idle":
        !std::strcmp(action,"airborne")?"sword_fall_loop":!std::strcmp(action,"land")?"sword_land_walk_out":
        (!std::strcmp(action,"crouchref")||!std::strcmp(action,"crouchpose"))?"sword_patrol_distraction_search_undertable":nullptr;
    if(!alias)return false;
    NativeSwordEquipment sword{};
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,sword,
       localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||sword.actor!=probeActor||!sword.equipped||!sword.started)return false;
    std::uintptr_t list=0,anim=0,render=0,model=0,basic=0,items=0,name=0;unsigned count=0;std::string text;
    if(!memoryRead(probeActor+0x268,&list,8)||!list||!memoryRead(probeActor+0x270,&count,4)||(count&0xffffff)>256)return false;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0,vt=0;if(!memoryRead(list+i*8,&c,8)||!c||!memoryRead(c,&vt,8))return false;if(vt==reinterpret_cast<std::uintptr_t>(gameImage)+0x1b80408)anim=c;}
    if(!anim||!memoryRead(anim+0x38,&render,8)||!render||!memoryRead(render+0x1fe8,&model,8)||!model||
       !memoryRead(model+8,&name,8)||!worldIdentityString(memoryRead,name,text)||
       text!=(localRole?"models/characters/d2coop/shadow_corvo.md6":"models/characters/d2coop/shadow_emily.md6")||
       !memoryRead(model+0x148,&basic,8)||!basic||!memoryRead(basic+8,&name,8)||!worldIdentityString(memoryRead,name,text)||
       text!="models/characters/d2coop/shadow_actions.animbasic"||!memoryRead(basic+0x58,&items,8)||!items||
       !memoryRead(basic+0x60,&count,4)||(count&0xffffff)!=12)return false;
    int index=-1;
    for(unsigned i=0;i<(count&0xffffff);++i){if(!memoryRead(items+i*16,&name,8)||!worldIdentityString(memoryRead,name,text))return false;if(text==alias)index=i;}
    const auto blend=shadowNamedInput(anim,"coopFullBodyWeight",1);
    const auto input=shadowNamedInput(anim,"animBasicInputData1",12);
    std::uintptr_t descriptor=0,tree=0,data=0;unsigned type=0;int offset=-1;
    if(index<5||index>11||!blend||!input||!memoryRead(anim+0x390,&descriptor,8)||!descriptor||
       !memoryRead(descriptor,&type,4)||type!=12||!memoryRead(descriptor+4,&offset,4)||offset<0||offset>0x100000||
       !memoryRead(anim+0x338,&tree,8)||!tree||!memoryRead(tree,&data,8)||!data||data+offset!=input)return false;
    const bool poseReference=!std::strcmp(action,"crouchpose");
    // durationTime_t is float seconds (6e4af0 -> 6e4850), not integer milliseconds.
    // The native node copies these bits directly into the clip's current time.
    if(!reinterpret_cast<bool(*)(void*,int,signed char,float,int,float,float,bool)>(gameImage+0x53eef0)(
       reinterpret_cast<void*>(anim),0,static_cast<signed char>(index),1.f,poseReference?1:index==6?1:index==9?2:0,poseReference?0.f:1.f,poseReference?1.2f:0.f,false))return false;
    const auto tick=GetTickCount64();
    const auto deadline=tick+(index==11?10000:3500);
    shadowAction={probeGame,probeActor,anim,blend,probeHandle,outgoing.worldEpoch,remote.worldEpoch,remote.session,tick,deadline,poseReference||index==6||index==9};
    shadowAction.crouchPose=poseReference;
    shadowActionBlend.store(blend);shadowActionDeadline.store(deadline);shadowActionAnim.store(anim,std::memory_order_release);
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(sword.entity));
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(sword.entity));
    consoleOutput("COOP: shadow action %s index=%d handle=%08x started\n",alias,index,probeHandle);return true;
}
bool captureNativeCrouch(){
    const auto tick=GetTickCount64();
    if(bodyStreamEnabled||!shadowAction.crouchPose||!shadowActionSameActor()||tick-shadowAction.start<600||
       tick>=shadowAction.until||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||!lastReceive||tick-lastReceive>=250)return false;
    NativeSkeleton s{};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s)||!s.stable||
       !bodyShadowTarget(s)||s.actor!=shadowAction.actor||s.animation!=shadowAction.anim||s.globalPose[2][14]>.75f)return false;
    NativeCrouchReference c{};c.game=s.game;c.actor=s.actor;c.skeleton=s.skeleton;c.handle=probeHandle;
    c.epoch=outgoing.worldEpoch;c.peerSession=remote.session;c.peerEpoch=remote.worldEpoch;c.local.resize(s.localPose.size());
    for(unsigned j=0;j<c.local.size();++j){std::memcpy(c.local[j].v,s.localPose[j].data(),64);if(!bodyRigid(c.local[j]))return false;}
    clearBodyProbe();nativeCrouchReference=std::move(c);return true;
}

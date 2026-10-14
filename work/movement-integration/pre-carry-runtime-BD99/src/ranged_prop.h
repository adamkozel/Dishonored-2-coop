#pragma once
#include "ranged_prop_calibration.h"
#include "ranged_hand_tracks.h"
#include "ranged_mechanism.h"
// Local, finite cosmetic attachment experiment. These custom idEntity assets
// contain only native rendering/animation, never inventory, firing or pickups.
// At most one prop of each kind per isolated avatar/world. The parked prop
// must be hidden before switching; reuse it and retire both on world changes.
struct RangedProp {
    std::uintptr_t game=0,actor=0,parent=0,definition=0,frame=0;
    unsigned handle=0,parentHandle=0,kind=0;std::uint16_t joint=0xffff;
    std::uint64_t epoch=0;ULONGLONG until=0;bool pendingHide=false,network=false;const char* tag="";
} rangedProp,rangedPropParked;
unsigned rangedPropRequest=0;
std::uint64_t rangedPropCreated=0,rangedPropAttached=0,rangedPropHidden=0,rangedPropRefused=0,rangedPropRetired=0;
std::uint64_t rangedPropSwitched=0,rangedPropReused=0;
const char* rangedPropResult="idle";
const char* rangedPropDefinition(unsigned kind){
    if(kind==1)return localRole?"models/weapons/d2coop/ranged_prop_corvo_gun.def":"models/weapons/d2coop/ranged_prop_emily_gun.def";
    if(kind==2)return localRole?"models/weapons/d2coop/ranged_prop_corvo_crossbow.def":"models/weapons/d2coop/ranged_prop_emily_crossbow.def";
    return "";
}
bool rangedPropApiMatches(){
    const unsigned char bind[]={0x48,0x89,0x5c,0x24,0x08,0x48,0x89,0x6c,0x24,0x10,0x48,0x89,0x74,0x24,0x18,0x57};
    const unsigned char tag[]={0x48,0x8b,0xc4,0x48,0x89,0x58,0x10,0x48,0x89,0x68,0x18,0x48,0x89,0x70,0x20,0x57};
    const unsigned char create[]={0x40,0x57,0x48,0x83,0xec,0x30,0x48,0xc7,0x44,0x24,0x20,0xfe,0xff,0xff,0xff,0x48};
    const unsigned char init[]={0x40,0x55,0x56,0x57,0x48,0x83,0xec,0x30,0x48,0xc7,0x44,0x24,0x20,0xfe,0xff,0xff};
    const unsigned char hide[]={0x0f,0xb6,0x81,0xd0,0,0,0,0xa8,1,0x75,0x0d,0x0c,1};
    const unsigned char show[]={0x0f,0xb6,0x81,0xd0,0,0,0,0xa8,1,0x74,0x0d,0x24,0xfe};
    return gameImage&&!std::memcmp(gameImage+0x875e80,bind,sizeof(bind))&&!std::memcmp(gameImage+0x879930,tag,sizeof(tag))&&
        !std::memcmp(gameImage+0x8a78f0,create,sizeof(create))&&!std::memcmp(gameImage+0x8aecc0,init,sizeof(init))&&
        !std::memcmp(gameImage+0x8784b0,hide,sizeof(hide))&&!std::memcmp(gameImage+0x87c590,show,sizeof(show));
}
bool rangedPropIdentityFor(const RangedProp& p,bool attached){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,d=0,n=0,list=0,c=0,owner=0;unsigned count=0;
    std::string name;
    if(!p.actor||p.game!=observedGame||p.epoch!=outgoing.worldEpoch||worldIdentityEntity(memoryRead,p.game,p.handle)!=p.actor||
       !memoryRead(p.actor,&vt,8)||vt!=base+0x1c87d58||!memoryRead(p.actor+0x2e8,&d,8)||d!=p.definition||
       !memoryRead(d,&vt,8)||vt!=base+0x1ab58f8||!memoryRead(d+8,&n,8)||!worldIdentityString(memoryRead,n,name)||name!=rangedPropDefinition(p.kind)||
       !memoryRead(p.actor+0x268,&list,8)||!list||!memoryRead(p.actor+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>4)return false;
    unsigned anim=0,reference=0;
    for(unsigned i=0;i<(count&0xffffff);++i){
        if(!memoryRead(list+i*8,&c,8)||!c||!memoryRead(c,&vt,8)||!memoryRead(c+0x10,&owner,8)||owner!=p.actor)return false;
        if(vt==base+0x1b80408){if(++anim>1)return false;}
        else if(vt==base+0x1c573e8){if(++reference>1)return false;}
        else return false; // No weapon, physics, attention, pickup or damage components.
    }
    if(anim!=1)return false;
    if(attached){unsigned parent=0;std::uint16_t joint=0;
        if(p.parent!=probeActor||p.parentHandle!=probeHandle||worldIdentityEntity(memoryRead,p.game,p.parentHandle)!=p.parent||
           !memoryRead(p.actor+0xac,&parent,4)||parent!=p.parentHandle||!memoryRead(p.actor+0x358,&c,8)||c!=p.frame||
           !memoryRead(c,&vt,8)||vt!=base+0x1c573e8||!memoryRead(c+0x10,&owner,8)||owner!=p.actor||
           !memoryRead(c+0x3c8,&owner,8)||owner!=p.parent||!memoryRead(c+0x3d0,&joint,2)||joint!=p.joint||
           !memoryRead(c+0x3c0,&n,8)||!worldIdentityString(memoryRead,n,name)||name!=p.tag||!shadowOwnedDamageSource(p.handle))return false;
        std::uintptr_t children=0;unsigned childrenCount=0,matches=0;
        if(!memoryRead(p.parent+0xb0,&children,8)||!children||!memoryRead(p.parent+0xb8,&childrenCount,4)||(childrenCount&0xffffff)>32)return false;
        for(unsigned i=0;i<(childrenCount&0xffffff);++i){if(!memoryRead(children+i*8,&c,8))return false;if(c==p.actor)++matches;}
        if(matches!=1)return false;
    }
    return true;
}
bool rangedPropIdentity(bool attached){return rangedPropIdentityFor(rangedProp,attached);}
bool rangedPropIsHidden(const RangedProp& p){
    unsigned char flags=0;
    return p.actor&&!p.until&&!p.pendingHide&&rangedPropIdentityFor(p,false)&&memoryRead(p.actor+0xd0,&flags,1)&&(flags&1);
}
void hideRangedProp(){
    stopRangedMechanism();
    if(!rangedProp.actor||(!rangedProp.until&&!rangedProp.pendingHide))return;rangedProp.until=0;rangedProp.pendingHide=true;
    if(!coopGameplayReady(outgoing)||!rangedPropApiMatches()||!rangedPropIdentity(false))return;
    reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(rangedProp.actor));
    unsigned char flags=0;
    if(memoryRead(rangedProp.actor+0xd0,&flags,1)&&(flags&1)){rangedProp.pendingHide=false;++rangedPropHidden;rangedPropResult="native_prop_hidden";}
}
void serviceRangedProp(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    unsigned request=rangedPropRequest;rangedPropRequest=0;
    unsigned presentedKind=0,presentedVariant=0,presentedMechanism=0;float presentedAmount=0;
    AcquireSRWLockShared(&movementLock);const auto& movement=movementPlayback;
    if(movement.end>tick&&movement.lease>tick&&movement.bank.hasRanged()&&movement.clock.weight>.5f&&movement.rangedWeight>.1f&&
       movement.actor==probeActor&&movement.game==observedGame&&movement.epoch==outgoing.worldEpoch&&
       shadowStreamCurrent(tick)&&shadowStream.peerReady&&shadowStream.lastState&&tick-shadowStream.lastState<250){
        presentedKind=movement.rangedKind;presentedVariant=movement.rangedVariant;presentedAmount=movement.rangedAmount;presentedMechanism=movement.rangedMechanism;
    }
    ReleaseSRWLockShared(&movementLock);
    const bool automatic=presentedKind>=1&&presentedKind<=2&&!request;
    for(auto* p:{&rangedProp,&rangedPropParked})if(p->actor&&(p->game!=observedGame||p->epoch!=outgoing.worldEpoch)){
        if(p==&rangedProp)stopRangedMechanism();*p={};++rangedPropRetired;rangedPropResult="world_retired";
    }
    if(rangedProp.network&&!automatic)hideRangedProp();
    // Hide the previous kind immediately on a source-confirmed switch. Never
    // display both children during the old 250ms network lease.
    if(rangedProp.actor&&((automatic&&rangedProp.kind!=presentedKind)||
       ((request==1||request==2)&&rangedProp.kind!=request)))hideRangedProp();
    if(automatic&&rangedProp.network&&rangedProp.until&&rangedProp.kind==presentedKind)rangedProp.until=tick+250;
    if(request==3||(rangedProp.until&&(tick>=rangedProp.until||!shadowEquipmentAiReady()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote))))hideRangedProp();
    if(rangedProp.pendingHide)hideRangedProp();
    if(automatic&&!rangedProp.until&&!rangedProp.pendingHide)request=presentedKind;
    if(automatic&&rangedProp.network&&rangedProp.until&&rangedProp.kind==presentedKind){
        if(!rangedPropIdentity(true)||!rangedPropApiMatches()){hideRangedProp();++rangedPropRefused;rangedPropResult="network_attachment_changed";return;}
        const auto slot=presentedVariant?RangedPresentation::slot(presentedVariant):0;
        const auto& track=rangedHandTracks[localRole?0:1][presentedKind-1][slot];
        const float f=(std::max)(0.f,(std::min)(1.f,presentedAmount))*119;const auto a=unsigned(f),b=(std::min)(119u,a+1);
        MovementBank::Joint x{},y{};std::copy(track[a],track[a]+7,x.begin());std::copy(track[b],track[b]+7,y.begin());
        const auto q=MovementBank::blend(x,y,f-a);std::array<float,12> qts{};std::copy(q.begin(),q.end(),qts.begin());qts[8]=qts[9]=qts[10]=1;
        BodyMatrix m{};if(!bodyFromQts(qts,m)){hideRangedProp();++rangedPropRefused;return;}
        float offset[3]{m.v[12],m.v[13],m.v[14]},axis[9]{};for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)axis[c*3+r]=m.v[c*4+r];
        // Reuse native bind/update; only this exact cosmetic child is touched.
        reinterpret_cast<void(*)(void*,void*,const char**,const float*,const float*,bool)>(gameImage+0x875e80)(reinterpret_cast<void*>(rangedProp.parent),reinterpret_cast<void*>(rangedProp.actor),&rangedProp.tag,offset,axis,true);
        if(!rangedPropIdentity(true)){hideRangedProp();++rangedPropRefused;rangedPropResult="updated_attachment_unconfirmed";}
        else if(rangedProp.kind==2&&!publishRangedMechanism(rangedProp.handle,presentedVariant,presentedAmount,presentedMechanism,rangedProp.until)){
            stopRangedMechanism();++rangedMechanismRefused;
        }
        return;
    }
    if(request!=1&&request!=2)return;
    const auto refuse=[&](const char* reason){++rangedPropRefused;rangedPropResult=reason;};
    if(rangedProp.until||rangedProp.pendingHide||!partnerAssetsEnabled||!probeDoppelganger||std::strcmp(mapName,kGuardMap)||!rangedPropApiMatches()||
       !coopGameplayReady(outgoing)||!coopGameplayReady(remote)||!shadowEquipmentAiReady()||
       (!automatic&&(!observerIsolation.ready||!observerIsolation.movementFixture||observerIsolation.until<tick+16000))){refuse("unsupported_scope");return;}
    if(rangedProp.actor&&rangedProp.kind!=request){
        if(!rangedPropIdentity(true)||!rangedPropIsHidden(rangedProp)){
            refuse("previous_prop_not_safely_parked");return;
        }
        if(rangedPropParked.actor&&(rangedPropParked.kind!=request||
           !rangedPropIdentityFor(rangedPropParked,true)||!rangedPropIsHidden(rangedPropParked))){
            refuse("parked_prop_identity_changed");return;
        }
        const bool reused=rangedPropParked.actor!=0;
        std::swap(rangedProp,rangedPropParked);++rangedPropSwitched;
        if(reused)++rangedPropReused;
    }
    if(!rangedProp.actor){
        float pos[3]{},axis[9]{};
        if(!memoryRead(probeActor+0x300,pos,12)||!memoryRead(probeActor+0x30c,axis,36)){refuse("avatar_transform_unavailable");return;}
        for(float v:pos)if(!std::isfinite(v)){refuse("avatar_position_invalid");return;}
        for(float v:axis)if(!std::isfinite(v)){refuse("avatar_axis_invalid");return;}
        const auto definition=lookupEntityDefinition(rangedPropDefinition(request));if(!definition){refuse("visual_definition_missing");return;}
        unsigned char parameters[12]{};reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
        void* actor=reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(definition),parameters);
        if(!actor){refuse("native_creation_refused");return;}
        // Mirror the native inventory spawn initialization (619380), which
        // sets owner+AC before 8aecc0. Reference-frame binding only establishes
        // transform parenting; it does not set this damage-source ownership.
        std::uintptr_t vt=0,createdDefinition=0;unsigned owner=0;const auto a=reinterpret_cast<std::uintptr_t>(actor);
        if(!memoryRead(a,&vt,8)||vt!=reinterpret_cast<std::uintptr_t>(gameImage)+0x1c87d58||
           !memoryRead(a+0x2e8,&createdDefinition,8)||createdDefinition!=definition||!memoryRead(a+0xac,&owner,4)||owner!=0x3fff0010||
           worldIdentityEntity(memoryRead,observedGame,probeHandle)!=probeActor){refuse("new_prop_ownership_refused");return;}
        std::memcpy(reinterpret_cast<void*>(a+0xac),&probeHandle,4);
        // Initialize at the avatar root. No player entity or inventory is changed.
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(actor,pos);
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(actor,axis);
        reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),actor,-1);
        rangedProp={};auto& p=rangedProp;p.game=observedGame;p.actor=reinterpret_cast<std::uintptr_t>(actor);p.parent=probeActor;p.parentHandle=probeHandle;p.definition=definition;p.kind=request;p.epoch=outgoing.worldEpoch;
        memoryRead(p.actor+0xa8,&p.handle,4);++rangedPropCreated;
        reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(actor);
        if(!rangedPropIdentity(false)){refuse("visual_component_identity_refused");return;}
    }
    if(rangedProp.joint==0xffff){
        auto& p=rangedProp;if(!rangedPropIdentity(false)){refuse("unbound_prop_identity_changed");return;}
        const char* tag="left_wrist_jnt";float offset[3]{},axis[9]{};const std::uint16_t joint=9;
        if(!rangedPropIdleCalibration(localRole?1u:2u,p.kind,offset,axis)){refuse("hand_calibration_unavailable");return;}
        {
            // Native equipment bind also supports a raw joint name. Check the
            // live skeleton/name table before invoking its fatal-on-miss path.
            NativeSkeleton s{};std::uintptr_t manager=0,vt=0,entries=0,namePointer=0;unsigned count=0;std::string name;
            const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
            if(!readNativeSkeleton(memoryRead,base,p.parentHandle,s)||!s.stable||s.actor!=p.parent||!bodyShadowTarget(s)||s.jointIds.size()<11||
               !memoryRead(base+0x22995c0,&manager,8)||!manager||!memoryRead(manager,&vt,8)||vt!=base+0x1b093c8||
               !memoryRead(manager+8,&entries,8)||!entries||!memoryRead(manager+0x10,&count,4)||s.jointIds[joint]>=(count&0xffffff)||
               !memoryRead(entries+std::uintptr_t(s.jointIds[joint])*72+16,&namePointer,8)||!worldIdentityString(memoryRead,namePointer,name)||name!=tag){
                refuse("native_left_hand_joint_unavailable");return;
            }
        }
        if(joint>=(localRole?215:219)){refuse("native_left_hand_joint_invalid");return;}
        p.tag=tag;
        reinterpret_cast<void(*)(void*,void*,const char**,const float*,const float*,bool)>(gameImage+0x875e80)(reinterpret_cast<void*>(p.parent),reinterpret_cast<void*>(p.actor),&p.tag,offset,axis,true);
        p.joint=joint;memoryRead(p.actor+0x358,&p.frame,8);
        if(!rangedPropIdentity(true)){hideRangedProp();refuse("native_attachment_unconfirmed");return;}++rangedPropAttached;
    }
    if(!rangedPropIdentity(true)){refuse("prop_identity_changed");return;}
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(rangedProp.actor));
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(rangedProp.actor));
    rangedProp.network=automatic;rangedProp.until=tick+(automatic?250:15000);rangedPropResult="native_visual_prop_attached";
    // Newly created native models have not published a pose cache yet.
    // The next automatic update starts mechanical playback after evaluation.
}
void writeRangedProp(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.ranged-prop.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& p=rangedProp;
    fprintf(f,"{\"localExperiment\":true,\"active\":%s,\"pendingHide\":%s,\"kind\":%u,\"handle\":%u,\"parentHandle\":%u,\"joint\":%u,\"tag\":\"%s\",\"created\":%llu,\"attached\":%llu,\"hidden\":%llu,\"refused\":%llu,\"retired\":%llu,\"result\":\"%s\",\"network\":%s,\"mechanismApplied\":%llu,\"mechanismRefused\":%llu,\"parkedHandle\":%u,\"parkedKind\":%u,\"parkedHidden\":%s,\"switched\":%llu,\"reused\":%llu}\n",p.until?"true":"false",p.pendingHide?"true":"false",p.kind,p.handle,p.parentHandle,p.joint,p.tag,rangedPropCreated,rangedPropAttached,rangedPropHidden,rangedPropRefused,rangedPropRetired,rangedPropResult,p.network?"true":"false",rangedMechanismApplied.load(),rangedMechanismRefused.load(),rangedPropParked.handle,rangedPropParked.kind,rangedPropIsHidden(rangedPropParked)?"true":"false",rangedPropSwitched,rangedPropReused);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

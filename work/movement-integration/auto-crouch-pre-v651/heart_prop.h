#pragma once
#include "heart_hand_tracks.h"
// Finite isolated visual experiment. Uses only the native heart mesh and
// animation component, never the source heart's gameplay components.
RangedProp heartProp;
unsigned heartPropRequest=0;
std::uint64_t heartPropCreated=0,heartPropAttached=0,heartPropHidden=0,heartPropRefused=0,heartPropRetired=0;
const char* heartPropResult="idle";
void hideHeartProp(){
    auto& p=heartProp;if(!p.actor||(!p.until&&!p.pendingHide))return;p.until=0;p.pendingHide=true;
    if(!coopGameplayReady(outgoing)||!rangedPropApiMatches()||!rangedPropIdentityFor(p,false))return;
    reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(p.actor));
    // The parked/reusable helper deliberately rejects pendingHide. Confirm
    // native Hide through the entity flag before clearing our pending state.
    unsigned char flags=0;
    if(memoryRead(p.actor+0xd0,&flags,1)&&(flags&1)){p.pendingHide=false;++heartPropHidden;heartPropResult="native_heart_prop_hidden";}
}
bool updateHeartPropTrack(unsigned variant,float amount){
    auto& p=heartProp;
    if(variant>3||!rangedPropIdentityFor(p,true)||!rangedPropApiMatches())return false;
    const auto& track=heartHandTracks[localRole?0:1][variant];
    const float f=(std::max)(0.f,(std::min)(1.f,amount))*119;const auto a=unsigned(f),b=(std::min)(119u,a+1);
    MovementBank::Joint x{},y{};std::copy(track[a],track[a]+7,x.begin());std::copy(track[b],track[b]+7,y.begin());
    const auto q=MovementBank::blend(x,y,f-a);std::array<float,12> qts{};std::copy(q.begin(),q.end(),qts.begin());qts[8]=qts[9]=qts[10]=1;
    BodyMatrix m{};if(!bodyFromQts(qts,m))return false;
    float offset[3]{m.v[12],m.v[13],m.v[14]},axis[9]{};for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)axis[c*3+r]=m.v[c*4+r];
    reinterpret_cast<void(*)(void*,void*,const char**,const float*,const float*,bool)>(gameImage+0x875e80)(reinterpret_cast<void*>(p.parent),reinterpret_cast<void*>(p.actor),&p.tag,offset,axis,true);
    return rangedPropIdentityFor(p,true);
}
void serviceHeartProp(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    auto& p=heartProp;auto request=heartPropRequest;heartPropRequest=0;
    unsigned variant=0;float amount=0;bool presented=false;
    AcquireSRWLockShared(&movementLock);const auto& movement=movementPlayback;
    if(movement.end>tick&&movement.lease>tick&&movement.bank.hasHeart()&&movement.clock.weight>.5f&&movement.heartWeight>.1f&&
       movement.actor==probeActor&&movement.game==observedGame&&movement.epoch==outgoing.worldEpoch&&
       shadowStreamCurrent(tick)&&shadowStream.peerReady&&shadowStream.lastState&&tick-shadowStream.lastState<250){
        variant=movement.heartVariant;amount=movement.heartAmount;presented=HeartPresentation::visible(movement.heartHeld,variant,amount);
    }
    ReleaseSRWLockShared(&movementLock);
    const bool automatic=presented&&!request;
    if(p.network&&!automatic)hideHeartProp();
    if(automatic&&p.network&&p.until)p.until=tick+250;
    if(p.actor&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){p={};++heartPropRetired;heartPropResult="old_world_retired";}
    if(request==2||(p.until&&(tick>=p.until||rangedProp.until||!observerIsolation.ready||!observerIsolation.movementFixture||!shadowEquipmentAiReady()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote))))hideHeartProp();
    if(p.pendingHide)hideHeartProp();
    if(automatic&&!p.until&&!p.pendingHide)request=1;
    if(automatic&&p.network&&p.until){
        if(!updateHeartPropTrack(variant,amount)){hideHeartProp();++heartPropRefused;heartPropResult="network_attachment_changed";}
        return;
    }
    if(request!=1)return;
    const auto refuse=[&](const char* why){++heartPropRefused;heartPropResult=why;};
    if(p.until||p.pendingHide||rangedProp.until||rangedProp.pendingHide||!partnerAssetsEnabled||!probeDoppelganger||
       !powerFixtureApiMatches()||!rangedPropApiMatches()||std::strcmp(mapName,kGuardMap)||!coopGameplayReady(outgoing)||
       !coopGameplayReady(remote)||!shadowEquipmentAiReady()||!observerIsolation.ready||!observerIsolation.movementFixture||
       observerIsolation.until<tick+(automatic?300:16000)){refuse("unsupported_isolated_scope");return;}
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!p.actor){
        float pos[3]{},axis[9]{};
        if(!memoryRead(probeActor+0x300,pos,12)||!memoryRead(probeActor+0x30c,axis,36)){refuse("avatar_transform_unavailable");return;}
        for(float v:pos)if(!std::isfinite(v)){refuse("invalid_avatar_transform");return;}
        for(float v:axis)if(!std::isfinite(v)){refuse("invalid_avatar_transform");return;}
        const auto definition=lookupEntityDefinition(rangedPropDefinition(4));if(!definition){refuse("visual_definition_missing");return;}
        unsigned char parameters[12]{};reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
        const auto actor=reinterpret_cast<std::uintptr_t>(reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(definition),parameters));
        if(!actor){refuse("native_creation_refused");return;}
        // Cache immediately, including a failed creation, to avoid duplicating
        // an unconfirmed native object on a later manual request.
        p={};p.game=observedGame;p.actor=actor;p.definition=definition;p.kind=4;p.epoch=outgoing.worldEpoch;p.parent=probeActor;p.parentHandle=probeHandle;++heartPropCreated;
        std::uintptr_t vt=0,d=0;unsigned parent=0;
        if(!memoryRead(actor,&vt,8)||vt!=base+0x1c87d58||!memoryRead(actor+0x2e8,&d,8)||d!=definition||
           !memoryRead(actor+0xac,&parent,4)||parent!=0x3fff0010||worldIdentityEntity(memoryRead,observedGame,probeHandle)!=probeActor){refuse("creation_owner_unconfirmed");return;}
        std::memcpy(reinterpret_cast<void*>(actor+0xac),&probeHandle,4);
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(reinterpret_cast<void*>(actor),pos);
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(reinterpret_cast<void*>(actor),axis);
        reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(actor),-1);
        memoryRead(actor+0xa8,&p.handle,4);
        reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(actor));
    }
    if(!rangedPropIdentityFor(p,false)||!rangedPropIsHidden(p)){refuse("cosmetic_identity_unconfirmed");return;}
    if(p.joint==0xffff){
        const char* tag="left_wrist_jnt";const std::uint16_t joint=9;float offset[3]{},axis[9]{};
        {
            std::array<float,12> qts{};std::copy(heartHandTracks[localRole?0:1][0][0],heartHandTracks[localRole?0:1][0][0]+7,qts.begin());qts[8]=qts[9]=qts[10]=1;
            BodyMatrix m{};if(!bodyFromQts(qts,m)){refuse("calibration_missing");return;}
            for(unsigned r=0;r<3;++r){offset[r]=m.v[12+r];for(unsigned c=0;c<3;++c)axis[c*3+r]=m.v[c*4+r];}
        }
        NativeSkeleton s{};std::uintptr_t manager=0,vt=0,entries=0,namePointer=0;unsigned count=0;std::string name;
        if(!readNativeSkeleton(memoryRead,base,p.parentHandle,s)||!s.stable||s.actor!=p.parent||!bodyShadowTarget(s)||s.jointIds.size()<11||
           !memoryRead(base+0x22995c0,&manager,8)||!manager||!memoryRead(manager,&vt,8)||vt!=base+0x1b093c8||
           !memoryRead(manager+8,&entries,8)||!entries||!memoryRead(manager+0x10,&count,4)||s.jointIds[joint]>=(count&0xffffff)||
           !memoryRead(entries+std::uintptr_t(s.jointIds[joint])*72+16,&namePointer,8)||!worldIdentityString(memoryRead,namePointer,name)||name!=tag){refuse("native_wrist_unavailable");return;}
        p.tag=tag;reinterpret_cast<void(*)(void*,void*,const char**,const float*,const float*,bool)>(gameImage+0x875e80)(reinterpret_cast<void*>(p.parent),reinterpret_cast<void*>(p.actor),&p.tag,offset,axis,true);
        p.joint=joint;memoryRead(p.actor+0x358,&p.frame,8);
        if(!rangedPropIdentityFor(p,true)){refuse("attachment_unconfirmed");return;}++heartPropAttached;
    }
    if(!rangedPropIdentityFor(p,true)){refuse("attached_identity_changed");return;}
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(p.actor));
    reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(p.actor));
    p.network=automatic;p.until=tick+(automatic?250:15000);
    if(automatic&&!updateHeartPropTrack(variant,amount)){hideHeartProp();refuse("network_track_unconfirmed");return;}
    heartPropResult="native_heart_prop_attached";
}
void writeHeartProp(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.heart-prop.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& p=heartProp;
    fprintf(f,"{\"testFixtureOnly\":true,\"active\":%s,\"pendingHide\":%s,\"handle\":%u,\"parentHandle\":%u,\"joint\":%u,\"created\":%llu,\"attached\":%llu,\"hidden\":%llu,\"refused\":%llu,\"retired\":%llu,\"result\":\"%s\"}\n",p.until?"true":"false",p.pendingHide?"true":"false",p.handle,p.parentHandle,p.joint,heartPropCreated,heartPropAttached,heartPropHidden,heartPropRefused,heartPropRetired,heartPropResult);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

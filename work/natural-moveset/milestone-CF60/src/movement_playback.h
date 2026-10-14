#pragma once
#include "movement_bank.h"
SRWLOCK movementLock=SRWLOCK_INIT;
struct MovementPlayback {
    MovementBank bank;MovementClock clock;
    std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0;
    std::uint64_t epoch=0,peerEpoch=0,session=0;ULONGLONG end=0,lease=0;
    std::vector<std::int16_t> parents;std::uint64_t applied=0,refused=0;
} movementPlayback;
bool movementBankActive(){AcquireSRWLockShared(&movementLock);const bool active=movementPlayback.end!=0;ReleaseSRWLockShared(&movementLock);return active;}
void stopMovementBank(){AcquireSRWLockExclusive(&movementLock);movementPlayback.end=movementPlayback.lease=0;ReleaseSRWLockExclusive(&movementLock);}
bool movementHelperMatches(const NativeSkeleton& s){
    const unsigned char bytes[]={0x40,0x55,0x53,0x41,0x56,0x41,0x57,0x48,0x8d,0x6c,0x24,0xb8,0x48,0x81,0xec,0x48};
    std::uintptr_t h=0;unsigned count=0,words=0,joints=0;
    return !std::memcmp(gameImage+0x23fca0,bytes,sizeof(bytes))&&memoryRead(s.model+0xa0,&h,8)&&h&&
        memoryRead(s.model+0xa8,&count,4)&&(count&0xffffff)>0&&(count&0xffffff)<=256&&
        memoryRead(s.animation+0x2d0,&words,4)&&words>0&&words<=8&&memoryRead(s.animation+0x2d4,&joints,4)&&joints==s.parents.size()&&(joints==215||joints==219);
}
bool beginMovementBank(){
    const auto tick=GetTickCount64();
    if(movementBankActive()||reviewClipActive()||authoringProof.end||!probeDoppelganger||!avatarAiHeld||!probeFollow||!locomotionEnabled||bodyStreamEnabled||
        shadowAction.actor||!networkFrameOwned.load()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||
        !lastReceive||tick-lastReceive>=250||!bodyNativeFunctions())return false;
    NativeSkeleton s{};NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSkeleton(memoryRead,base,probeHandle,s)||!s.stable||s.logic!=1||s.actor!=probeActor||s.game!=observedGame||
        !bodyShadowTarget(s)||!movementHelperMatches(s)||
        !readNativeSwordEquipment(memoryRead,base,probeHandle,sword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)||!sword.equipped||!sword.started)return false;
    char path[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.movement.bin",statusPath)>=sizeof(path))return false;
    FILE* f=nullptr;if(fopen_s(&f,path,"rb")||!f)return false;
    std::vector<unsigned char> bytes(MovementBank::maximumBytes+1);const auto n=fread(bytes.data(),1,bytes.size(),f);const bool io=!ferror(f);fclose(f);bytes.resize(n);
    MovementPlayback next{};const auto layout=localRole?0x00db32d37719931full:0x84db84f5d1336b80ull;
    if(!io||!next.bank.decode(bytes,layout,localRole?215:219))return false;
    for(const auto& clip:next.bank.clips)for(unsigned frame=0;frame<MovementBank::frames;++frame)for(unsigned j=3;j<=26;++j)if(j!=17)
        for(unsigned k=0;k<3;++k)if(std::fabs(clip.values[frame*next.bank.joints+j][4+k]-s.bindLocal[j][4+k])>.0001f)return false;
    next.game=s.game;next.actor=s.actor;next.animation=s.animation;next.skeleton=s.skeleton;next.handle=probeHandle;next.parents=s.parents;
    next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.session=remote.session;next.end=tick+60000;next.lease=tick+250;
    AcquireSRWLockExclusive(&movementLock);movementPlayback=std::move(next);ReleaseSRWLockExclusive(&movementLock);
    bodyPublication.animation.store(s.animation,std::memory_order_release);return true;
}
void serviceMovementBank(ULONGLONG tick){
    AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(p.end){const bool ok=tick<p.end&&p.actor==probeActor&&p.game==observedGame&&p.handle==probeHandle&&p.epoch==outgoing.worldEpoch&&
        p.peerEpoch==remote.worldEpoch&&p.session==remote.session&&probeFollow&&avatarAiHeld&&locomotionEnabled&&!bodyStreamEnabled&&
        networkFrameOwned.load()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&lastReceive&&tick-lastReceive<250;
        if(ok)p.lease=lastReceive+250;else p.end=p.lease=0;}
    ReleaseSRWLockExclusive(&movementLock);
}
void publishMovementBank(const PresentationPose& pose,const float* position,double now){
    const auto tick=GetTickCount64();AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(p.end&&tick<p.lease){
        // Sword semantics travel through the existing native Steam action
        // stream. Unknown/stale actions never count as an idle permission.
        const bool eligible=pose.bodyValid&&pose.stance==0&&pose.motion==0&&shadowStream.until>tick&&shadowStream.peerReady&&
            shadowStream.lastState&&tick-shadowStream.lastState<250&&shadowStream.remoteState==0&&!shadowAction.actor;
        if(!p.clock.advance(p.bank,now,position,pose.orientation,eligible))++p.refused;
        if(!eligible)p.clock.weight=0; // native action has immediate priority
    }ReleaseSRWLockExclusive(&movementLock);
}
void applyMovementBank(void* component,void* actor){
    const auto tick=GetTickCount64();AcquireSRWLockExclusive(&movementLock);auto& p=movementPlayback;
    if(!p.end||tick>=p.end||tick>=p.lease||p.clock.weight<.001f||p.animation!=reinterpret_cast<std::uintptr_t>(component)||p.actor!=reinterpret_cast<std::uintptr_t>(actor)){
        ReleaseSRWLockExclusive(&movementLock);return;}
    NativeSkeleton s{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    bool ok=networkFrameOwned.load()&&readNativeSkeleton(memoryRead,base,p.handle,s)&&s.stable&&s.logic==1&&s.game==p.game&&s.actor==p.actor&&s.animation==p.animation&&s.skeleton==p.skeleton&&s.parents==p.parents&&movementHelperMatches(s);
    const unsigned count=p.bank.joints;std::vector<BodyMatrix> pose(count);
    float moving=(std::max)(0.f,(std::min)(1.f,(p.clock.speed-.025f)/.30f));moving=moving*moving*(3-2*moving);
    for(unsigned j=2;ok&&j<count;++j){
        auto a=p.bank.sample(p.clock.a,j,p.clock.phase),b=p.bank.sample(p.clock.b,j,p.clock.phase);
        auto gait=MovementBank::blend(a,b,p.clock.mix);
        if(p.bank.gaits>1)gait=MovementBank::blend(gait,MovementBank::blend(p.bank.sample(p.clock.a+8,j,p.clock.phase),p.bank.sample(p.clock.b+8,j,p.clock.phase),p.clock.mix),p.clock.runMix);
        auto v=MovementBank::blend(p.bank.sample(0,j,p.clock.idle),gait,moving);
        std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());q[8]=q[9]=q[10]=1;ok=bodyFromQts(q,pose[j]);
        if(ok&&p.clock.weight<.999f){BodyMatrix native{};std::memcpy(native.v,s.localPose[j].data(),64);if(!bodyRigid(native)){ok=false;break;}
            float x[9]{},y[9]{},r[9]{};for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k){x[c*3+k]=native.v[c*4+k];y[c*3+k]=pose[j].v[c*4+k];}
            presentationRotation(x,y,p.clock.weight,r);for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k)pose[j].v[c*4+k]=r[c*3+k];
            for(unsigned k=12;k<15;++k)pose[j].v[k]=native.v[k]+(pose[j].v[k]-native.v[k])*p.clock.weight;}
    }
    if(ok){for(unsigned j=2;j<count;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.animation+0x40),static_cast<std::uint16_t>(j),&pose[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.animation+0x40));
        reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x23fca0)(reinterpret_cast<void*>(s.model),reinterpret_cast<void*>(p.animation+0x40),reinterpret_cast<void*>(p.animation+0x2b0));++p.applied;
    }else{++p.refused;p.end=p.lease=0;}
    ReleaseSRWLockExclusive(&movementLock);
}
void writeMovementStatus(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.movement.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;
    fprintf(f,"{\"experimental\":true,\"active\":%s,\"end\":%llu,\"joints\":%u,\"phase\":%.7f,\"speed\":%.6f,\"weight\":%.6f,\"a\":%u,\"b\":%u,\"mix\":%.6f,\"checksum\":%llu,\"applied\":%llu,\"refused\":%llu,\"gaits\":%u,\"runMix\":%.6f}\n",p.end?"true":"false",p.end,p.bank.joints,p.clock.phase,p.clock.speed,p.clock.weight,p.clock.a,p.clock.b,p.clock.mix,p.bank.checksum,p.applied,p.refused,p.bank.gaits,p.clock.runMix);
    ReleaseSRWLockShared(&movementLock);const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

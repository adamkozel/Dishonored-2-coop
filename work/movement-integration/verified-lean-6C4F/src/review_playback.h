#pragma once
#include "review_clip.h"
SRWLOCK reviewLock=SRWLOCK_INIT;
struct ReviewPlayback {
    ReviewClip clip;std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0;
    std::uint64_t epoch=0,peerEpoch=0,session=0,applied=0,refused=0;ULONGLONG start=0,end=0,lease=0;
    float fixed=-1,seconds=0;std::vector<std::int16_t> parents;
} reviewPlayback;
bool reviewClipActive(){AcquireSRWLockShared(&reviewLock);const bool result=reviewPlayback.end!=0;ReleaseSRWLockShared(&reviewLock);return result;}
void stopReviewClip(){AcquireSRWLockExclusive(&reviewLock);reviewPlayback.end=reviewPlayback.lease=0;ReleaseSRWLockExclusive(&reviewLock);}
bool beginReviewClip(int fixedFrame){
    const auto tick=GetTickCount64();
    if(reviewClipActive()||movementBankActive()||authoringProof.end||!probeDoppelganger||!avatarAiHeld||!probeFollow||!locomotionEnabled||bodyStreamEnabled||
       shadowAction.actor||shadowStream.until||!networkFrameOwned.load()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||
       !lastReceive||tick-lastReceive>=250||!bodyNativeFunctions())return false;
    NativeSkeleton s{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSkeleton(memoryRead,base,probeHandle,s)||!s.stable||s.logic!=1||s.actor!=probeActor||s.game!=observedGame||!bodyShadowTarget(s)||!movementHelperMatches(s))return false;
    char path[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.review.bin",statusPath)>=sizeof(path))return false;
    FILE* f=nullptr;if(fopen_s(&f,path,"rb")||!f)return false;
    std::vector<unsigned char> bytes(ReviewClip::maximumBytes+1);const auto size=fread(bytes.data(),1,bytes.size(),f);const bool io=!ferror(f);fclose(f);bytes.resize(size);
    ReviewPlayback next{};const auto layout=localRole?0x00db32d37719931full:0x84db84f5d1336b80ull;
    if(!io||!next.clip.decode(bytes,localRole?215:219,layout)||fixedFrame>=int(next.clip.frames)||fixedFrame < -1)return false;
    for(unsigned frame=0;frame<next.clip.frames;++frame)for(unsigned j=3;j<=26;++j)if(j!=17)
        for(unsigned k=0;k<3;++k)if(std::fabs(next.clip.values[frame*next.clip.joints+j][4+k]-s.bindLocal[j][4+k])>.0001f)return false;
    next.game=s.game;next.actor=s.actor;next.animation=s.animation;next.skeleton=s.skeleton;next.handle=s.handle;next.parents=s.parents;
    next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.session=remote.session;
    next.fixed=fixedFrame<0?-1.f:float(fixedFrame)/next.clip.fps;next.start=tick+500;
    next.end=next.start+ULONGLONG(fixedFrame<0?next.clip.duration()*1000+600:10000);next.lease=tick+250;
    AcquireSRWLockExclusive(&reviewLock);reviewPlayback=std::move(next);ReleaseSRWLockExclusive(&reviewLock);
    bodyPublication.animation.store(s.animation,std::memory_order_release);return true;
}
void serviceReviewClip(ULONGLONG tick){
    AcquireSRWLockExclusive(&reviewLock);auto& p=reviewPlayback;
    if(p.end){const bool ok=tick<p.end&&p.actor==probeActor&&p.game==observedGame&&p.handle==probeHandle&&p.epoch==outgoing.worldEpoch&&
       p.peerEpoch==remote.worldEpoch&&p.session==remote.session&&probeFollow&&avatarAiHeld&&locomotionEnabled&&!bodyStreamEnabled&&!shadowAction.actor&&!shadowStream.until&&
       networkFrameOwned.load()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&lastReceive&&tick-lastReceive<250;
       if(ok)p.lease=lastReceive+250;else p.end=p.lease=0;}
    ReleaseSRWLockExclusive(&reviewLock);
}
void applyReviewClip(void* component,void* actor){
    const auto tick=GetTickCount64();AcquireSRWLockExclusive(&reviewLock);auto& p=reviewPlayback;
    if(!p.end||tick<p.start||tick>=p.end||tick>=p.lease||p.animation!=reinterpret_cast<std::uintptr_t>(component)||p.actor!=reinterpret_cast<std::uintptr_t>(actor)){
        ReleaseSRWLockExclusive(&reviewLock);return;}
    NativeSkeleton s{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    bool ok=!shadowAction.actor&&!shadowStream.until&&!bodyStreamEnabled&&networkFrameOwned.load()&&readNativeSkeleton(memoryRead,base,p.handle,s)&&s.stable&&s.logic==1&&
        s.game==p.game&&s.actor==p.actor&&s.animation==p.animation&&s.skeleton==p.skeleton&&s.parents==p.parents&&movementHelperMatches(s);
    p.seconds=p.fixed>=0?p.fixed:(std::max)(0.f,float(tick-p.start)*.001f-.3f);
    const float weight=(std::min)(1.f,(std::min)(float(tick-p.start)/300.f,float(p.end-tick)/300.f));
    const unsigned count=p.clip.joints;std::vector<BodyMatrix> pose(count);
    for(unsigned j=2;ok&&j<count;++j){const auto v=p.clip.sample(j,p.seconds);std::array<float,12> q{};std::copy(v.begin(),v.end(),q.begin());q[8]=q[9]=q[10]=1;ok=bodyFromQts(q,pose[j]);
        if(ok&&weight<.999f){BodyMatrix native{};std::memcpy(native.v,s.localPose[j].data(),64);if(!bodyRigid(native)){ok=false;break;}
            float x[9]{},y[9]{},r[9]{};for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k){x[c*3+k]=native.v[c*4+k];y[c*3+k]=pose[j].v[c*4+k];}
            const float w=weight*weight*(3-2*weight);presentationRotation(x,y,w,r);for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k)pose[j].v[c*4+k]=r[c*3+k];
            for(unsigned k=12;k<15;++k)pose[j].v[k]=native.v[k]+(pose[j].v[k]-native.v[k])*w;}
    }
    if(ok){for(unsigned j=2;j<count;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.animation+0x40),static_cast<std::uint16_t>(j),&pose[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.animation+0x40));
        reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x23fca0)(reinterpret_cast<void*>(s.model),reinterpret_cast<void*>(p.animation+0x40),reinterpret_cast<void*>(p.animation+0x2b0));++p.applied;
    }else{++p.refused;p.end=p.lease=0;}ReleaseSRWLockExclusive(&reviewLock);
}
void writeReviewStatus(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.review.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    AcquireSRWLockShared(&reviewLock);const auto& p=reviewPlayback;
    fprintf(f,"{\"experimental\":true,\"purpose\":\"visual review only\",\"active\":%s,\"start\":%llu,\"end\":%llu,\"seconds\":%.6f,\"fixed\":%.6f,\"joints\":%u,\"frames\":%u,\"checksum\":%llu,\"applied\":%llu,\"refused\":%llu}\n",p.end?"true":"false",p.start,p.end,p.seconds,p.fixed,p.clip.joints,p.clip.frames,p.clip.checksum,p.applied,p.refused);
    ReleaseSRWLockShared(&reviewLock);const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

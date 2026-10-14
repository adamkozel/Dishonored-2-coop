#pragma once
#include "authoring_clip.h"
SRWLOCK authoringLock=SRWLOCK_INIT;
struct AuthoringProof {
    AuthoringClip clip;std::uintptr_t game=0,actor=0,animation=0,skeleton=0;unsigned handle=0;
    std::uint64_t epoch=0,peerEpoch=0,session=0;ULONGLONG start=0,end=0,lease=0;
    float fixed=-1,frameSeconds=0,origin[3]{},forward[3]{};ULONGLONG stepFinished=0;std::vector<std::int16_t> parents;
} authoringProof;
std::atomic<std::uint64_t> authoringApplied{0},authoringRefused{0};
void stopAuthoringProof(){AcquireSRWLockExclusive(&authoringLock);authoringProof.lease=0;authoringProof.end=0;ReleaseSRWLockExclusive(&authoringLock);}
bool authoringHelperMatches(const NativeSkeleton& s){
    const unsigned char bytes[]={0x40,0x55,0x53,0x41,0x56,0x41,0x57,0x48,0x8d,0x6c,0x24,0xb8,0x48,0x81,0xec,0x48};
    std::uintptr_t h=0;unsigned count=0,words=0,joints=0;
    return !std::memcmp(gameImage+0x23fca0,bytes,sizeof(bytes))&&memoryRead(s.model+0xa0,&h,8)&&h&&
        memoryRead(s.model+0xa8,&count,4)&&(count&0xffffff)>0&&(count&0xffffff)<=256&&
        memoryRead(s.animation+0x2d0,&words,4)&&words>0&&words<=8&&memoryRead(s.animation+0x2d4,&joints,4)&&joints==215;
}
bool beginAuthoringProof(const char* mode){
    if(!std::strcmp(mode,"off")){stopAuthoringProof();return true;}
    const bool play=!std::strcmp(mode,"play"),ready=!std::strcmp(mode,"hold_ready"),guard=!std::strcmp(mode,"hold_guard");
    const auto tick=GetTickCount64();
    if((!play&&!ready&&!guard)||movementBankActive()||reviewClipActive()||localRole!=1||!probeDoppelganger||!avatarAiHeld||!probeFollow||!locomotionEnabled||bodyStreamEnabled||
        shadowAction.actor||shadowStream.until||!networkFrameOwned.load()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)||
        !lastReceive||tick-lastReceive>=250||!bodyNativeFunctions())return false;
    NativeSkeleton s{};NativeSwordEquipment sword{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!readNativeSkeleton(memoryRead,base,probeHandle,s)||!s.stable||s.logic!=1||s.actor!=probeActor||s.game!=observedGame||
        !bodyShadowTarget(s)||s.parents.size()!=215||!authoringHelperMatches(s)||
        !readNativeSwordEquipment(memoryRead,base,probeHandle,sword,NativeSwordScope::DoppelgangerCorvo)||!sword.equipped||!sword.started)return false;
    char path[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.authoring.bin",statusPath)>=sizeof(path))return false;
    FILE* f=nullptr;if(fopen_s(&f,path,"rb")||!f)return false;
    constexpr size_t expected=AuthoringClip::maximumBytes;
    std::vector<unsigned char> bytes(expected+1);const auto n=fread(bytes.data(),1,bytes.size(),f);const bool io=ferror(f)==0;fclose(f);bytes.resize(n);
    AuthoringProof next{};if(!io||!next.clip.decode(bytes))return false;
    // Preserve native anatomical lengths. Only pelvis and socket translation
    // and corrective helper transforms may differ from the bind reference.
    for(unsigned frame=0;frame<next.clip.frameCount;++frame)for(unsigned j=3;j<=26;++j)if(j!=17)
        for(unsigned k=0;k<3;++k)if(std::fabs(next.clip.values[frame*215+j][4+k]-s.bindLocal[j][4+k])>.0001f)return false;
    next.game=s.game;next.actor=s.actor;next.animation=s.animation;next.skeleton=s.skeleton;next.handle=probeHandle;next.parents=s.parents;
    next.epoch=outgoing.worldEpoch;next.peerEpoch=remote.worldEpoch;next.session=remote.session;
    std::memcpy(next.origin,s.root,12);std::memcpy(next.forward,s.root+3,12);
    next.start=tick+500;next.end=next.start+(next.clip.generated()?12000:10000);next.lease=tick+250;next.fixed=play?-1.f:ready?0.f:float(next.clip.guardFrame)/60;
    AcquireSRWLockExclusive(&authoringLock);authoringProof=std::move(next);ReleaseSRWLockExclusive(&authoringLock);
    bodyPublication.animation.store(s.animation,std::memory_order_release);return true;
}
void serviceAuthoringProof(ULONGLONG tick){
    AcquireSRWLockExclusive(&authoringLock);auto& p=authoringProof;
    if(p.end){
        const bool ok=tick<p.end&&p.actor==probeActor&&p.game==observedGame&&p.handle==probeHandle&&p.epoch==outgoing.worldEpoch&&
            p.peerEpoch==remote.worldEpoch&&p.session==remote.session&&probeFollow&&avatarAiHeld&&locomotionEnabled&&!bodyStreamEnabled&&
            !shadowAction.actor&&!shadowStream.until&&networkFrameOwned.load()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&lastReceive&&tick-lastReceive<250;
        if(ok)p.lease=lastReceive+250;else {p.lease=0;p.end=0;}
    }ReleaseSRWLockExclusive(&authoringLock);
}
void applyAuthoringProof(void* component,void* actor){
    AcquireSRWLockExclusive(&authoringLock);auto& p=authoringProof;const auto tick=GetTickCount64();
    if(!p.end||tick<p.start||tick>=p.end||tick>=p.lease||p.animation!=reinterpret_cast<std::uintptr_t>(component)||p.actor!=reinterpret_cast<std::uintptr_t>(actor)){ReleaseSRWLockExclusive(&authoringLock);return;}
    NativeSkeleton s{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    bool ok=networkFrameOwned.load()&&readNativeSkeleton(memoryRead,base,p.handle,s)&&s.stable&&s.logic==1&&s.game==p.game&&s.actor==p.actor&&s.animation==p.animation&&s.skeleton==p.skeleton&&s.parents==p.parents;
    std::vector<AuthoringClip::Joint> values;std::vector<BodyMatrix> pose(215);
    const float moveStart=float(p.clip.moveStart)/60,moveEnd=float(p.clip.moveEnd)/60;
    p.frameSeconds=p.fixed>=0?p.fixed:(std::min)(moveStart,float(tick-p.start)*.001f);
    if(ok&&p.fixed<0&&tick>=p.start+ULONGLONG(moveStart*1000)){
        float d=0;for(unsigned k=0;k<3;++k)d+=(s.root[k]-p.origin[k])*p.forward[k];
        float lateral=0;for(unsigned k=0;k<3;++k){const float v=s.root[k]-p.origin[k]-p.forward[k]*d;lateral+=v*v;}
        const float travelEnd=p.clip.generated()?p.clip.distanceCurve[p.clip.moveEnd]:.4f;
        if(d<-.03f||d>travelEnd+.15f||lateral>.0064f)ok=false;
        if(!p.stepFinished){
            if(p.clip.generated()){
                ok=ok&&p.clip.secondsAtDistance((std::max)(0.f,(std::min)(travelEnd,d)),p.frameSeconds);
                if(d>=travelEnd-.005f){p.stepFinished=tick;p.end=tick+ULONGLONG((p.clip.duration()-moveEnd+.3f)*1000);}
            }else{
            // Invert the authored 0.4 m smoothstep. The native controller and
            // ordinary Steam root stream determine gait phase, not wall time.
            float low=0,high=1;const float travel=(std::max)(0.f,(std::min)(1.f,d/.4f));
            for(unsigned i=0;i<18;++i){const float mid=(low+high)*.5f;if(mid*mid*(3-2*mid)<travel)low=mid;else high=mid;}
            p.frameSeconds=.8f+1.5f*(low+high)*.5f;
            if(d>=.395f){p.stepFinished=tick;p.end=tick+4200;}
            }
        }
        if(p.stepFinished)p.frameSeconds=(std::min)(p.clip.duration(),moveEnd+float(tick-p.stepFinished)*.001f);
    }
    if(ok)ok=p.clip.sample(p.frameSeconds,values)&&authoringHelperMatches(s);
    for(unsigned j=2;ok&&j<215;++j){std::array<float,12> q{};std::copy(values[j].begin(),values[j].end(),q.begin());q[8]=q[9]=q[10]=1;ok=bodyFromQts(q,pose[j]);}
    const float fade=(std::min)(1.f,(std::min)(float(tick-p.start)/300.f,float(p.end-tick)/300.f));
    const float weight=fade*fade*(3-2*fade);
    if(ok&&weight<1)for(unsigned j=2;j<215;++j){
        BodyMatrix native{};std::memcpy(native.v,s.localPose[j].data(),64);if(!bodyRigid(native)){ok=false;break;}
        float a[9]{},b[9]{},r[9]{};for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k){a[c*3+k]=native.v[c*4+k];b[c*3+k]=pose[j].v[c*4+k];}
        presentationRotation(a,b,weight,r);for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k)pose[j].v[c*4+k]=r[c*3+k];
        for(unsigned k=12;k<15;++k)pose[j].v[k]=native.v[k]+(pose[j].v[k]-native.v[k])*weight;
    }
    if(ok){
        // Whole offline-authored pose; no native action events and no gameplay
        // effects. Root and anchor are never written. Native skin helpers and
        // attachment publication run afterward on the owned remote clone only.
        for(unsigned j=2;j<215;++j)reinterpret_cast<void(*)(void*,std::uint16_t,const BodyMatrix*)>(gameImage+0x9cfa0)(reinterpret_cast<void*>(p.animation+0x40),static_cast<std::uint16_t>(j),&pose[j]);
        reinterpret_cast<void(*)(void*)>(gameImage+0x9dc40)(reinterpret_cast<void*>(p.animation+0x40));
        reinterpret_cast<void(*)(void*,void*,void*)>(gameImage+0x23fca0)(reinterpret_cast<void*>(s.model),reinterpret_cast<void*>(p.animation+0x40),reinterpret_cast<void*>(p.animation+0x2b0));
        ++authoringApplied;
    }else {++authoringRefused;p.lease=0;p.end=0;}
    ReleaseSRWLockExclusive(&authoringLock);
}
void writeAuthoringStatus(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.authoring.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    AcquireSRWLockShared(&authoringLock);const auto& p=authoringProof;
    fprintf(f,"{\"experimental\":true,\"active\":%s,\"start\":%llu,\"end\":%llu,\"seconds\":%.6f,\"fixed\":%.6f,\"checksum\":%llu,\"applied\":%llu,\"refused\":%llu}\n",p.end?"true":"false",p.start,p.end,p.frameSeconds,p.fixed,p.clip.checksum,authoringApplied.load(),authoringRefused.load());
    ReleaseSRWLockShared(&authoringLock);const bool ok=!ferror(f);const int close=fclose(f);if(ok&&!close)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

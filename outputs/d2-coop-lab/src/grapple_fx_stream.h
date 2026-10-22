#pragma once
#include "grapple_fx_protocol.h"
#include "native_player_grapple.h"
struct GrappleFxStream {
    std::uint64_t token=0,peerToken=0,event=0,activeEvent=0,impactEvent=0;
    unsigned nativeId=0,beamId=~0u;ULONGLONG lastRead=0,lastSend=0;
    bool sourceActive=false,presentedValid=false;CoopGrappleFxPacket last{},presented{};GrappleFxTimeline timeline;
} grappleFxStream;
std::uint64_t grappleFxSequence=0,grappleFxEvent=0,grappleFxSent=0,grappleFxReceived=0,grappleFxRejected=0,grappleFxBeams=0,grappleFxImpacts=0,grappleFxUpdates=0;
bool grappleFxCurrent(ULONGLONG tick){return shadowStreamCurrent(tick)&&shadowStream.peerReady&&grappleFxStream.token==shadowStream.token&&grappleFxStream.peerToken==shadowStream.peerToken;}
bool sendGrappleFx(CoopGrappleFxPacket p){
    auto& s=grappleFxStream;p.sequence=++grappleFxSequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
    p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;p.sourceToken=shadowStream.token;p.targetToken=shadowStream.peerToken;
    p.event=s.event;p.sampleTimeUs=(std::max)(static_cast<std::uint64_t>(presentationClock()*1000.),s.last.sampleTimeUs+1);p.rootSequence=outgoing.sequence;
    if(!validGrappleFxPacket(p,remote,outgoing)||!sockets||!connection||coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    s.last=p;++grappleFxSent;return true;
}
void sampleNativeGrappleFx(ULONGLONG tick){
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick)||!shadowStream.peerReady)return;
    auto& s=grappleFxStream;if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    if(localCharacter()!=2||tick-s.lastSend<20)return;s.lastSend=tick; // 1214ia (#159): whoever plays Emily
    NativePlayerGrapple g{};const bool valid=readNativePlayerGrapple(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),g);
    if(valid)s.lastRead=tick;
    if(valid&&g.present){
        if(s.sourceActive&&s.nativeId!=g.id){auto end=s.last;end.phase=3;sendGrappleFx(end);s.sourceActive=false;}
        const bool first=!s.sourceActive;if(first){s.event=++grappleFxEvent;s.nativeId=g.id;}
        CoopGrappleFxPacket p{};p.role=localRole;p.phase=!first&&g.pulling?2:1;p.goal=g.goal;p.impact=g.impact;p.normal=g.normal;
        float full=0,left=0;for(unsigned k=0;k<3;++k){const float a=g.goal[k]-g.hand[k],b=g.goal[k]-g.end[k];full+=a*a;left+=b*b;}
        p.progress=full>.00001f?(std::max)(0.f,(std::min)(1.f,1.f-std::sqrt(left/full))):1.f;
        if(!first&&p.phase<s.last.phase)p.phase=s.last.phase;
        if(sendGrappleFx(p))s.sourceActive=true;
    }else if(s.sourceActive&&(valid||tick-s.lastRead>140)){auto end=s.last;end.phase=3;sendGrappleFx(end);s.sourceActive=false;}
}
void receiveNativeGrappleFx(const CoopGrappleFxPacket& p,ULONGLONG tick){
    ++grappleFxReceived;auto& s=grappleFxStream;const double time=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset,now=presentationClock();
    if(!nativeMutationAllowed||!grappleFxCurrent(tick)||!movementBankActive()||!validGrappleFxPacket(p,outgoing,remote)||
       p.sourceToken!=shadowStream.peerToken||p.targetToken!=shadowStream.token||sourcePoseTimeline.session!=p.sourceSession||
       sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||time>now+80||now-time>250||!s.timeline.push(p,time))++grappleFxRejected;
}
void presentNativeGrapple(const PresentationPose& pose,ULONGLONG tick){
    auto& s=grappleFxStream;s.presentedValid=grappleFxCurrent(tick)&&movementBankActive()&&s.timeline.sample(pose.time,s.presented);
}
bool nativeGrappleHand(std::array<float,3>& hand){
    if(remoteIsCorvo())return false;NativeSkeleton s{};
    if(!readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s)||!s.stable||s.logic!=1||s.game!=observedGame||
       s.actor!=probeActor||!bodyShadowTarget(s)||s.parents.size()!=219||s.parents[9]!=8||s.parents[10]!=9)return false;
    AcquireSRWLockShared(&movementLock);const bool owned=movementPlayback.end&&movementPlayback.actor==s.actor&&movementPlayback.skeleton==s.skeleton&&movementPlayback.clock.weight>.8f;ReleaseSRWLockShared(&movementLock);
    if(!owned)return false;for(unsigned k=0;k<3;++k){hand[k]=s.root[k];for(unsigned c=0;c<3;++c)hand[k]+=s.root[3+c*3+k]*s.globalPose[9][12+c];}
    return true;
}
void stopNativeGrappleBeam(){
    auto& s=grappleFxStream;for(auto& e:nativePowerEffects)if(e.expires&&e.kind==4&&e.id==s.beamId&&e.streamToken==s.token&&e.peerToken==s.peerToken){
        if(nativePowerFxRecord(e))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),e.id);e={};++powerFxStopped;
    }s.beamId=~0u;
}
void serviceNativeGrappleFx(ULONGLONG tick){
    if(!nativePowerFxApi())return;auto& s=grappleFxStream;
    if(!grappleFxCurrent(tick)){if(coopGameplayReady(outgoing))stopNativeGrappleBeam();s={};return;}
    if(remoteIsCorvo())return; // 1214ia (#159): the partner plays Emily
    if(!movementBankActive()){stopNativeGrappleBeam();return;}
    if(!s.presentedValid||s.presented.phase==3){stopNativeGrappleBeam();return;}
    std::array<float,3> hand{};if(!nativeGrappleHand(hand))return;const auto& p=s.presented;
    if(s.activeEvent!=p.event){stopNativeGrappleBeam();s.activeEvent=p.event;
        if(startNativePowerFx(4,hand.data(),tick,true)){for(const auto& e:nativePowerEffects)if(e.expires&&e.kind==4&&e.streamToken==s.token&&e.peerToken==s.peerToken)s.beamId=e.id;++grappleFxBeams;}
    }
    for(auto& e:nativePowerEffects)if(e.expires&&e.kind==4&&e.id==s.beamId){const auto record=nativePowerFxRecord(e);if(!record)continue;
        std::array<float,3> end{};for(unsigned k=0;k<3;++k)end[k]=hand[k]+(p.goal[k]-hand[k])*p.progress;
        // Same two owned-record fields updated by native PlayerBody 776490.
        // The native first-person release callback is deliberately absent.
        std::uintptr_t callback=1;if(!memoryRead(record+0x128,&callback,8)||callback){++grappleFxRejected;continue;}
        std::memcpy(reinterpret_cast<void*>(record+0x38),hand.data(),12);std::memcpy(reinterpret_cast<void*>(record+0x80),end.data(),12);++grappleFxUpdates;
    }
    if(p.phase==2&&s.impactEvent!=p.event){s.impactEvent=p.event;float axis[9]{};
        float n=0;for(float v:p.normal)n+=v*v;n=std::sqrt(n);for(unsigned k=0;k<3;++k)axis[6+k]=p.normal[k]/n;
        const std::array<float,3> reference=std::fabs(axis[8])<.9f?std::array<float,3>{0,0,1}:std::array<float,3>{0,1,0};
        axis[0]=reference[1]*axis[8]-reference[2]*axis[7];axis[1]=reference[2]*axis[6]-reference[0]*axis[8];axis[2]=reference[0]*axis[7]-reference[1]*axis[6];
        n=std::sqrt(axis[0]*axis[0]+axis[1]*axis[1]+axis[2]*axis[2]);for(unsigned k=0;k<3;++k)axis[k]/=n;
        axis[3]=axis[7]*axis[2]-axis[8]*axis[1];axis[4]=axis[8]*axis[0]-axis[6]*axis[2];axis[5]=axis[6]*axis[1]-axis[7]*axis[0];
        if(startNativePowerFx(5,p.impact.data(),tick,true,axis))++grappleFxImpacts;else ++grappleFxRejected;
    }
}
void writeGrappleFxStream(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.grapple-fx.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& s=grappleFxStream;
    fprintf(f,"{\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"beams\":%llu,\"impacts\":%llu,\"updates\":%llu,\"sourceActive\":%s,\"event\":%llu,\"beamId\":%u,\"phase\":%u,\"progress\":%.6f,\"presented\":%s}\n",grappleFxSent,grappleFxReceived,grappleFxRejected,grappleFxBeams,grappleFxImpacts,grappleFxUpdates,s.sourceActive?"true":"false",s.activeEvent,s.beamId,s.presented.phase,s.presented.progress,s.presentedValid?"true":"false");fclose(f);MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
void duplicateNativeGrappleFx(){
    const auto& p=grappleFxStream.last;if(GetCurrentThreadId()==engineThread.load()&&grappleFxCurrent(GetTickCount64())&&p.sourceToken==shadowStream.token&&p.targetToken==shadowStream.peerToken&&sockets&&connection)
        coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}

#pragma once
#include "mesmerize_world_protocol.h"
#include "native_mesmerize_world.h"
#include "native_mesmerize_eye.h"
#include "native_mesmerize_targets.h"
struct MesmerizeWorldStream {
    struct Pending {CoopMesmerizeWorldPacket packet{};double time=0;};
    std::array<Pending,8> pending{};unsigned count=0;
    std::uint64_t token=0,peerToken=0,sourceEvent=0;
    unsigned sourceHandle=0,sourceState=0;bool sourceSnapshot=false;ULONGLONG nextSample=0;
    MesmerizeWorldOrder order{};
    CoopMesmerizeWorldPacket presented{};ULONGLONG lease=0,firstPresented=0,retireAt=0;
} mesmerizeWorldStream;
std::uint64_t mesmerizeWorldSequence=0,mesmerizeWorldEvent=0,mesmerizeWorldSent=0,mesmerizeWorldReceived=0,mesmerizeWorldRejected=0,mesmerizeWorldUnknown=0,mesmerizeWorldPresented=0;
CoopMesmerizeWorldPacket lastMesmerizeWorldPacket{};
double mesmerizeEyeReadMaxMs=0;std::uint64_t mesmerizeEyeReadCount=0,mesmerizeEyeReadFailed=0;
double mesmerizeLinkReadMaxMs=0;std::uint64_t mesmerizeLinkReadCount=0,mesmerizeLinkReadFailed=0;
unsigned mesmerizeLinkReadStage=0;
bool mesmerizeWorldStreamCurrent(ULONGLONG tick){
    return shadowStreamCurrent(tick)&&shadowStream.peerReady&&mesmerizeWorldStream.token==shadowStream.token&&mesmerizeWorldStream.peerToken==shadowStream.peerToken;
}
void sampleMesmerizeWorld(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    auto& s=mesmerizeWorldStream;
    if(!shadowStreamCurrent(tick)||!shadowStream.peerReady){s={};return;}
    if(s.token!=shadowStream.token||s.peerToken!=shadowStream.peerToken){s={};s.token=shadowStream.token;s.peerToken=shadowStream.peerToken;}
    if(localCharacter()!=2||tick<s.nextSample)return;s.nextSample=tick+80; // 1214ia (#159)
    NativeMesmerizeWorld world{};
    // An unreadable or retired pointer is not evidence of a gameplay removal.
    // Silence merely lets the receiver's short cosmetic lease expire.
    if(!readNativeMesmerizeWorld(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),world)){++mesmerizeWorldUnknown;return;}
    if(!world.present||(world.state!=1&&world.state!=2&&world.state!=3))return;
    if(world.handle!=s.sourceHandle){
        const bool snapshot=!s.sourceHandle&&world.state==3;
        if(world.state!=1&&!snapshot)return;
        s.sourceSnapshot=snapshot;s.sourceHandle=world.handle;s.sourceEvent=++mesmerizeWorldEvent;s.sourceState=0;
    }
    if(!s.sourceEvent)return;
    CoopMesmerizeWorldPacket p{};p.role=localRole;p.sequence=++mesmerizeWorldSequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
    p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;p.sourceToken=s.token;p.targetToken=s.peerToken;
    p.event=s.sourceEvent;p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;p.nativeHandle=world.handle;p.state=world.state;p.transform=world.transform;
    p.snapshot=s.sourceSnapshot&&world.state==3?1:0;
    NativeMesmerizeEyes eyes{};const double eyeReadStart=presentationClock();
    const bool eyeRead=readNativeMesmerizeEyes(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),eyes,nullptr,&world);
    mesmerizeEyeReadMaxMs=(std::max)(mesmerizeEyeReadMaxMs,presentationClock()-eyeReadStart);++mesmerizeEyeReadCount;if(!eyeRead)++mesmerizeEyeReadFailed;
    if(eyeRead&&eyes.world.present&&
       eyes.world.actor==world.actor&&eyes.world.handle==world.handle&&eyes.world.component==world.component&&eyes.world.state==world.state&&eyes.world.transform==world.transform){
        p.eyesValid=1;p.eyeCount=static_cast<unsigned>(eyes.effects.size());
        for(unsigned i=0;i<p.eyeCount;++i){p.eyes[i].id=eyes.effects[i].id;std::memcpy(p.eyes[i].transform.data(),eyes.effects[i].origin,12);std::memcpy(p.eyes[i].transform.data()+3,eyes.effects[i].axis,36);}
    }
    NativeMesmerizeTargets targets{};const double targetStart=presentationClock();
    const bool targetRead=readNativeMesmerizeTargets(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),targets,&mesmerizeLinkReadStage,&world);
    mesmerizeLinkReadMaxMs=(std::max)(mesmerizeLinkReadMaxMs,presentationClock()-targetStart);++mesmerizeLinkReadCount;if(!targetRead)++mesmerizeLinkReadFailed;
    if(targetRead&&targets.world.present&&targets.world.actor==world.actor&&targets.world.handle==world.handle&&
       targets.world.component==world.component&&targets.world.state==world.state&&targets.world.transform==world.transform&&targets.targets.size()<=5){
        bool scoped=true;
        for(const auto& target:targets.targets){
            if(target.name.find("/d2coop/")!=std::string::npos){scoped=false;break;}
            for(const auto& fx:target.effects){
                if(p.linkCount>=p.links.size()){scoped=false;break;}
                auto& link=p.links[p.linkCount++];link.id=fx.fx.id;link.target=target.key.handle;link.kind=fx.kind;
                std::memcpy(link.transform.data(),fx.fx.origin,12);std::memcpy(link.transform.data()+3,fx.fx.axis,36);if(fx.kind<3)link.other=fx.other;
            }
            if(!scoped)break;
        }
        if(scoped)p.linksValid=1;else{p.linkCount=0;p.links={};}
    }
    if(!validMesmerizeWorldPacket(p,remote,outgoing)||!sockets||!connection)return;
    const int flags=s.sourceState!=world.state?k_nSteamNetworkingSend_ReliableNoNagle:k_nSteamNetworkingSend_UnreliableNoDelay;
    if(coopSendMessage(connection,&p,sizeof(p),flags,nullptr)!=k_EResultOK)return;
    s.sourceState=world.state;lastMesmerizeWorldPacket=p;++mesmerizeWorldSent;
}
void receiveMesmerizeWorld(const CoopMesmerizeWorldPacket& p,ULONGLONG tick){
    ++mesmerizeWorldReceived;auto& s=mesmerizeWorldStream;
    if(!nativeMutationAllowed||!mesmerizeWorldStreamCurrent(tick)||!validMesmerizeWorldPacket(p,outgoing,remote)||
       p.sourceToken!=shadowStream.peerToken||p.targetToken!=shadowStream.token||!movementBankActive()||s.count>=8){++mesmerizeWorldRejected;return;}
    const double now=presentationClock(),mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset;auto order=s.order;
    if(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||
       mapped>now+80||now-mapped>250||!order.accept(p)){++mesmerizeWorldRejected;return;}
    s.order=order;s.pending[s.count++]={p,mapped};
}
void presentMesmerizeWorld(ULONGLONG tick){
    auto& s=mesmerizeWorldStream;
    if(!mesmerizeWorldStreamCurrent(tick)||!movementBankActive()||!coopGameplayReady(outgoing)||!coopGameplayReady(remote)){s.lease=0;return;}
    const double now=presentationClock()-80.;
    while(s.count&&s.pending[0].time<=now){const auto pending=s.pending[0];for(unsigned i=1;i<s.count;++i)s.pending[i-1]=s.pending[i];--s.count;
        if(now-pending.time>250){++mesmerizeWorldRejected;continue;}
        if(s.presented.event!=pending.packet.event){s.firstPresented=tick;s.retireAt=0;}
        if(pending.packet.state==2&&!s.retireAt)s.retireAt=tick+700;
        s.presented=pending.packet;s.lease=pending.packet.state==2?s.retireAt:tick+300;++mesmerizeWorldPresented;
    }
    if(s.retireAt&&tick>=s.retireAt)s.lease=0;
    if(s.firstPresented&&tick-s.firstPresented>40000)s.lease=0;
}
void duplicateMesmerizeWorld(){
    if(GetCurrentThreadId()==engineThread.load()&&mesmerizeWorldStreamCurrent(GetTickCount64())&&lastMesmerizeWorldPacket.sourceToken==shadowStream.token&&lastMesmerizeWorldPacket.targetToken==shadowStream.peerToken&&sockets&&connection)
        coopSendMessage(connection,&lastMesmerizeWorldPacket,sizeof(lastMesmerizeWorldPacket),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void writeMesmerizeWorldStream(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.mesmerize-world-stream.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=mesmerizeWorldStream;
    fprintf(f,"{\"snapshot\":%u,\"sourceSnapshot\":%s,",s.presented.snapshot,s.sourceSnapshot?"true":"false");
    fprintf(f,"\"eyeReadMaxMs\":%.4f,\"eyeReadCount\":%llu,\"eyeReadFailed\":%llu,\"linkReadMaxMs\":%.4f,\"linkReadCount\":%llu,\"linkReadFailed\":%llu,\"linkReadStage\":%u,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"unknownReads\":%llu,\"presented\":%llu,\"event\":%llu,\"sourceEvent\":%llu,\"state\":%u,\"sourceState\":%u,\"nativeHandle\":%u,\"leased\":%s,\"pending\":%u}\n",mesmerizeEyeReadMaxMs,mesmerizeEyeReadCount,mesmerizeEyeReadFailed,mesmerizeLinkReadMaxMs,mesmerizeLinkReadCount,mesmerizeLinkReadFailed,mesmerizeLinkReadStage,mesmerizeWorldSent,mesmerizeWorldReceived,mesmerizeWorldRejected,mesmerizeWorldUnknown,mesmerizeWorldPresented,s.presented.event,s.sourceEvent,s.presented.state,s.sourceState,s.presented.nativeHandle,s.lease>GetTickCount64()?"true":"false",s.count);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

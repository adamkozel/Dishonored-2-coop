#pragma once
#include "power_fx_protocol.h"
struct PowerFxStream {
    struct Pending {CoopPowerFxPacket packet{};double time=0;};
    std::array<Pending,8> pending{},ready{};unsigned pendingCount=0,readyCount=0;
    std::uint64_t token=0,peerToken=0,event=0;PowerFxOrder order{};
    bool inBlink=false,sourceActive=false,sourceSent=false,holding=false,arrived=false;
    std::array<float,3> sourceGoal{},holdPosition{},arrivalPosition{};
    double sourceStarted=0,holdTime=0,arrivalTime=0;
} powerFxStream;
std::uint64_t powerFxSequence=0,powerFxEvent=0,powerFxSent=0,powerFxReceived=0,powerFxRejected=0,powerFxApplied=0,powerFxCancelled=0;
CoopPowerFxPacket lastPowerFxPacket{};
bool powerFxStreamCurrent(ULONGLONG tick){
    return shadowStreamCurrent(tick)&&shadowStream.peerReady&&powerFxStream.token==shadowStream.token&&powerFxStream.peerToken==shadowStream.peerToken;
}
bool sendNativeBlinkFx(unsigned kind,const std::array<float,3>& point){
    CoopPowerFxPacket p{};p.role=localRole;p.sequence=++powerFxSequence;p.sourceSession=outgoing.session;p.targetSession=remote.session;
    p.sourceWorldEpoch=outgoing.worldEpoch;p.targetWorldEpoch=remote.worldEpoch;p.mapId=outgoing.mapId;p.sourceToken=shadowStream.token;p.targetToken=shadowStream.peerToken;
    p.event=powerFxStream.event;p.sampleTimeUs=static_cast<std::uint64_t>(presentationClock()*1000.);p.rootSequence=outgoing.sequence;p.kind=kind;p.position=point;
    if(!validPowerFxPacket(p,remote,outgoing)||!sockets||!connection)return false;
    if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)!=k_EResultOK)return false;
    lastPowerFxPacket=p;++powerFxSent;return true;
}
void sampleNativeBlinkFx(ULONGLONG tick){
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick)||!shadowStream.peerReady)return;
    if(powerFxStream.token!=shadowStream.token||powerFxStream.peerToken!=shadowStream.peerToken){powerFxStream={};powerFxStream.token=shadowStream.token;powerFxStream.peerToken=shadowStream.peerToken;}
    if(localCharacter()!=1)return;auto& s=powerFxStream;const double now=presentationClock(); // 1214ia (#159): whoever plays Corvo
    if(sampledBlinkValid){
        const auto& blink=sampledNativeBlink;
        if(!s.inBlink){s.inBlink=s.sourceActive=true;s.sourceGoal=blink.goal;s.event=++powerFxEvent;s.sourceStarted=now;s.sourceSent=sendNativeBlinkFx(1,blink.start);}
        float distance=0;for(unsigned k=0;k<3;++k){const float d=outgoing.position[k]-s.sourceGoal[k];distance+=d*d;}
        if(s.sourceActive&&blink.completed&&distance<.04f){if(s.sourceSent)sendNativeBlinkFx(2,blink.foot);s.sourceActive=false;}
    }else if(sampledBlinkStateKnown&&!sampledBlinkStatePresent){
        if(s.sourceActive){float distance=0;std::array<float,3> point{};for(unsigned k=0;k<3;++k){point[k]=outgoing.position[k];const float d=point[k]-s.sourceGoal[k];distance+=d*d;}
            if(s.sourceSent&&now-s.sourceStarted<2000&&distance<.04f)sendNativeBlinkFx(2,point);else ++powerFxCancelled;
        }
        s.inBlink=s.sourceActive=s.sourceSent=false;
    }
    if(s.sourceActive&&now-s.sourceStarted>2000){s.sourceActive=false;++powerFxCancelled;}
}
void receiveNativePowerFx(const CoopPowerFxPacket& p,ULONGLONG tick){
    ++powerFxReceived;
    if(!nativeMutationAllowed||!powerFxStreamCurrent(tick)||!validPowerFxPacket(p,outgoing,remote)||
       p.sourceToken!=shadowStream.peerToken||p.targetToken!=shadowStream.token||!movementBankActive()||powerFxStream.pendingCount>=8){++powerFxRejected;return;}
    const double mapped=double(p.sampleTimeUs)*.001+sourcePoseTimeline.offset,now=presentationClock();auto order=powerFxStream.order;
    if(sourcePoseTimeline.session!=p.sourceSession||sourcePoseTimeline.epoch!=p.sourceWorldEpoch||sourcePoseTimeline.map!=p.mapId||
       mapped>now+80||now-mapped>250||!order.accept(p)){++powerFxRejected;return;}
    powerFxStream.order=order;powerFxStream.pending[powerFxStream.pendingCount++]={p,mapped};
}
void presentNativeBlink(PresentationPose& pose,ULONGLONG tick){
    if(!powerFxStreamCurrent(tick)||!movementBankActive())return;auto& s=powerFxStream;
    while(s.pendingCount&&s.pending[0].time<=pose.time){const auto p=s.pending[0];for(unsigned i=1;i<s.pendingCount;++i)s.pending[i-1]=s.pending[i];--s.pendingCount;
        if(pose.time-p.time>250||s.readyCount>=8){++powerFxRejected;continue;}
        s.ready[s.readyCount++]=p;
        if(p.packet.kind==1){s.holding=true;s.arrived=false;s.holdTime=p.time;s.holdPosition=p.packet.position;}
        else{s.holding=false;s.arrived=true;s.arrivalTime=p.time;s.arrivalPosition=p.packet.position;pose.teleport=true;}
    }
    if(s.holding){if(pose.time-s.holdTime<=1000)std::memcpy(pose.position,s.holdPosition.data(),12);else{s.holding=false;++powerFxCancelled;}}
    // Keep the confirmed arrival through the small overlap with pre-arrival
    // root samples. Then the ordinary network root resumes without rewinding.
    if(s.arrived){if(pose.time-s.arrivalTime<=100)std::memcpy(pose.position,s.arrivalPosition.data(),12);else s.arrived=false;}
}
void servicePowerFxStream(ULONGLONG tick){
    if(!nativeMutationAllowed)return;
    if(!powerFxStreamCurrent(tick)){powerFxStream={};return;}
    for(unsigned i=0;i<powerFxStream.readyCount;++i){const auto& p=powerFxStream.ready[i];
        if(presentationClock()-p.time<400&&startNativePowerFx(p.packet.kind,p.packet.position.data(),tick,true))++powerFxApplied;else ++powerFxRejected;
    }
    powerFxStream.readyCount=0;
}
void duplicateNativePowerFx(){
    if(GetCurrentThreadId()==engineThread.load()&&powerFxStreamCurrent(GetTickCount64())&&lastPowerFxPacket.sourceToken==shadowStream.token&&lastPowerFxPacket.targetToken==shadowStream.peerToken&&sockets&&connection)
        coopSendMessage(connection,&lastPowerFxPacket,sizeof(lastPowerFxPacket),k_nSteamNetworkingSend_ReliableNoNagle,nullptr);
}
void writePowerFxStream(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};if(std::snprintf(path,sizeof(path),"%s.power-fx-stream.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"applied\":%llu,\"cancelled\":%llu,\"sourceActive\":%s,\"holding\":%s,\"arrived\":%s,\"pending\":%u,\"event\":%llu,\"phase\":%u,\"nativeBlink\":%s,\"nativeBlinkCompleted\":%s,\"nativeBlinkFlags\":%u}\n",powerFxSent,powerFxReceived,powerFxRejected,powerFxApplied,powerFxCancelled,
        powerFxStream.sourceActive?"true":"false",powerFxStream.holding?"true":"false",powerFxStream.arrived?"true":"false",powerFxStream.pendingCount,powerFxStream.order.event,powerFxStream.order.phase,
        sampledBlinkValid?"true":"false",sampledBlinkValid&&sampledNativeBlink.completed?"true":"false",sampledBlinkValid?sampledNativeBlink.flags:0);
    fclose(f);MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

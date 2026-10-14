#pragma once
#include "power_failure_fx_policy.h"
PowerFailureFxOrder powerFailureFxOrder;
void servicePowerFailureFx(ULONGLONG tick){
    if(!nativeMutationAllowed||!shadowStreamCurrent(tick)||!shadowStream.peerReady){powerFailureFxOrder={};return;}
    if(localRole!=1)return; // Only Corvo's failed Bend Time scene is currently verified.
    std::uint64_t event=0;unsigned variant=0;float amount=0;double at=0;bool current=false;
    AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;
    current=p.end>tick&&p.lease>tick&&p.game==observedGame&&p.epoch==outgoing.worldEpoch&&
        p.peerEpoch==remote.worldEpoch&&p.session==remote.session&&p.actor==probeActor&&
        p.bank.hasOutOfMana()&&p.powerWeight>.05f;
    event=p.powerEvent;variant=p.powerVariant;amount=p.powerAmount;at=p.combatLastTime;
    ReleaseSRWLockShared(&movementLock);
    PowerPresentation::Sample sample{};float sampledAmount=0;
    current=current&&presentationClock()-at>=0&&presentationClock()-at<400&&
        shadowStream.power.sample(at,sample,sampledAmount)&&sample.event==event&&sample.variant==variant;
    if(powerFailureFxOrder.consume(shadowStream.token,shadowStream.peerToken,event,variant,amount,current))
        startNativePowerFx(11,nullptr,tick,true);
}

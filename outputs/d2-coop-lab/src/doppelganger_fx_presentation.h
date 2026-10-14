#pragma once
struct DoppelgangerFxPresentation {
    NativeDoppelgangerVisual target{};DoppelgangerFxScene scene{};
    std::uintptr_t game=0;std::uint64_t epoch=0,event=0,token=0,peerToken=0;
    unsigned sourceScene=0,sourceVariant=0,elapsed=0,nativeTime=0;
    std::array<unsigned,8> attempted{};unsigned attempts=0;
} doppelgangerFxPresentation;
struct DoppelgangerFxStartRecord {std::uint64_t sequence,event;unsigned sourceScene,nativeScene,variant,elapsed,nativeTime;bool verified;};
std::array<DoppelgangerFxStartRecord,32> doppelgangerFxHistory{};std::uint64_t doppelgangerFxHistoryCount=0;
std::array<std::uint64_t,2> doppelgangerFxStarted{};std::uint64_t doppelgangerFxStopped=0,doppelgangerFxRefused=0;
const char* doppelgangerFxResult="idle";
void serviceDoppelgangerFx(ULONGLONG tick,const NativeDoppelgangerVisual& target,std::uintptr_t game,std::uint64_t epoch,bool active){
    auto& f=doppelgangerFxPresentation;const auto& stream=doppelgangerWorldStream;const auto& p=stream.presented;
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    if(f.game&&(f.game!=observedGame||f.epoch!=outgoing.worldEpoch)){f={};doppelgangerFxResult="world_retired";}
    const auto stop=[&](){if(!f.scene.handle)return true;if(!stopDoppelgangerFxScene(f.scene,f.target))return false;++doppelgangerFxStopped;return true;};
    const bool owner=active&&game==observedGame&&epoch==outgoing.worldEpoch&&p.present&&doppelgangerWorldCurrent(tick)&&stream.lease>tick&&currentDoppelgangerVisual(target,game);
    if(!owner){if(!stop())return;doppelgangerFxResult="inactive";return;}
    if(f.event!=p.event||f.token!=stream.token||f.peerToken!=stream.peerToken||f.target.actor!=target.actor){
        if(!stop())return;f={};f.target=target;f.game=game;f.epoch=epoch;f.event=p.event;f.token=stream.token;f.peerToken=stream.peerToken;
    }
    if(!p.fxKnown)return; // An unknown source read is not a disappearance event.
    if(!p.fxVariant){if(!stop())return;doppelgangerFxResult="source_scene_complete";return;}
    if(f.scene.handle&&f.sourceScene!=p.fxScene){if(!stop())return;}
    for(unsigned i=0;i<f.attempts;++i)if(f.attempted[i]==p.fxScene)return;
    if(f.attempts>=f.attempted.size()){++doppelgangerFxRefused;doppelgangerFxResult="scene_limit";return;}
    const double age=(std::max)(0.,presentationClock()-120.-stream.presentedTime);
    const double elapsed=double(p.fxTimeMs)+age;const unsigned duration=doppelgangerFxDuration(p.fxVariant);
    // Mark before invoking native code. Ambiguous native results cannot start twice.
    f.attempted[f.attempts++]=p.fxScene;f.sourceScene=p.fxScene;f.sourceVariant=p.fxVariant;
    if(!std::isfinite(elapsed)||elapsed>=duration){doppelgangerFxResult="expired_before_ready";return;}
    f.elapsed=static_cast<unsigned>(elapsed);
    const bool started=startDoppelgangerFxScene(f.scene,f.target,p.fxVariant,f.elapsed);
    std::uintptr_t instance=0;unsigned phase=0;f.nativeTime=0;
    if(doppelgangerFxSceneInspect(f.scene,f.target,instance,phase)&&instance)memoryRead(instance+0x160,&f.nativeTime,4);
    doppelgangerFxHistory[doppelgangerFxHistoryCount++%32]={p.sequence,p.event,p.fxScene,f.scene.handle,p.fxVariant,f.elapsed,f.nativeTime,started};
    if(started){++doppelgangerFxStarted[p.fxVariant-1];doppelgangerFxResult="native_effect_scene_started";}
    else{++doppelgangerFxRefused;doppelgangerFxResult="native_effect_scene_refused";}
}
void writeDoppelgangerFx(FILE* f){
    const auto& p=doppelgangerFxPresentation;
    fprintf(f,",\"fx\":{\"scene\":%u,\"sourceScene\":%u,\"variant\":%u,\"elapsed\":%u,\"nativeTime\":%u,\"started\":[%llu,%llu],\"stopped\":%llu,\"refused\":%llu,\"stage\":%u,\"startError\":%d,\"result\":\"%s\",\"history\":[",p.scene.handle,p.sourceScene,p.sourceVariant,p.elapsed,p.nativeTime,doppelgangerFxStarted[0],doppelgangerFxStarted[1],doppelgangerFxStopped,doppelgangerFxRefused,doppelgangerFxSceneStage,doppelgangerFxStartError,doppelgangerFxResult);
    const auto n=(std::min)(doppelgangerFxHistoryCount,32ull);for(std::uint64_t i=0;i<n;++i){const auto& r=doppelgangerFxHistory[(doppelgangerFxHistoryCount-n+i)%32];
        fprintf(f,"%s{\"sequence\":%llu,\"event\":%llu,\"sourceScene\":%u,\"nativeScene\":%u,\"variant\":%u,\"elapsed\":%u,\"nativeTime\":%u,\"verified\":%s}",i?",":"",r.sequence,r.event,r.sourceScene,r.nativeScene,r.variant,r.elapsed,r.nativeTime,r.verified?"true":"false");}
    fputs("]}",f);
}

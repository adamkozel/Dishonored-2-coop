#pragma once
// Source-owned particles only: the receiver never creates a native power,
// RatAgent, attachment callback or damage event. The already validated rat
// snapshot carries the native trail's world transform and opaque identity.
struct SwarmTrailPresentation {
    struct Entry {unsigned sourceId=0,nativeId=~0u,handle=0,incarnation=0;std::array<float,12> pose{};};
    std::array<Entry,100> entries{};std::uint64_t token=0,peerToken=0,event=0;unsigned world=0;
} swarmTrailPresentation;
std::uint64_t swarmTrailStarted=0,swarmTrailStopped=0,swarmTrailFinished=0,swarmTrailRefused=0,swarmTrailUpdated=0,swarmTrailInterpolated=0;
const char* swarmTrailResult="idle";
const char* swarmTrailLastRefusal="none";
double swarmTrailMaximumReadbackError=0;
NativePowerFx* swarmTrailEffect(unsigned id){if(id==~0u)return nullptr;for(auto& e:nativePowerEffects)if(e.expires&&e.kind==21&&e.id==id)return &e;return nullptr;}
void stopSwarmTrail(SwarmTrailPresentation::Entry& e){
    if(e.nativeId==~0u)return;
    if(auto* fx=swarmTrailEffect(e.nativeId)){
        if(nativePowerFxRecord(*fx)){
            reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),fx->id);++swarmTrailStopped;swarmTrailResult="source_trail_stopped";
        }else{++swarmTrailFinished;++powerFxFinished;swarmTrailResult="native_trail_finished";}
        *fx={};
    }else{++swarmTrailFinished;swarmTrailResult="native_trail_already_finished";}
    e.nativeId=~0u; // sourceId is a tombstone until the whole world/scope changes.
}
void serviceSwarmTrails(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load()||!remoteIsCorvo()||!nativePowerFxApi())return;
    auto& s=swarmTrailPresentation;const auto& w=swarmWorldStream;
    const bool live=swarmWorldCurrent(tick)&&movementBankActive()&&coopGameplayReady(outgoing)&&coopGameplayReady(remote)&&w.lease>tick&&w.presented.present;
    if(!coopGameplayReady(outgoing))return; // Native pause retains ownership until cleanup can resume.
    if(!live){for(auto& e:s.entries)stopSwarmTrail(e);return;}
    if(s.token!=w.token||s.peerToken!=w.peerToken||s.event!=w.presented.event||s.world!=w.presented.nativeHandle){
        for(auto& e:s.entries)stopSwarmTrail(e);s={};
        s.token=w.token;s.peerToken=w.peerToken;s.event=w.presented.event;s.world=w.presented.nativeHandle;
    }
    for(unsigned index=0;index<s.entries.size();++index){auto& e=s.entries[index];CoopSwarmAgent rat{};float alpha=0;
        if(!swarmRatPresentation(w.presented,w.presentedTime,w.count?&w.pending[0].packet:nullptr,w.count?w.pending[0].time:0,
            presentationClock()-kSwarmPresentationDelayMs,index,rat,alpha)||!rat.trailId){stopSwarmTrail(e);continue;}
        if(e.sourceId&&(e.sourceId!=rat.trailId||e.handle!=rat.handle||e.incarnation!=rat.incarnation)){stopSwarmTrail(e);continue;}
        if(!e.sourceId){
            e.sourceId=rat.trailId;e.handle=rat.handle;e.incarnation=rat.incarnation;
            // Expired/missed starts cannot replay a lingering record or restart
            // after a transient gap; first admission consumes this source ID.
            if(rat.trailAgeMs>200||!startNativePowerFx(21,rat.trailTransform.data(),tick,true,rat.trailTransform.data()+3,&e.nativeId)){++swarmTrailRefused;swarmTrailLastRefusal=swarmTrailResult="start_refused";continue;}
            ++swarmTrailStarted;
        }
        auto* fx=swarmTrailEffect(e.nativeId);if(!fx){stopSwarmTrail(e);continue;}
        const auto record=nativePowerFxRecord(*fx);std::uintptr_t callback=1;
        // Native particles retire their own record after the authored lifetime;
        // this is expected completion, not a failed binding or a restart cue.
        if(!record){stopSwarmTrail(e);continue;}
        if(!memoryRead(record+0x128,&callback,8)||callback||fx->actor!=probeActor||fx->owner!=probeHandle||
            fx->streamToken!=shadowStream.token||fx->peerToken!=shadowStream.peerToken){stopSwarmTrail(e);++swarmTrailRefused;swarmTrailLastRefusal=swarmTrailResult="owned_record_guard_refused";continue;}
        // Same owned-record transform fields used by native initialization and
        // the existing verified hand effect path. No first-person callback.
        std::memcpy(reinterpret_cast<void*>(record+0x38),rat.trailTransform.data(),48);
        std::array<float,12> readback{};if(!memoryRead(record+0x38,readback.data(),48)){stopSwarmTrail(e);++swarmTrailRefused;swarmTrailLastRefusal=swarmTrailResult="readback_unavailable";continue;}
        double error=0;for(unsigned k=0;k<12;++k)error=(std::max)(error,std::fabs(double(readback[k])-rat.trailTransform[k]));
        if(!std::isfinite(error)||error>.00001){stopSwarmTrail(e);++swarmTrailRefused;swarmTrailLastRefusal=swarmTrailResult="readback_mismatch";continue;}
        swarmTrailMaximumReadbackError=(std::max)(swarmTrailMaximumReadbackError,error);e.pose=rat.trailTransform;
        for(unsigned k=0;k<3;++k)fx->handPoint[k]=rat.trailTransform[k];
        ++swarmTrailUpdated;if(alpha>0&&alpha<1)++swarmTrailInterpolated;swarmTrailResult="owned_trail_transform_verified";
    }
}
void writeSwarmTrails(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.swarm-trails.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    unsigned active=0;for(const auto& e:swarmTrailPresentation.entries)if(swarmTrailEffect(e.nativeId))++active;
    fprintf(f,"{\"sourceScans\":%llu,\"sourceDiscovered\":%llu,\"sourceSamples\":%llu,\"sourceReadFailures\":%llu,\"readMaxMs\":%.6f,\"started\":%llu,\"stopped\":%llu,\"finished\":%llu,\"result\":\"%s\",\"lastRefusal\":\"%s\",\"refused\":%llu,\"updated\":%llu,\"interpolated\":%llu,\"maxReadbackError\":%.9f,\"active\":%u,\"effects\":[",
        swarmTrailScans,swarmTrailDiscovered,swarmTrailSourceSamples,swarmTrailReadFailures,swarmTrailReadMaxMs,swarmTrailStarted,swarmTrailStopped,swarmTrailFinished,swarmTrailResult,swarmTrailLastRefusal,swarmTrailRefused,swarmTrailUpdated,swarmTrailInterpolated,swarmTrailMaximumReadbackError,active);
    bool first=true;for(unsigned i=0;i<swarmTrailPresentation.entries.size();++i){const auto& e=swarmTrailPresentation.entries[i];if(!e.sourceId)continue;
        fprintf(f,"%s{\"index\":%u,\"sourceId\":%u,\"nativeId\":%u,\"handle\":%u,\"incarnation\":%u,\"active\":%s,\"pose\":[",first?"":",",i,e.sourceId,e.nativeId,e.handle,e.incarnation,swarmTrailEffect(e.nativeId)?"true":"false");first=false;
        for(unsigned k=0;k<12;++k)fprintf(f,"%s%.6f",k?",":"",e.pose[k]);fputs("]}",f);
    }
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

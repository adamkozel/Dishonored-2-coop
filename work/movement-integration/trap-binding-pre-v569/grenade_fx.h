#pragma once
// Native-derived particles/light/audio on the exact cosmetic held grenade.
// This never arms a gameplay grenade or calls weapon/fire/explosion code.
NativePowerFx grenadeEffect;
bool grenadeFxAttempted=false;
std::uint64_t grenadeFxStarted=0,grenadeFxStopped=0,grenadeFxFinished=0,grenadeFxRetired=0,grenadeFxRefused=0,grenadeFxUpdated=0;
const char* grenadeFxResult="idle";
bool grenadeFxTransform(float* point,float* axis){
    NativeSkeleton s{};NativeCosmeticRanged prop{};
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!grenadeProp.network||!readNativeCosmeticRanged(memoryRead,base,observedGame,grenadeProp.handle,prop)||prop.kind!=3||
       prop.actor!=grenadeProp.actor||prop.owner!=probeActor||prop.ownerHandle!=probeHandle||
       !readNativeSkeleton(memoryRead,base,grenadeProp.handle,s,NativeSkeletonScope::CosmeticRanged)||!s.stable||
       s.actor!=prop.actor||s.game!=observedGame||s.logic!=1||s.parents.size()!=6||s.parents[0]!=-1)return false;
    const auto& j=s.globalPose[0];
    for(unsigned r=0;r<3;++r){point[r]=s.root[r];for(unsigned k=0;k<3;++k)point[r]+=s.root[3+k*3+r]*j[12+k];
        for(unsigned c=0;c<3;++c){axis[c*3+r]=0;for(unsigned k=0;k<3;++k)axis[c*3+r]+=s.root[3+k*3+r]*j[c*4+k];}}
    for(unsigned k=0;k<3;++k)if(!std::isfinite(point[k])||std::fabs(point[k])>100000)return false;
    for(unsigned k=0;k<9;++k)if(!std::isfinite(axis[k])||std::fabs(axis[k])>1.001f)return false;
    return true;
}
void serviceGrenadeFx(ULONGLONG tick){
    if(!nativePowerFxApi())return;
    auto& e=grenadeEffect;
    if(e.expires&&(e.game!=observedGame||e.epoch!=outgoing.worldEpoch)){e={};grenadeFxAttempted=false;++grenadeFxRetired;}
    if(!coopGameplayReady(outgoing))return; // Defer native cleanup until resume.
    unsigned variant=0;bool wanted=false;
    AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;
    variant=p.grenadeVariant;
    wanted=p.end>tick&&p.lease>tick&&p.game==observedGame&&p.epoch==outgoing.worldEpoch&&p.actor==probeActor&&p.grenadeWeight>.9f&&
        (variant==4||variant==5||variant==7)&&grenadeProp.network&&grenadeProp.until>tick&&!grenadeProp.pendingHide&&
        shadowStreamCurrent(tick)&&shadowStream.peerReady&&shadowStream.lastState&&tick>=shadowStream.lastState&&tick-shadowStream.lastState<250&&
        coopGameplayReady(remote)&&observerIsolation.ready&&observerIsolation.movementFixture;
    ReleaseSRWLockShared(&movementLock);
    const bool semanticWanted=wanted;
    float point[3]{},axis[9]{};
    if(wanted&&!grenadeFxTransform(point,axis)){wanted=false;++grenadeFxRefused;grenadeFxResult="transform_unavailable";}
    if(e.expires){
        const auto record=nativePowerFxRecord(e);std::uintptr_t callback=1;
        const bool same=e.actor==grenadeProp.actor&&e.owner==grenadeProp.handle&&e.streamToken==shadowStream.token&&e.peerToken==shadowStream.peerToken;
        if(!record){e={};++grenadeFxFinished;grenadeFxResult="native_effect_finished";}
        else if(!wanted||!same||tick>=e.expires||!memoryRead(record+0x128,&callback,8)||callback){
            reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),e.id);
            e={};++grenadeFxStopped;grenadeFxResult="owned_grenade_effect_stopped";
        }else{
            // Same owned-record fields as the validated hand-following effect.
            std::memcpy(reinterpret_cast<void*>(record+0x38),point,12);std::memcpy(reinterpret_cast<void*>(record+0x44),axis,36);
            for(unsigned k=0;k<3;++k)e.handPoint[k]=point[k];e.expires=tick+250;++grenadeFxUpdated;
        }
    }
    if(!semanticWanted)grenadeFxAttempted=false;
    if(!wanted)return;
    if(e.expires||grenadeFxAttempted||variant!=4)return;
    // One attempt per continuous held-cooking presentation, not per packet or
    // looping arm clip. Never duplicate an ambiguously returned native effect.
    grenadeFxAttempted=true;
    NativePowerFx next{};next.game=observedGame;next.actor=grenadeProp.actor;next.owner=grenadeProp.handle;next.epoch=outgoing.worldEpoch;
    next.kind=12;next.streamToken=shadowStream.token;next.peerToken=shadowStream.peerToken;next.declaration=nativePowerFxDefinition(12);
    if(!next.declaration){++grenadeFxRefused;grenadeFxResult="definition_unavailable";return;}
    using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
    reinterpret_cast<Start>(gameImage+0x87cb90)(next.actor,&next.id,0,0,0,next.declaration,point,axis,0,0,0,0);
    if(next.id==~0u){++grenadeFxRefused;grenadeFxResult="native_start_failed";return;}
    next.expires=tick+250;next.record=nativePowerFxRecord(next);for(unsigned k=0;k<3;++k)next.handPoint[k]=point[k];e=next;++grenadeFxStarted;
    grenadeFxResult=next.record?"owned_grenade_effect":"ownership_unconfirmed";
}
void writeGrenadeFx(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.grenade-fx.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& e=grenadeEffect;
    fprintf(f,"{\"active\":%s,\"owner\":%u,\"id\":%u,\"started\":%llu,\"stopped\":%llu,\"finished\":%llu,\"retired\":%llu,\"refused\":%llu,\"updated\":%llu,\"origin\":[%.6f,%.6f,%.6f],\"result\":\"%s\"}\n",e.expires?"true":"false",e.owner,e.id,grenadeFxStarted,grenadeFxStopped,grenadeFxFinished,grenadeFxRetired,grenadeFxRefused,grenadeFxUpdated,e.handPoint[0],e.handPoint[1],e.handPoint[2],grenadeFxResult);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

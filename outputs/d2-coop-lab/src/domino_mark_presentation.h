#pragma once
// Reuse the native validated mark particles on an owned presentation actor.
// No native NPC, attribute, selected-node or Domino gameplay mutation occurs.
struct DominoPresentedMark {NativePowerFx fx{};unsigned sourceId=0,target=0;std::uint64_t token=0,peerToken=0,event=0;};
std::array<DominoPresentedMark,4> dominoMarks{};
std::uint64_t dominoMarkStarted=0,dominoMarkStopped=0,dominoMarkUpdated=0,dominoMarkRefused=0,dominoMarkRetired=0,dominoMarkSequence=0;
unsigned dominoReadyMask=0;
// Why the observer does not present the partner's Domino marks and beams (0: presented). Bits: 1 not the host, 2 stream
// not current, 4 lease expired, 8 no movement bank, 16 partner not in gameplay, 32 neither lab fixture nor armed product
// session, 64 no native partner avatar, 128 partner AI not held, 256 follow not exact, 512 AI target is not the avatar,
// 1024 avatar identity changed.
unsigned dominoPresentationReadyMask(ULONGLONG tick){
    const auto& stream=dominoWorldStream;unsigned m=0;
    if(remoteIsCorvo())m|=1;if(!dominoWorldCurrent(tick))m|=2;if(stream.lease<=tick)m|=4;if(!movementBankActive())m|=8;if(!coopGameplayReady(remote))m|=16;
    if(!((!std::strcmp(mapName,kGuardMap)&&observerIsolation.ready&&observerIsolation.movementFixture)||productSessionArmed()))m|=32;
    if(!probeDoppelganger)m|=64;if(!avatarAiHeld)m|=128;if(!probeFollowExact)m|=256;if(avatarAiTarget.actor!=probeActor)m|=512;
    if(!m&&worldIdentityEntity(memoryRead,observedGame,probeHandle)!=probeActor)m|=1024;
    return m;
}
void stopDominoMark(DominoPresentedMark& mark){
    if(!mark.sourceId)return;
    if(mark.fx.game!=observedGame||mark.fx.epoch!=outgoing.worldEpoch){mark={};++dominoMarkRetired;return;}
    if(const auto record=nativePowerFxRecord(mark.fx))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),mark.fx.id);
    mark={};++dominoMarkStopped;
}
void serviceDominoMarks(ULONGLONG tick){
    if(!nativePowerFxApi())return;
    for(auto& mark:dominoMarks)if(mark.sourceId&&(mark.fx.game!=observedGame||mark.fx.epoch!=outgoing.worldEpoch)){mark={};++dominoMarkRetired;}
    if(!coopGameplayReady(outgoing))return; // Native pause keeps its world frozen; clean on resume.
    const auto& stream=dominoWorldStream;const auto& p=stream.presented;
    // 1214cr (#11): marks and beams are also presented in an armed product session on any map (as the clone and spirit
    // since 1214cp); the lab fixture scope is unchanged. Both stay cosmetic: no native NPC, link or damage call.
    dominoReadyMask=dominoPresentationReadyMask(tick);const bool ready=!dominoReadyMask;
    const bool snapshot=ready&&p.sequence!=dominoMarkSequence;
    for(auto& mark:dominoMarks)if(mark.sourceId){
        bool retain=ready&&mark.fx.actor==probeActor&&mark.fx.owner==probeHandle&&mark.token==stream.token&&mark.peerToken==stream.peerToken&&mark.event==p.event;
        if(retain&&snapshot){retain=false;for(unsigned i=0;i<p.count;++i)if(p.marks[i].id==mark.sourceId&&p.marks[i].target==mark.target)retain=true;}
        if(!retain)stopDominoMark(mark);
    }
    if(!snapshot)return;dominoMarkSequence=p.sequence;
    NativeSwordEquipment sword{};
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,sword,NativeSwordScope::DoppelgangerEmily)||
       sword.actor!=probeActor||sword.game!=observedGame){++dominoMarkRefused;return;}
    for(unsigned i=0;i<p.count;++i){const auto& source=p.marks[i];DominoPresentedMark* slot=nullptr;
        for(auto& mark:dominoMarks)if(mark.sourceId==source.id&&mark.target==source.target){slot=&mark;break;}
        if(!slot){for(auto& mark:dominoMarks)if(!mark.sourceId){slot=&mark;break;}if(!slot){++dominoMarkRefused;return;}
            slot->sourceId=source.id;slot->target=source.target;slot->token=stream.token;slot->peerToken=stream.peerToken;slot->event=p.event;
            auto& effect=slot->fx;effect.game=observedGame;effect.epoch=outgoing.worldEpoch;effect.actor=probeActor;effect.owner=probeHandle;effect.kind=18;
            effect.declaration=nativePowerFxDefinition(18);if(!effect.declaration){*slot={};++dominoMarkRefused;continue;}
            using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
            reinterpret_cast<Start>(gameImage+0x87cb90)(effect.actor,&effect.id,0,0,0,effect.declaration,source.transform.data(),source.transform.data()+3,0,0,0,0);
            if(effect.id==~0u){*slot={};++dominoMarkRefused;continue;}
            effect.expires=tick+300;effect.record=nativePowerFxRecord(effect);++dominoMarkStarted;
        }
        const auto record=nativePowerFxRecord(slot->fx);std::uintptr_t callback=1;
        if(!record||!memoryRead(record+0x128,&callback,8)||callback){stopDominoMark(*slot);++dominoMarkRefused;continue;}
        std::memcpy(reinterpret_cast<void*>(record+0x38),source.transform.data(),48);slot->fx.expires=tick+300;++dominoMarkUpdated;
    }
}
void writeDominoWorld(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.domino-world.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& s=dominoWorldStream;
    fprintf(f,"{\"readyMask\":%u,\"marksOnly\":%llu,\"sent\":%llu,\"received\":%llu,\"rejected\":%llu,\"unknown\":%llu,\"presented\":%llu,\"event\":%llu,\"nativeHandle\":%u,\"markCount\":%u,\"leased\":%s,\"readCount\":%llu,\"scopedReadCount\":%llu,\"readMaxMs\":%.4f,\"scopedReadMaxMs\":%.4f,\"readStage\":%u,\"started\":%llu,\"stopped\":%llu,\"updated\":%llu,\"refused\":%llu,\"retired\":%llu,\"marks\":[",dominoReadyMask,dominoWorldMarksOnly,dominoWorldSent,dominoWorldReceived,dominoWorldRejected,dominoWorldUnknown,dominoWorldPresented,s.presented.event,s.presented.nativeHandle,s.presented.count,s.lease>GetTickCount64()?"true":"false",dominoReadCount,dominoScopedReadCount,dominoReadMaxMs,dominoScopedReadMaxMs,dominoReadStage,dominoMarkStarted,dominoMarkStopped,dominoMarkUpdated,dominoMarkRefused,dominoMarkRetired);
    unsigned n=0;for(const auto& m:dominoMarks)if(m.sourceId)fprintf(f,"%s{\"sourceId\":%u,\"target\":%u,\"id\":%u,\"owner\":%u}",n++?",":"",m.sourceId,m.target,m.fx.id,m.fx.owner);
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

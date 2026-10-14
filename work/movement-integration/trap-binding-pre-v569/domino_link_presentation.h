#pragma once
// Only observed native beam FX are recreated. No connection/NPC/gameplay API.
struct DominoPresentedBeam {NativePowerFx fx{};unsigned sourceId=0,from=0,to=0;std::uint64_t token=0,peerToken=0,event=0;};
std::array<DominoPresentedBeam,6> dominoBeams{};
std::uint64_t dominoBeamStarted=0,dominoBeamStopped=0,dominoBeamUpdated=0,dominoBeamRefused=0,dominoBeamRetired=0,dominoBeamSequence=0;
void stopDominoBeam(DominoPresentedBeam& beam){
    if(!beam.sourceId)return;
    if(beam.fx.game!=observedGame||beam.fx.epoch!=outgoing.worldEpoch){beam={};++dominoBeamRetired;return;}
    if(nativePowerFxRecord(beam.fx))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),beam.fx.id);
    beam={};++dominoBeamStopped;
}
void serviceDominoBeams(ULONGLONG tick){
    if(!nativePowerFxApi())return;
    for(auto& beam:dominoBeams)if(beam.sourceId&&(beam.fx.game!=observedGame||beam.fx.epoch!=outgoing.worldEpoch)){beam={};++dominoBeamRetired;}
    if(!coopGameplayReady(outgoing))return;
    const auto& stream=dominoWorldStream;const auto& p=stream.presented;
    const bool ready=localRole==0&&dominoWorldCurrent(tick)&&stream.lease>tick&&movementBankActive()&&coopGameplayReady(remote)&&
        !std::strcmp(mapName,kGuardMap)&&observerIsolation.ready&&observerIsolation.movementFixture&&probeDoppelganger&&avatarAiHeld&&probeFollowExact&&
        avatarAiTarget.actor==probeActor&&worldIdentityEntity(memoryRead,observedGame,probeHandle)==probeActor;
    const bool snapshot=ready&&p.sequence!=dominoBeamSequence;
    for(auto& beam:dominoBeams)if(beam.sourceId){
        bool retain=ready&&beam.fx.actor==probeActor&&beam.fx.owner==probeHandle&&beam.token==stream.token&&beam.peerToken==stream.peerToken&&beam.event==p.event;
        if(retain&&snapshot){retain=false;for(unsigned i=0;i<p.beamCount;++i)if(p.beams[i].id==beam.sourceId&&p.beams[i].from==beam.from&&p.beams[i].to==beam.to)retain=true;}
        if(!retain)stopDominoBeam(beam);
    }
    if(!snapshot)return;dominoBeamSequence=p.sequence;
    NativeSwordEquipment sword{};
    if(!readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,sword,NativeSwordScope::DoppelgangerEmily)||
       sword.actor!=probeActor||sword.game!=observedGame){++dominoBeamRefused;return;}
    for(unsigned i=0;i<p.beamCount;++i){const auto& source=p.beams[i];DominoPresentedBeam* slot=nullptr;
        for(auto& beam:dominoBeams)if(beam.sourceId==source.id&&beam.from==source.from&&beam.to==source.to){slot=&beam;break;}
        if(!slot){for(auto& beam:dominoBeams)if(!beam.sourceId){slot=&beam;break;}if(!slot){++dominoBeamRefused;return;}
            slot->sourceId=source.id;slot->from=source.from;slot->to=source.to;slot->token=stream.token;slot->peerToken=stream.peerToken;slot->event=p.event;
            auto& effect=slot->fx;effect.game=observedGame;effect.epoch=outgoing.worldEpoch;effect.actor=probeActor;effect.owner=probeHandle;effect.kind=19;
            effect.declaration=nativePowerFxDefinition(19);if(!effect.declaration){*slot={};++dominoBeamRefused;continue;}
            using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
            reinterpret_cast<Start>(gameImage+0x87cb90)(effect.actor,&effect.id,0,0,0,effect.declaration,source.start.data(),source.start.data()+3,0,0,0,0);
            if(effect.id==~0u){*slot={};++dominoBeamRefused;continue;}
            effect.expires=tick+300;effect.record=nativePowerFxRecord(effect);++dominoBeamStarted;
        }
        const auto record=nativePowerFxRecord(slot->fx);std::uintptr_t callback=1;
        if(!record||!memoryRead(record+0x128,&callback,8)||callback){stopDominoBeam(*slot);++dominoBeamRefused;continue;}
        // Same native record layout populated by 1404e2de0. This record belongs
        // exclusively to this presentation slot and current local avatar.
        std::memcpy(reinterpret_cast<void*>(record+0x38),source.start.data(),48);
        std::memcpy(reinterpret_cast<void*>(record+0x80),source.end.data(),48);
        slot->fx.expires=tick+300;++dominoBeamUpdated;
    }
}
void writeDominoBeams(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.domino-links.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    fprintf(f,"{\"protocol\":2,\"started\":%llu,\"stopped\":%llu,\"updated\":%llu,\"refused\":%llu,\"retired\":%llu,\"sequence\":%llu,\"beams\":[",dominoBeamStarted,dominoBeamStopped,dominoBeamUpdated,dominoBeamRefused,dominoBeamRetired,dominoBeamSequence);
    unsigned n=0;for(const auto& b:dominoBeams)if(b.sourceId)fprintf(f,"%s{\"sourceId\":%u,\"from\":%u,\"to\":%u,\"id\":%u,\"owner\":%u}",n++?",":"",b.sourceId,b.from,b.to,b.fx.id,b.fx.owner);
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

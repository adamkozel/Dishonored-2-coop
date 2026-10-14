#pragma once
// Cosmetic, leased native arm/hand particles. The accepted body timeline owns
// phase and the actual published peer rig owns placement. No source camera FX,
// damage, activation, character mesh or gameplay-state calls occur here.
struct ShadowWalkFx {
    struct Slot{unsigned id=~0u,kind=0,joint=0;std::uint64_t token=0,peer=0;};
    std::array<Slot,4> slots{};bool moving=false;unsigned variant=0;
    std::uint64_t started=0,stopped=0,updates=0,refused=0;
} shadowWalkFx;
NativePowerFx* shadowWalkOwnedFx(const ShadowWalkFx::Slot& s){
    for(auto& e:nativePowerEffects)if(e.expires&&e.id==s.id&&e.kind==s.kind&&e.streamToken==s.token&&e.peerToken==s.peer)return &e;
    return nullptr;
}
void stopShadowWalkFxSlot(ShadowWalkFx::Slot& s){
    if(auto* e=shadowWalkOwnedFx(s)){
        if(nativePowerFxRecord(*e))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),e->id);
        *e={};++shadowWalkFx.stopped;
    }
    s={};
}
void serviceShadowWalkFx(ULONGLONG tick){
    if(!nativePowerFxApi()||!nativeMutationAllowed||!coopGameplayReady(outgoing))return;
    auto& fx=shadowWalkFx;unsigned variant=0;float speed=0;std::uintptr_t skeleton=0;
    AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;
    if(localRole==0&&p.end>tick&&p.lease>tick&&p.bank.hasShadowWalk()&&p.actor==probeActor&&p.handle==probeHandle&&
       p.clock.weight>.9f&&p.powerWeight>.9f&&p.powerVariant>=32&&p.powerVariant<=34&&!p.presentedDrawn){
        variant=p.powerVariant;speed=p.clock.speed;skeleton=p.skeleton;
    }
    ReleaseSRWLockShared(&movementLock);
    NativeSkeleton s{};
    const bool valid=variant&&shadowStreamCurrent(tick)&&shadowStream.peerReady&&movementBankActive()&&
        readNativeSkeleton(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,s)&&
        s.stable&&s.logic==1&&s.actor==probeActor&&s.game==observedGame&&s.skeleton==skeleton&&bodyShadowTarget(s)&&
        s.parents.size()==219&&s.globalPose.size()==219&&s.parents[8]==7&&s.parents[9]==8&&s.parents[15]==14&&s.parents[16]==15;
    if(!valid){for(auto& slot:fx.slots)stopShadowWalkFxSlot(slot);fx.variant=0;fx.moving=false;return;}
    fx.variant=variant;if(speed>.15f)fx.moving=true;else if(speed<.05f)fx.moving=false;
    const unsigned joints[]={8,15,9,16};
    const unsigned kinds[]={variant==33?(fx.moving?25u:23u):28u,variant==33?(fx.moving?26u:23u):28u,
        variant==33?(fx.moving?27u:24u):0u,variant==33?(fx.moving?27u:24u):0u};
    for(unsigned i=0;i<fx.slots.size();++i){auto& slot=fx.slots[i];const auto joint=joints[i],kind=kinds[i];
        if(slot.kind&&(slot.kind!=kind||slot.joint!=joint||slot.token!=shadowStream.token||slot.peer!=shadowStream.peerToken))stopShadowWalkFxSlot(slot);
        if(!kind)continue;
        float point[3]{},axis[9]{};const auto& hand=s.globalPose[joint];bool finite=true;
        for(unsigned r=0;r<3;++r){point[r]=s.root[r];for(unsigned k=0;k<3;++k)point[r]+=s.root[3+k*3+r]*hand[12+k];
            for(unsigned c=0;c<3;++c)for(unsigned k=0;k<3;++k)axis[c*3+r]+=s.root[3+k*3+r]*hand[c*4+k];}
        for(float v:point)finite=finite&&std::isfinite(v)&&std::fabs(v)<100000;
        for(float v:axis)finite=finite&&std::isfinite(v)&&std::fabs(v)<1.001f;
        if(!finite){stopShadowWalkFxSlot(slot);++fx.refused;continue;}
        auto* effect=shadowWalkOwnedFx(slot);
        if(!effect){unsigned id=~0u;
            if(!startNativePowerFx(kind,point,tick,true,axis,&id)){++fx.refused;continue;}
            slot={id,kind,joint,shadowStream.token,shadowStream.peerToken};effect=shadowWalkOwnedFx(slot);++fx.started;
        }
        const auto record=effect?nativePowerFxRecord(*effect):0;std::uintptr_t callback=1;
        if(!record||effect->actor!=s.actor||effect->owner!=s.handle||!memoryRead(record+0x128,&callback,8)||callback){stopShadowWalkFxSlot(slot);++fx.refused;continue;}
        // Same verified native world-origin/axis fields used by the tattoo
        // effect. Only this slot's exact generation/token-owned record moves.
        std::memcpy(reinterpret_cast<void*>(record+0x38),point,12);std::memcpy(reinterpret_cast<void*>(record+0x44),axis,36);
        std::copy(point,point+3,effect->handPoint.begin());effect->expires=tick+350;++fx.updates;
    }
}
void writeShadowWalkFx(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.shadowwalk-fx.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& x=shadowWalkFx;
    fprintf(f,"{\"variant\":%u,\"moving\":%s,\"started\":%llu,\"stopped\":%llu,\"updates\":%llu,\"refused\":%llu,\"slots\":[",x.variant,x.moving?"true":"false",x.started,x.stopped,x.updates,x.refused);
    for(unsigned i=0;i<x.slots.size();++i){const auto& s=x.slots[i];fprintf(f,"%s{\"id\":%u,\"kind\":%u,\"joint\":%u}",i?",":"",s.id,s.kind,s.joint);}
    fprintf(f,"]}\n");fclose(f);MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

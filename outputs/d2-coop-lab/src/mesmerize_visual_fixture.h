#pragma once
#include "mesmerize_visual_scene.h"
// Finite, isolated animation-only proof. This does not grant Mesmerize or run
// its attention/FSM/damage components. Native event tracks are removed in the
// separately hashed asset build. Only one cached model per world is allowed.
struct MesmerizeVisualEye {NativePowerFx effect{};unsigned sourceId=0;ULONGLONG until=0;};
struct MesmerizeVisualLink {NativePowerFx effect{};unsigned sourceId=0,target=0,kind=0;ULONGLONG until=0;};
std::uint64_t mesmerizeEyeStarted=0,mesmerizeEyeStopped=0,mesmerizeEyeUpdated=0,mesmerizeEyeRefused=0;
struct MesmerizeVisualFixture {
    MesmerizeVisualTarget target{};MesmerizeVisualScene scene{};NativePowerFx effect{};bool effectAttempted=false;
    std::uintptr_t game=0,definition=0;std::uint64_t epoch=0;
    ULONGLONG until=0,readyAfter=0;unsigned wanted=0,playing=0;bool pendingHide=false,network=false;std::uint64_t event=0,streamToken=0,peerToken=0;
    std::array<MesmerizeVisualEye,5> eyes{};std::uint64_t eyeSequence=0;
    std::array<MesmerizeVisualLink,15> links{};std::uint64_t linkSequence=0;
} mesmerizeVisual;
unsigned mesmerizeVisualRequest=0;
std::uint64_t mesmerizeVisualFxStarted=0,mesmerizeVisualFxStopped=0;
std::uint64_t mesmerizeVisualCreated=0,mesmerizeVisualStarted[3]{},mesmerizeVisualHidden=0,mesmerizeVisualRefused=0,mesmerizeVisualRetired=0;
const char* mesmerizeVisualResult="idle";
const char* mesmerizeVisualError="";
constexpr const char* kMesmerizeVisualDefinition="models/powers/d2coop/mesmerize_visual.def";
bool mesmerizeVisualIdentity(){
    const auto& p=mesmerizeVisual;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vt=0,d=0,n=0,list=0;unsigned count=0,parent=0;std::string text;
    if(!p.target.actor||p.game!=observedGame||p.epoch!=outgoing.worldEpoch||
       worldIdentityEntity(memoryRead,p.game,p.target.handle)!=p.target.actor||
       !memoryRead(p.target.actor,&vt,8)||vt!=base+0x1c87d58||
       !memoryRead(p.target.actor+0xac,&parent,4)||parent!=0x3fff0010||
       !memoryRead(p.target.actor+0x2e8,&d,8)||d!=p.definition||
       !memoryRead(d,&vt,8)||vt!=base+0x1ab58f8||!memoryRead(d+8,&n,8)||
       !worldIdentityString(memoryRead,n,text)||text!=kMesmerizeVisualDefinition||
       !memoryRead(p.target.actor+0x268,&list,8)||!list||!memoryRead(p.target.actor+0x270,&count,4)||(count&0xffffff)!=4)return false;
    constexpr const char* allowed[]={"components/interactive/animevolved_midnight.cpntanimevolved",
        "components/base/midnightproxy_default.cpntmidnightproxy","components/base/audio_default.cpntaudio",
        "components/base/referenceframe_default.cpntreferenceframe"};
    unsigned seen=0;
    for(unsigned i=0;i<4;++i){std::uintptr_t c=0,owner=0,decl=0;
        if(!memoryRead(list+i*8,&c,8)||!c||!memoryRead(c+0x10,&owner,8)||owner!=p.target.actor||
           !memoryRead(c+8,&decl,8)||!decl||!memoryRead(decl+8,&n,8)||!worldIdentityString(memoryRead,n,text))return false;
        unsigned k=0;while(k<4&&text!=allowed[k])++k;
        if(k==4||(seen&(1u<<k)))return false;seen|=1u<<k;
        if(!memoryRead(c,&vt,8)||vt<base+0x1a00000||vt>=base+0x1e00000)return false;
        if((k==0&&vt!=base+0x1b80408)||(k==1&&vt!=base+0x1c59ac8)||(k==2&&vt!=base+0x1c4e588)||(k==3&&vt!=base+0x1c573e8))return false;
    }
    return seen==15&&worldIdentityEntity(memoryRead,p.game,p.target.handle)==p.target.actor;
}
bool stopMesmerizeEye(MesmerizeVisualEye& eye){
    if(!eye.sourceId)return true;if(!nativePowerFxApi())return false;
    if(eye.effect.expires){if(nativePowerFxRecord(eye.effect))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),eye.effect.id);++mesmerizeEyeStopped;}
    eye={};return true;
}
bool serviceMesmerizeEyes(ULONGLONG tick,bool automatic){
    auto& p=mesmerizeVisual;const auto& packet=mesmerizeWorldStream.presented;
    if(!automatic){for(auto& eye:p.eyes)if(!stopMesmerizeEye(eye))return false;return true;}
    const bool snapshot=packet.eyesValid&&packet.sequence!=p.eyeSequence;
    for(auto& eye:p.eyes)if(eye.sourceId){
        bool retained=!snapshot;for(unsigned i=0;snapshot&&i<packet.eyeCount;++i)if(packet.eyes[i].id==eye.sourceId)retained=true;
        if(!retained||tick>=eye.until)if(!stopMesmerizeEye(eye))return false;
    }
    if(!snapshot)return true;p.eyeSequence=packet.sequence;
    for(unsigned i=0;i<packet.eyeCount;++i){const auto& source=packet.eyes[i];MesmerizeVisualEye* slot=nullptr;
        for(auto& eye:p.eyes)if(eye.sourceId==source.id){slot=&eye;break;}
        if(!slot){for(auto& eye:p.eyes)if(!eye.sourceId){slot=&eye;break;}if(!slot)return false;
            slot->sourceId=source.id;auto& effect=slot->effect;effect.game=p.game;effect.epoch=p.epoch;effect.actor=p.target.actor;effect.owner=p.target.handle;effect.kind=14;
            effect.declaration=nativePowerFxDefinition(14);if(!effect.declaration)return false;
            using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
            reinterpret_cast<Start>(gameImage+0x87cb90)(effect.actor,&effect.id,0,0,0,effect.declaration,source.transform.data(),source.transform.data()+3,0,0,0,0);
            if(effect.id==~0u)return false;effect.expires=tick+300;effect.record=nativePowerFxRecord(effect);++mesmerizeEyeStarted;
            if(!effect.record)return false;
        }
        const auto record=nativePowerFxRecord(slot->effect);if(!record)return false;
        // Exact native 14072e5c0 updates these fields. This slot is owned by
        // the animation-only model, never the original power/NPC effect.
        std::memcpy(reinterpret_cast<void*>(record+0x38),source.transform.data(),48);
        slot->until=tick+300;slot->effect.expires=slot->until;++mesmerizeEyeUpdated;
    }
    return true;
}
void writeMesmerizeEyes(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.mesmerize-eyes.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& p=mesmerizeVisual;
    fprintf(f,"{\"started\":%llu,\"stopped\":%llu,\"updated\":%llu,\"refused\":%llu,\"sequence\":%llu,\"eyes\":[",mesmerizeEyeStarted,mesmerizeEyeStopped,mesmerizeEyeUpdated,mesmerizeEyeRefused,p.eyeSequence);
    unsigned n=0;for(const auto& eye:p.eyes)if(eye.sourceId)fprintf(f,"%s{\"sourceId\":%u,\"id\":%u,\"owner\":%u,\"active\":%s}",n++?",":"",eye.sourceId,eye.effect.id,eye.effect.owner,eye.until>GetTickCount64()?"true":"false");
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
#include "mesmerize_target_presentation.h"
void serviceMesmerizeVisual(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    auto& p=mesmerizeVisual;unsigned request=mesmerizeVisualRequest;mesmerizeVisualRequest=0;
    const auto refuse=[&](const char* why){++mesmerizeVisualRefused;mesmerizeVisualResult=why;mesmerizeVisualError=why;};
    if(p.target.actor&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){p={};++mesmerizeVisualRetired;mesmerizeVisualResult="world_retired";}
    const auto& stream=mesmerizeWorldStream;
    const bool automatic=!remoteIsCorvo()&&mesmerizeWorldStreamCurrent(tick)&&stream.lease>tick&&movementBankActive()&&coopGameplayReady(remote);
    const unsigned remoteVariant=stream.presented.state==1?1:stream.presented.state==3?2:3;
    const bool activeSnapshot=automatic&&stream.presented.snapshot==1&&stream.presented.state==3;
    if(p.network&&p.until&&(!automatic||p.event!=stream.presented.event||p.streamToken!=stream.token||p.peerToken!=stream.peerToken)){p.pendingHide=true;p.wanted=0;}
    if(automatic&&!request){
        if(p.network&&p.until&&!p.pendingHide&&p.event==stream.presented.event){p.until=tick+300;if(p.wanted!=remoteVariant)request=remoteVariant;}
        else if(!p.until&&!p.pendingHide&&(stream.presented.state==1||activeSnapshot)&&
                (p.event!=stream.presented.event||p.streamToken!=stream.token||p.peerToken!=stream.peerToken))request=activeSnapshot?2:1;
    }
    // 1214cp (#104): the networked spirit also runs in an armed product session; the local experiment stays lab-only.
    const bool productScope=automatic&&productSessionArmed(),fixtureScope=observerIsolation.ready&&observerIsolation.movementFixture;
    if(request==4||(p.until&&tick>=p.until)||!(fixtureScope||productScope)){p.wanted=0;if(p.until)p.pendingHide=true;}
    if(!coopGameplayReady(outgoing)||!powerFixtureApiMatches()||!rangedPropApiMatches()||!carryVictimApis())return;
    if(request>=1&&request<=3){
        const bool labScope=!std::strcmp(mapName,kGuardMap)&&fixtureScope&&observerIsolation.until>=tick+14000;
        if(!partnerAssetsEnabled||!(labScope||productScope)){refuse("unsupported_scope");return;}
        if(!p.target.actor){
            if(request!=1&&!(request==2&&activeSnapshot)){refuse("spawn_required");return;}
            const auto definition=lookupEntityDefinition(kMesmerizeVisualDefinition);if(!definition){refuse("visual_definition_missing");return;}
            unsigned char parameters[12]{};reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
            void* actor=reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(definition),parameters);
            if(!actor){refuse("native_creation_refused");return;}
            // Cache immediately, even if initialization cannot be confirmed.
            // An ambiguous result must never spawn a second model.
            p.game=observedGame;p.epoch=outgoing.worldEpoch;p.definition=definition;p.target.actor=reinterpret_cast<std::uintptr_t>(actor);++mesmerizeVisualCreated;
            float pos[3]{-33.6873f,-155.303f,66.3984f},axis[9]{1,0,0,0,1,0,0,0,1};
            if(automatic){std::memcpy(pos,stream.presented.transform.data(),12);std::memcpy(axis,stream.presented.transform.data()+3,36);}
            reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(actor,pos);
            reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(actor,axis);
            reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),actor,-1);
            memoryRead(p.target.actor+0xa8,&p.target.handle,4);
            reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(actor);
            p.readyAfter=tick+200;
        }
        if(!mesmerizeVisualIdentity()){refuse("visual_identity_refused");return;}
        if((request==1||(request==2&&activeSnapshot))&&!p.until){
            p.network=automatic;p.until=tick+(automatic?300:12000);
            if(automatic){
                p.event=stream.presented.event;p.streamToken=stream.token;p.peerToken=stream.peerToken;
                reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(reinterpret_cast<void*>(p.target.actor),stream.presented.transform.data());
                reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(reinterpret_cast<void*>(p.target.actor),stream.presented.transform.data()+3);
            }
        }
        if(!p.until||p.pendingHide){refuse("inactive_or_retiring");return;}
        p.wanted=request;
    }
    if(!p.target.actor||(!p.until&&!p.pendingHide))return;
    if(!mesmerizeVisualIdentity()){refuse("owned_model_identity_changed");return;}
    if(p.scene.handle&&(p.pendingHide||p.playing!=p.wanted)){
        if(!stopMesmerizeVisualScene(p.scene,p.target))return;p.playing=0;
    }
    if(p.pendingHide){
        if(!serviceMesmerizeEyes(tick,false))return;
        if(!serviceMesmerizeLinks(tick,false))return;
        if(p.effect.expires){
            if(!nativePowerFxApi())return;
            if(nativePowerFxRecord(p.effect))reinterpret_cast<void(*)(std::uintptr_t,unsigned)>(gameImage+0x1a31f0)(nativePowerFxManager(),p.effect.id);
            p.effect={};++mesmerizeVisualFxStopped;
        }
        reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(p.target.actor));
        unsigned char flags=0;if(memoryRead(p.target.actor+0xd0,&flags,1)&&(flags&1)){p.pendingHide=false;p.until=0;p.playing=0;p.effectAttempted=false;++mesmerizeVisualHidden;mesmerizeVisualResult="native_visual_hidden";}return;
    }
    if(p.wanted&&p.playing!=p.wanted){
        if(tick<p.readyAfter)return;
        // Native Mesmerize state1 shows its entity before starting Midnight.
        reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(p.target.actor));
        reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(p.target.actor));
        if(!p.effectAttempted){
            p.effectAttempted=true;NativePowerFx e{};e.game=p.game;e.epoch=p.epoch;e.actor=p.target.actor;e.owner=p.target.handle;e.kind=13;
            e.declaration=nativePowerFxDefinition(13);float transform[12]{};
            if(!e.declaration||!memoryRead(p.target.actor+0x300,transform,48)){p.pendingHide=true;refuse("native_body_effect_definition_missing");return;}
            using Start=unsigned*(*)(std::uintptr_t,unsigned*,std::uintptr_t,std::uintptr_t,std::uintptr_t,std::uintptr_t,const float*,const float*,std::uintptr_t,unsigned,std::uintptr_t,std::uintptr_t);
            reinterpret_cast<Start>(gameImage+0x87cb90)(e.actor,&e.id,0,0,0,e.declaration,transform,transform+3,0,0,0,0);
            if(e.id==~0u){p.pendingHide=true;refuse("native_body_effect_start_failed");return;}
            e.expires=p.until;e.record=nativePowerFxRecord(e);p.effect=e;++mesmerizeVisualFxStarted;
            if(!e.record){p.pendingHide=true;refuse("native_body_effect_identity_unconfirmed");return;}
        }
        const auto wanted=p.wanted;p.playing=wanted; // one attempt, even on ambiguous start
        if(!startMesmerizeVisualScene(p.scene,p.target,wanted)){p.pendingHide=true;refuse("native_scene_unconfirmed");return;}
        reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(p.target.actor));
        reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(p.target.actor));
        ++mesmerizeVisualStarted[wanted-1];mesmerizeVisualResult="native_visual_scene_started";
    }
    if(!p.pendingHide&&p.playing&&!serviceMesmerizeEyes(tick,automatic)){++mesmerizeEyeRefused;p.pendingHide=true;refuse("native_eye_identity_unconfirmed");}
    if(!p.pendingHide&&p.playing&&!serviceMesmerizeLinks(tick,automatic)){++mesmerizeLinkRefused;p.pendingHide=true;refuse("native_link_identity_unconfirmed");}
}
void writeMesmerizeVisual(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.mesmerize-visual.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& p=mesmerizeVisual;
    fprintf(f,"{\"localExperiment\":true,\"active\":%s,\"pendingHide\":%s,\"handle\":%u,\"scene\":%u,\"playing\":%u,\"wanted\":%u,\"created\":%llu,\"started\":[%llu,%llu,%llu],\"hidden\":%llu,\"refused\":%llu,\"retired\":%llu,\"network\":%s,\"event\":%llu,\"fxActive\":%s,\"fxId\":%u,\"fxStarted\":%llu,\"fxStopped\":%llu,\"phaseBefore\":%u,\"phaseAfter\":%u,\"startError\":%d,\"startOk\":%s,\"stage\":%u,\"error\":\"%s\",\"result\":\"%s\"}\n",p.until?"true":"false",p.pendingHide?"true":"false",p.target.handle,p.scene.handle,p.playing,p.wanted,mesmerizeVisualCreated,mesmerizeVisualStarted[0],mesmerizeVisualStarted[1],mesmerizeVisualStarted[2],mesmerizeVisualHidden,mesmerizeVisualRefused,mesmerizeVisualRetired,p.network?"true":"false",p.event,p.effect.expires?"true":"false",p.effect.id,mesmerizeVisualFxStarted,mesmerizeVisualFxStopped,mesmerizeVisualPhaseBefore,mesmerizeVisualPhaseAfter,mesmerizeVisualStartError,mesmerizeVisualStartOk?"true":"false",mesmerizeVisualSceneStage,mesmerizeVisualError,mesmerizeVisualResult);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
    writeMesmerizeEyes();
    writeMesmerizeLinks();
}

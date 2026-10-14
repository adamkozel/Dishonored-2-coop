from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src'
p=S/'swarm_rat_visual_scene.h';assert not p.exists()
s=(S/'mesmerize_visual_scene.h').read_text(encoding='utf-8-sig').replace('MesmerizeVisual','SwarmRatVisual').replace('mesmerizeVisual','swarmRatVisual').replace('mesmerize_visual_in','swarm_rat_idle').replace('mesmerize_visual_loop','swarm_rat_walk').replace('mesmerize_visual_out','swarm_rat_run').replace('"Mesmerize"','"Rat"')
p.write_text(s)
s=(S/'mesmerize_visual_fixture.h').read_text(encoding='utf-8-sig');start=s.index('bool mesmerizeVisualIdentity(){');end=s.index('bool stopMesmerizeEye',start)
identity=s[start:end].replace('mesmerizeVisual','swarmRatVisual').replace('kMesmerizeVisualDefinition','kSwarmRatVisualDefinition')
p=S/'swarm_rat_visual_fixture.h';assert not p.exists()
p.write_text('''#pragma once
#include "swarm_rat_visual_scene.h"
// One cached cosmetic rat proves model/animation compatibility before a pool.
// No source handle is resolved locally. No gameplay/AI/possession components.
struct SwarmRatVisualFixture {
    SwarmRatVisualTarget target{};SwarmRatVisualScene scene{};
    std::uintptr_t game=0,definition=0;std::uint64_t epoch=0,event=0,streamToken=0,peerToken=0;
    ULONGLONG until=0,readyAfter=0;unsigned sourceHandle=0,incarnation=0,playing=0,wanted=0;
    bool pendingHide=false;std::array<float,12> sourceTransform{};
} swarmRatVisual;
std::uint64_t swarmRatCreated=0,swarmRatStarted=0,swarmRatHidden=0,swarmRatUpdated=0,swarmRatRefused=0,swarmRatRetired=0;
const char* swarmRatResult="idle";
constexpr const char* kSwarmRatVisualDefinition="models/powers/d2coop/swarm_rat_visual.def";
'''+identity+'''
void serviceSwarmRatVisual(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    auto& p=swarmRatVisual;const auto& s=swarmWorldStream;
    const auto refuse=[&](const char* why){++swarmRatRefused;swarmRatResult=why;p.pendingHide=true;};
    if(p.target.actor&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){p={};++swarmRatRetired;swarmRatResult="world_retired";}
    const bool automatic=localRole==1&&swarmWorldCurrent(tick)&&s.lease>tick&&s.presented.present&&s.presented.count&&movementBankActive()&&coopGameplayReady(remote)&&
        observerIsolation.ready&&observerIsolation.movementFixture&&observerIsolation.until>tick+2000&&!std::strcmp(mapName,kGuardMap)&&partnerAssetsEnabled;
    const CoopSwarmAgent* source=nullptr;
    if(automatic){
        if(p.until&&p.event==s.presented.event&&p.streamToken==s.token&&p.peerToken==s.peerToken){
            for(unsigned i=0;i<s.presented.count;++i)if(s.presented.agents[i].handle==p.sourceHandle&&s.presented.agents[i].incarnation==p.incarnation)source=&s.presented.agents[i];
        }else if(!p.until&&!p.pendingHide)source=&s.presented.agents[0];
    }
    if(p.until&&(!source||tick>=p.until)){p.pendingHide=true;p.wanted=0;}
    if(!coopGameplayReady(outgoing)||!powerFixtureApiMatches()||!rangedPropApiMatches()||!carryVictimApis())return;
    if(source&&!p.pendingHide){
        if(!p.target.actor){
            const auto definition=lookupEntityDefinition(kSwarmRatVisualDefinition);if(!definition){refuse("visual_definition_missing");return;}
            unsigned char parameters[12]{};reinterpret_cast<void*(*)(void*)>(gameImage+0x146520)(parameters);
            void* actor=reinterpret_cast<void*(*)(void*,void*,void*)>(gameImage+0x8a78f0)(reinterpret_cast<void*>(observedGame),reinterpret_cast<void*>(definition),parameters);
            if(!actor){refuse("native_creation_refused");return;}
            p.game=observedGame;p.epoch=outgoing.worldEpoch;p.definition=definition;p.target.actor=reinterpret_cast<std::uintptr_t>(actor);++swarmRatCreated;
            reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(actor,source->transform.data());
            reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(actor,source->transform.data()+3);
            reinterpret_cast<void(*)(void*,void*,int)>(gameImage+0x8aecc0)(reinterpret_cast<void*>(observedGame),actor,-1);
            memoryRead(p.target.actor+0xa8,&p.target.handle,4);reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(actor);p.readyAfter=tick+200;
        }
        if(!swarmRatVisualIdentity()){refuse("visual_identity_refused");return;}
        p.event=s.presented.event;p.streamToken=s.token;p.peerToken=s.peerToken;p.sourceHandle=source->handle;p.incarnation=source->incarnation;
        p.until=tick+300;p.wanted=2;p.sourceTransform=source->transform;
    }
    if(!p.target.actor||(!p.until&&!p.pendingHide))return;
    if(!swarmRatVisualIdentity()){refuse("owned_model_identity_changed");return;}
    if(p.scene.handle&&(p.pendingHide||p.playing!=p.wanted)){if(!stopSwarmRatVisualScene(p.scene,p.target))return;p.playing=0;}
    if(p.pendingHide){
        reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(p.target.actor));unsigned char flags=0;
        if(memoryRead(p.target.actor+0xd0,&flags,1)&&(flags&1)){p.pendingHide=false;p.until=0;p.playing=0;p.sourceHandle=0;++swarmRatHidden;swarmRatResult="native_visual_hidden";}return;
    }
    if(p.wanted&&p.playing!=p.wanted){
        if(tick<p.readyAfter)return;
        reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(p.target.actor));reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(p.target.actor));
        p.playing=p.wanted;
        if(!startSwarmRatVisualScene(p.scene,p.target,p.wanted)){refuse("native_scene_unconfirmed");return;}
        ++swarmRatStarted;swarmRatResult="native_rat_scene_started";
    }
    if(p.playing){
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c320)(reinterpret_cast<void*>(p.target.actor),p.sourceTransform.data());
        reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x87c2c0)(reinterpret_cast<void*>(p.target.actor),p.sourceTransform.data()+3);++swarmRatUpdated;
    }
}
void writeSwarmRatVisual(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.swarm-rat-visual.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;const auto& p=swarmRatVisual;
    fprintf(f,"{\\"experiment\\":true,\\"maximumModels\\":1,\\"active\\":%s,\\"pendingHide\\":%s,\\"handle\\":%u,\\"sourceHandle\\":%u,\\"incarnation\\":%u,\\"scene\\":%u,\\"playing\\":%u,\\"created\\":%llu,\\"started\\":%llu,\\"hidden\\":%llu,\\"updated\\":%llu,\\"refused\\":%llu,\\"retired\\":%llu,\\"event\\":%llu,\\"stage\\":%u,\\"result\\":\\"%s\\",\\"sourceTransform\\":[",p.until?"true":"false",p.pendingHide?"true":"false",p.target.handle,p.sourceHandle,p.incarnation,p.scene.handle,p.playing,swarmRatCreated,swarmRatStarted,swarmRatHidden,swarmRatUpdated,swarmRatRefused,swarmRatRetired,p.event,swarmRatVisualSceneStage,swarmRatResult);
    for(unsigned i=0;i<12;++i)fprintf(f,"%s%.6f",i?",":"",p.sourceTransform[i]);
    fputs("]}\\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}
''')
p=S/'engine_bridge.cpp';s=p.read_text(encoding='utf-8-sig');s=s.replace('#include "mesmerize_visual_fixture.h"','#include "mesmerize_visual_fixture.h"\n#include "swarm_rat_visual_fixture.h"')
s=s.replace('presentSwarmWorld(tick);','presentSwarmWorld(tick);serviceSwarmRatVisual(tick);').replace('writeSwarmWorld();','writeSwarmWorld();writeSwarmRatVisual();');p.write_text(s)
print('Prepared single-model native Rat/Midnight feasibility candidate')

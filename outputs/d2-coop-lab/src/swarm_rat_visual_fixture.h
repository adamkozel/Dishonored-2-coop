#pragma once
#include "swarm_rat_visual_scene.h"
// Bounded cosmetic rat pool, one cached model per validated source pool index.
// No source handle is resolved locally. No gameplay/AI/possession components.
struct SwarmRatVisualFixture {
    SwarmRatVisualTarget target{};SwarmRatVisualScene scene{};
    std::uintptr_t game=0,definition=0;std::uint64_t epoch=0,event=0,streamToken=0,peerToken=0;
    ULONGLONG until=0,readyAfter=0;unsigned sourceHandle=0,incarnation=0,playing=0,wanted=0;
    bool pendingHide=false;std::array<float,12> sourceTransform{},actualTransform{};
    float positionError=0,maxPositionError=0,interpolationAlpha=0;std::uint64_t verifiedTransforms=0,interpolatedTransforms=0;
    std::uintptr_t animation=0,graph=0,inputData=0,rateInput=0,movingInput=0;
    double gaitTick=0;std::array<float,3> gaitPosition{};
    float gaitSpeed=0,gaitRate=0,gaitMoving=0;std::uint64_t gaitUpdates=0;
    float sourceScale=1,pendingScale=0;std::array<float,3> frameScale{},entityScale{},renderScale{};
    ULONGLONG scaleDeadline=0;std::uint64_t scaleVerified=0;bool scaleReady=false;
    unsigned scaleStage=0;std::array<std::uintptr_t,8> scaleDiagnostic{};
};
std::array<SwarmRatVisualFixture,100> swarmRatVisualPool{};
double swarmRatServiceMaxMs=0;
std::uint64_t swarmRatCreated=0,swarmRatStarted=0,swarmRatHidden=0,swarmRatUpdated=0,swarmRatRefused=0,swarmRatRetired=0;
const char* swarmRatResult="idle";
constexpr const char* kSwarmRatVisualDefinition="models/powers/d2coop/swarm_rat_visual.def";
struct SwarmRatTiming {std::uint64_t count=0;double total=0,maximum=0;void add(double ms){if(ms>=0&&std::isfinite(ms)){++count;total+=ms;maximum=(std::max)(maximum,ms);}}};
std::array<std::array<SwarmRatTiming,5>,2> swarmRatTimings{};
std::array<SwarmRatTiming,2> swarmRatServiceIntervals{};
std::uintptr_t swarmRatTimingGame=0;std::uint64_t swarmRatTimingEpoch=0;
unsigned swarmRatTimingPhase=0,swarmRatPreviousTimingPhase=0;double swarmRatPreviousService=0;
struct SwarmRatTimingScope {unsigned stage,phase;double start;explicit SwarmRatTimingScope(unsigned s):stage(s),phase(swarmRatTimingPhase),start(presentationClock()){}~SwarmRatTimingScope(){swarmRatTimings[phase][stage].add(presentationClock()-start);}};
bool swarmRatVisualIdentity(const SwarmRatVisualFixture& p){
    SwarmRatTimingScope measure(0);
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vt=0,d=0,n=0,list=0;unsigned count=0,parent=0;std::string text;
    if(!p.target.actor||p.game!=observedGame||p.epoch!=outgoing.worldEpoch||
       worldIdentityEntity(memoryRead,p.game,p.target.handle)!=p.target.actor||
       !memoryRead(p.target.actor,&vt,8)||vt!=base+0x1c87d58||
       !memoryRead(p.target.actor+0xac,&parent,4)||parent!=0x3fff0010||
       !memoryRead(p.target.actor+0x2e8,&d,8)||d!=p.definition||
       !memoryRead(d,&vt,8)||vt!=base+0x1ab58f8||!memoryRead(d+8,&n,8)||
       !worldIdentityString(memoryRead,n,text)||text!=kSwarmRatVisualDefinition||
       !memoryRead(p.target.actor+0x268,&list,8)||!list||!memoryRead(p.target.actor+0x270,&count,4)||(count&0xffffff)!=4)return false;
    constexpr const char* allowed[]={"components/d2coop/swarm_rat_visual.cpntanimevolved",
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

bool swarmRatSetLiveTransform(SwarmRatVisualFixture& p){
    SwarmRatTimingScope measure(3);
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const unsigned char code[]={0x48,0x8b,0xc4,0x44,0x88,0x48,0x20,0x48,0x89,0x50,0x10,0x55,0x56,0x57,0x48,0x8d};
    std::uintptr_t frame=0,vt=0,owner=0,set=0;
    if(std::memcmp(gameImage+0x87d200,code,sizeof(code))||!swarmRatVisualIdentity(p)||
       !memoryRead(p.target.actor+0x358,&frame,8)||!frame||!memoryRead(frame,&vt,8)||vt!=base+0x1c573e8||
       !memoryRead(frame+0x10,&owner,8)||owner!=p.target.actor||!memoryRead(vt+22*8,&set,8)||set!=base+0x751d00)return false;
    // 87c320/87c2c0 are spawn-local setters. The live dispatcher notifies owned
    // components and updates entity/render transforms; no direct pose writes.
    reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(
        reinterpret_cast<void*>(p.target.actor),p.sourceTransform.data(),p.sourceTransform.data()+3,true,0);
    if(!memoryRead(p.target.actor+0x300,p.actualTransform.data(),sizeof(p.actualTransform)))return false;
    float d2=0;for(unsigned i=0;i<12;++i){
        if(!std::isfinite(p.actualTransform[i]))return false;
        if(i<3){const auto d=p.actualTransform[i]-p.sourceTransform[i];d2+=d*d;}
    }
    p.positionError=std::sqrt(d2);p.maxPositionError=(std::max)(p.maxPositionError,p.positionError);
    if(p.positionError>.001f)return false;
    ++p.verifiedTransforms;return true;
}

bool swarmRatSetNativeScale(SwarmRatVisualFixture& p,ULONGLONG tick){
    SwarmRatTimingScope measure(2);
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const unsigned char code[]={0x40,0x53,0x48,0x83,0xec,0x20,0x48,0x8b,0xd9,0x48,0x8b,0x89,0xf0,0x00,0x00,0x00};
    std::uintptr_t frame=0,owner=0,vt=0,driver=0,back=0,parent=0,render=0,animRender=0;
    const auto fail=[&](){p.scaleDiagnostic={frame,owner,vt,driver,back,parent,render,animRender};return false;};
    p.scaleStage=1;
    if(!std::isfinite(p.sourceScale)||p.sourceScale<.1f||p.sourceScale>4.f||std::memcmp(gameImage+0x87c5d0,code,sizeof(code)))return fail();
    p.scaleStage=2;if(!memoryRead(p.target.actor+0x358,&frame,8)||!frame||!memoryRead(frame,&vt,8)||vt!=base+0x1c573e8)return fail();
    p.scaleStage=3;if(!memoryRead(frame+0x10,&owner,8)||owner!=p.target.actor||!memoryRead(frame+0xd8,&driver,8)||driver!=frame+0x20)return fail();
    p.scaleStage=4;if(!memoryRead(driver,&vt,8)||vt!=base+0x1c574c0||!memoryRead(driver+8,&back,8)||(back&&back!=frame))return fail();
    p.scaleStage=5;if(!memoryRead(frame+0x3c8,&parent,8)||parent)return fail();
    p.scaleStage=6;if(!p.animation||!memoryRead(p.animation+0x38,&animRender,8)||!memoryRead(p.target.actor+0xf0,&render,8)||!render||render!=animRender)return fail();
    p.scaleDiagnostic={frame,owner,vt,driver,back,parent,render,animRender};
    p.scaleStage=8;
    if(p.pendingScale!=p.sourceScale){
        const float desired[]={p.sourceScale,p.sourceScale,p.sourceScale};
        //307: native753c10 copies entity scale back into the self-driven frame.
        //753630 only persists for an external driver (native rats use RatAgent).
        //Write the exact owned cosmetic entity's authoritative scale, then let
        //the existing native render flush and frame update consume it. No AI
        //driver, source actor, player, parent or gameplay rat is modified.
        float before[3]{};
        if(!memoryRead(p.target.actor+0x330,before,12))return false;
        for(float v:before)if(!std::isfinite(v)||v<.1f||v>4.f)return false;
        std::memcpy(reinterpret_cast<void*>(p.target.actor+0x330),desired,12);
        reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(p.target.actor));
        p.pendingScale=p.sourceScale;p.scaleDeadline=tick+500;p.scaleReady=false;
    }
    p.scaleStage=9;
    if(!memoryRead(frame+0x60,p.frameScale.data(),12)||!memoryRead(p.target.actor+0x330,p.entityScale.data(),12)||
       !memoryRead(render+0x1890,p.renderScale.data(),12))return false;
    p.scaleReady=true;
    for(unsigned i=0;i<3;++i){
        if(!std::isfinite(p.frameScale[i])||!std::isfinite(p.entityScale[i])||!std::isfinite(p.renderScale[i])){p.scaleReady=false;return false;}
        if(std::fabs(p.frameScale[i]-p.sourceScale)>.0001f||std::fabs(p.entityScale[i]-p.sourceScale)>.0001f||std::fabs(p.renderScale[i]-p.sourceScale)>.0001f)p.scaleReady=false;
    }
    if(p.scaleReady){++p.scaleVerified;p.scaleStage=10;}
    return p.scaleReady||tick<=p.scaleDeadline;
}

bool swarmRatDriveGait(SwarmRatVisualFixture& p){
    SwarmRatTimingScope measure(1);
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t vt=0,owner=0,tree=0,data=0,graph=0;
    if(!p.animation){
        std::uintptr_t list=0,animation=0,inputData=0;if(!memoryRead(p.target.actor+0x268,&list,8)||!list)return false;
        for(unsigned i=0;i<4;++i){std::uintptr_t c=0;if(!memoryRead(list+i*8,&c,8)||!c||!memoryRead(c,&vt,8))return false;
            if(vt==base+0x1b80408)animation=c;}
        std::uintptr_t name=0;std::string text;
        if(!animation||!memoryRead(animation+0x340,&graph,8)||!graph||!memoryRead(graph+8,&name,8)||
           !worldIdentityString(memoryRead,name,text)||text!="animtree/d2coop_swarm_rat_visual.animtree"||
           !memoryRead(animation+0x338,&tree,8)||!tree||!memoryRead(tree,&inputData,8)||!inputData)return false;
        const auto rateInput=shadowNamedInput(animation,"ratRate",1),movingInput=shadowNamedInput(animation,"ratMoving",1);
        if(!rateInput||!movingInput||rateInput==movingInput)return false;
        // Commit a binding only once the complete named input layout is available.
        p.animation=animation;p.graph=graph;p.inputData=inputData;p.rateInput=rateInput;p.movingInput=movingInput;
    }
    if(!p.rateInput||!p.movingInput||p.rateInput==p.movingInput||!memoryRead(p.animation,&vt,8)||vt!=base+0x1b80408||
       !memoryRead(p.animation+0x10,&owner,8)||owner!=p.target.actor||!memoryRead(p.animation+0x340,&graph,8)||graph!=p.graph||
       !memoryRead(p.animation+0x338,&tree,8)||!tree||!memoryRead(tree,&data,8)||data!=p.inputData)return false;
    const auto now=presentationClock();const double dt=now-p.gaitTick;float speed=0;
    if(p.gaitTick&&dt>0&&dt<=250){const float x=p.sourceTransform[0]-p.gaitPosition[0],y=p.sourceTransform[1]-p.gaitPosition[1];
        const float distance=std::sqrt(x*x+y*y);if(distance<=3.f)speed=distance*static_cast<float>(1000./dt);}
    if(!std::isfinite(speed)||speed<0)return false;
    p.gaitSpeed=speed;p.gaitRate=(std::min)(3.f,speed/(1.11f*(p.scaleReady?p.sourceScale:1.f))); // Use applied model scale only.
    const float target=speed>.03f?1.f:0.f;
    const float step=static_cast<float>((std::min)(125.,(std::max)(0.,dt))*.008);
    p.gaitMoving+=(std::max)(-step,(std::min)(step,target-p.gaitMoving));
    float previousRate=0,previousMoving=0;
    if(!memoryRead(p.rateInput,&previousRate,4)||!memoryRead(p.movingInput,&previousMoving,4)||
       !std::isfinite(previousRate)||previousRate<0||previousRate>3||!std::isfinite(previousMoving)||previousMoving<0||previousMoving>1)return false;
    // Only the named float inputs in this exact owned animation tree are changed.
    *reinterpret_cast<float*>(p.rateInput)=p.gaitRate;*reinterpret_cast<float*>(p.movingInput)=p.gaitMoving;
    float actualRate=0,actualMoving=0;
    if(!memoryRead(p.rateInput,&actualRate,4)||!memoryRead(p.movingInput,&actualMoving,4)||actualRate!=p.gaitRate||actualMoving!=p.gaitMoving)return false;
    for(unsigned i=0;i<3;++i)p.gaitPosition[i]=p.sourceTransform[i];p.gaitTick=now;++p.gaitUpdates;return true;
}

void serviceSwarmRatSlot(ULONGLONG tick,unsigned index,bool& mayCreate){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    auto& p=swarmRatVisualPool[index];const auto& s=swarmWorldStream;
    const auto refuse=[&](const char* why){++swarmRatRefused;swarmRatResult=why;p.pendingHide=true;};
    if(p.target.actor&&(p.game!=observedGame||p.epoch!=outgoing.worldEpoch)){p={};++swarmRatRetired;swarmRatResult="world_retired";}
    const bool automatic=remoteIsCorvo()&&swarmWorldCurrent(tick)&&s.lease>tick&&s.presented.present&&s.presented.count&&movementBankActive()&&coopGameplayReady(remote)&&
        // 1214ej: product sessions too (1500a: 0 rat models while the stream presented 15 rats).
        ((observerIsolation.ready&&observerIsolation.movementFixture&&observerIsolation.until>tick+2000&&!std::strcmp(mapName,kGuardMap))||productSessionArmed())&&partnerAssetsEnabled;
    const CoopSwarmAgent* source=nullptr;
    if(automatic){
        if(p.until&&p.event==s.presented.event&&p.streamToken==s.token&&p.peerToken==s.peerToken){
            for(unsigned i=0;i<s.presented.count;++i)if(s.presented.agents[i].index==index&&s.presented.agents[i].handle==p.sourceHandle&&s.presented.agents[i].incarnation==p.incarnation)source=&s.presented.agents[i];
        }else if(!p.until&&!p.pendingHide){for(unsigned i=0;i<s.presented.count;++i)if(s.presented.agents[i].index==index)source=&s.presented.agents[i];}
    }
    if(p.until&&(!source||tick>=p.until)){p.pendingHide=true;p.wanted=0;}
    if(!coopGameplayReady(outgoing)||!powerFixtureApiMatches()||!rangedPropApiMatches()||!carryVictimApis())return;
    if(source&&!p.pendingHide){
        if(!p.target.actor){
            if(!mayCreate)return;mayCreate=false; // Bound allocation cost to one new model per engine frame.
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
        if(!swarmRatVisualIdentity(p)){refuse("visual_identity_refused");return;}
        p.event=s.presented.event;p.streamToken=s.token;p.peerToken=s.peerToken;p.sourceHandle=source->handle;p.incarnation=source->incarnation;
        CoopSwarmAgent pose{};float alpha=0;
        if(!swarmRatPresentation(s.presented,s.presentedTime,s.count?&s.pending[0].packet:nullptr,
            s.count?s.pending[0].time:0,presentationClock()-kSwarmPresentationDelayMs,index,pose,alpha)){refuse("rat_presentation_missing");return;}
        p.until=tick+300;p.wanted=2;p.sourceTransform=pose.transform;p.interpolationAlpha=alpha;p.sourceScale=pose.scale;
    }
    if(!p.target.actor||(!p.until&&!p.pendingHide))return;
    if(!swarmRatVisualIdentity(p)){refuse("owned_model_identity_changed");return;}
    if(p.scene.handle&&(p.pendingHide||p.playing!=p.wanted)){if(!stopSwarmRatVisualScene(p.scene,p.target))return;p.playing=0;}
    if(p.pendingHide){
        reinterpret_cast<void(*)(void*)>(gameImage+0x8784b0)(reinterpret_cast<void*>(p.target.actor));unsigned char flags=0;
        if(memoryRead(p.target.actor+0xd0,&flags,1)&&(flags&1)){p.pendingHide=false;p.until=0;p.playing=0;p.sourceHandle=0;++swarmRatHidden;swarmRatResult="native_visual_hidden";}return;
    }
    if(p.wanted&&p.playing!=p.wanted){
        if(tick<p.readyAfter)return;
        reinterpret_cast<void(*)(void*)>(gameImage+0x87c590)(reinterpret_cast<void*>(p.target.actor));reinterpret_cast<void(*)(void*)>(gameImage+0x87c5d0)(reinterpret_cast<void*>(p.target.actor));
        p.playing=p.wanted;
        // Candidate296: the dedicated native animleaf owns the walking clip.
        // No Midnight events or source rat gameplay execute on this cosmetic body.
        ++swarmRatStarted;swarmRatResult="native_rat_leaf_active";
    }
    if(p.playing){
        if(!swarmRatDriveGait(p)){refuse("native_gait_input_refused");return;}
        if(!swarmRatSetNativeScale(p,tick)){refuse("native_scale_refused");return;}
        if(!swarmRatSetLiveTransform(p)){refuse("live_transform_refused");return;}
        if(p.interpolationAlpha>0&&p.interpolationAlpha<1)++p.interpolatedTransforms;
        ++swarmRatUpdated;
    }
}

void serviceSwarmRatVisual(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto began=presentationClock();bool mayCreate=true;
    if(swarmRatTimingGame!=observedGame||swarmRatTimingEpoch!=outgoing.worldEpoch){
        swarmRatTimings={};swarmRatServiceIntervals={};swarmRatPreviousService=0;
        swarmRatTimingGame=observedGame;swarmRatTimingEpoch=outgoing.worldEpoch;
    }
    swarmRatTimingPhase=remoteIsCorvo()&&swarmWorldCurrent(tick)&&swarmWorldStream.lease>tick&&swarmWorldStream.presented.present&&swarmWorldStream.presented.count?1:0;
    if(swarmRatPreviousService&&swarmRatPreviousTimingPhase==swarmRatTimingPhase&&began-swarmRatPreviousService<250.)swarmRatServiceIntervals[swarmRatTimingPhase].add(began-swarmRatPreviousService);
    swarmRatPreviousService=began;swarmRatPreviousTimingPhase=swarmRatTimingPhase;
    SwarmRatTimingScope measure(4);
    for(unsigned i=0;i<swarmRatVisualPool.size();++i)serviceSwarmRatSlot(tick,i,mayCreate);
    swarmRatServiceMaxMs=(std::max)(swarmRatServiceMaxMs,presentationClock()-began);
}
void writeSwarmRatVisual(){
    char path[MAX_PATH]{},tmp[MAX_PATH]{};std::snprintf(path,sizeof(path),"%s.swarm-rat-visual.json",statusPath);std::snprintf(tmp,sizeof(tmp),"%s.tmp",path);
    FILE* f=nullptr;if(fopen_s(&f,tmp,"wb")||!f)return;
    unsigned active=0,cached=0,hiding=0;float error=0;std::uint64_t verified=0,interpolated=0;
    for(const auto& p:swarmRatVisualPool){if(p.target.actor)++cached;if(p.until&&!p.pendingHide&&p.playing)++active;if(p.pendingHide)++hiding;error=(std::max)(error,p.maxPositionError);verified+=p.verifiedTransforms;interpolated+=p.interpolatedTransforms;}
    // Keep first-slot diagnostic fields for the established closeup/skeleton harness.
    const auto& p=swarmRatVisualPool[0];
    fprintf(f,"{\"experiment\":true,\"maximumModels\":100,\"active\":%s,\"activeModels\":%u,\"cachedModels\":%u,\"pendingHide\":%s,\"hidingModels\":%u,\"serviceMaxMs\":%.4f,\"handle\":%u,\"sourceHandle\":%u,\"incarnation\":%u,\"scene\":0,\"playing\":%u,\"created\":%llu,\"started\":%llu,\"hidden\":%llu,\"updated\":%llu,\"refused\":%llu,\"retired\":%llu,\"event\":%llu,\"result\":\"%s\",\"sourceTransform\":[",active?"true":"false",active,cached,hiding?"true":"false",hiding,swarmRatServiceMaxMs,p.target.handle,p.sourceHandle,p.incarnation,p.playing,swarmRatCreated,swarmRatStarted,swarmRatHidden,swarmRatUpdated,swarmRatRefused,swarmRatRetired,p.event,swarmRatResult);
    for(unsigned i=0;i<12;++i)fprintf(f,"%s%.6f",i?",":"",p.sourceTransform[i]);
    fprintf(f,"],\"verifiedTransforms\":%llu,\"positionError\":%.7f,\"maxPositionError\":%.7f,\"actualTransform\":[",verified,p.positionError,error);
    for(unsigned i=0;i<12;++i)fprintf(f,"%s%.6f",i?",":"",p.actualTransform[i]);
    fprintf(f,"],\"interpolatedTransforms\":%llu,\"models\":[",interpolated);bool comma=false;
    for(unsigned i=0;i<swarmRatVisualPool.size();++i){const auto& a=swarmRatVisualPool[i];if(!a.target.actor)continue;
        fprintf(f,"%s{\"index\":%u,\"handle\":%u,\"sourceHandle\":%u,\"incarnation\":%u,\"active\":%s,\"pendingHide\":%s,\"verifiedTransforms\":%llu,\"maxPositionError\":%.7f,\"actualTransform\":[",comma?",":"",i,a.target.handle,a.sourceHandle,a.incarnation,a.until&&!a.pendingHide&&a.playing?"true":"false",a.pendingHide?"true":"false",a.verifiedTransforms,a.maxPositionError);comma=true;
        for(unsigned j=0;j<12;++j)fprintf(f,"%s%.6f",j?",":"",a.actualTransform[j]);fprintf(f,"],\"gaitSpeed\":%.6f,\"gaitRate\":%.6f,\"gaitMoving\":%.6f,\"gaitUpdates\":%llu,\"sourceScale\":%.6f,\"scaleReady\":%s,\"scaleVerified\":%llu,\"frameScale\":[%.6f,%.6f,%.6f],\"entityScale\":[%.6f,%.6f,%.6f],\"renderScale\":[%.6f,%.6f,%.6f],\"scaleStage\":%u,\"scaleDiagnostic\":[%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu]}",a.gaitSpeed,a.gaitRate,a.gaitMoving,a.gaitUpdates,a.sourceScale,a.scaleReady?"true":"false",a.scaleVerified,a.frameScale[0],a.frameScale[1],a.frameScale[2],a.entityScale[0],a.entityScale[1],a.entityScale[2],a.renderScale[0],a.renderScale[1],a.renderScale[2],a.scaleStage,a.scaleDiagnostic[0],a.scaleDiagnostic[1],a.scaleDiagnostic[2],a.scaleDiagnostic[3],a.scaleDiagnostic[4],a.scaleDiagnostic[5],a.scaleDiagnostic[6],a.scaleDiagnostic[7]);
    }
    fputs("],\"timings\":{\"scope\":\"service cadence is capped by the10ms transport pump; intervals are not display FPS; transform includes identity and total includes all stages\",\"phases\":[",f);
    constexpr const char* names[]={"identity","gait","scale","transform","total"};
    for(unsigned phase=0;phase<2;++phase){const auto& interval=swarmRatServiceIntervals[phase];
        fprintf(f,"%s{\"phase\":\"%s\",\"intervalCount\":%llu,\"intervalTotalMs\":%.6f,\"intervalMaxMs\":%.6f,\"stages\":[",phase?",":"",phase?"source_present":"source_absent",interval.count,interval.total,interval.maximum);
        for(unsigned stage=0;stage<5;++stage){const auto& v=swarmRatTimings[phase][stage];fprintf(f,"%s{\"stage\":\"%s\",\"count\":%llu,\"totalMs\":%.6f,\"maxMs\":%.6f}",stage?",":"",names[stage],v.count,v.total,v.maximum);}
        fputs("]}",f);
    }
    fputs("]}}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(tmp,path,MOVEFILE_REPLACE_EXISTING);
}

// Experimental, opt-in adapter for the independent owned avatar only.
// Applied immediately before native 0x540f20 evaluates its animation tree.
using EvaluateAnimation=bool(*)(void*,void*,std::uint32_t);
EvaluateAnimation realEvaluateAnimation=nullptr;
SRWLOCK locomotionLock=SRWLOCK_INIT;
struct LocomotionPose {
    std::uintptr_t actor=0,anim=0;std::uint32_t handle=0;
    int stance=-1,equipment=-1;
    ULONGLONG expires=0;float velocity[3]{},position[3]{},orientation[9]{};
};
LocomotionPose locomotionPose{};
bool locomotionEnabled=false;
ULONGLONG shadowLocoUntil=0;
bool shadowLocoCombat=false;
std::uintptr_t shadowLocoGame=0,shadowLocoActor=0;
std::uint32_t shadowLocoHandle=0;
std::uint64_t shadowLocoEpoch=0,shadowLocoRemoteEpoch=0,shadowLocoRemoteSession=0;
int shadowLocoName(std::uintptr_t anim,bool equipment,const char* expected){
    std::uintptr_t render=0,model=0,loco=0,names=0;unsigned flags=0;
    if(!memoryRead(anim+0x38,&render,8)||!render||!memoryRead(render+0x1fe8,&model,8)||!model||
       !memoryRead(model+0x140,&loco,8)||!loco||!memoryRead(loco+(equipment?0x88:0x78),&names,8)||!names||
       !memoryRead(loco+(equipment?0x90:0x80),&flags,4)||!(flags&0xffffff)||(flags&0xffffff)>127)return -1;
    for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t name=0;char value[32]{};
        if(!memoryRead(names+i*8,&name,8)||!name||!memoryRead(name,value,sizeof(value)-1))return -1;
        if(!std::strcmp(value,expected))return static_cast<int>(i);
    }return -1;
}

std::uint32_t locomotionLastSequence=0;
ULONGLONG locomotionLastTick=0;
float locomotionLastPosition[3]{},locomotionVelocity[3]{};
std::atomic<std::uint64_t> locomotionApplied{0},locomotionRefused{0};
std::atomic<float> locomotionAppliedSpeed{0};

// Binding names/types/offsets are checked against the game's own declarations.
// Native 0x6e0b50 resolves this same descriptor offset into instance storage.
std::uintptr_t locomotionParameter(std::uintptr_t anim,unsigned field,const char* expected,unsigned type,unsigned bytes) {
    std::uintptr_t wrapper=0,tree=0,data=0,descriptor=0,name=0;std::uint32_t actualType=0;std::int32_t offset=-1;
    char actualName[64]{};
    if(!memoryRead(anim+0x2728+0x338,&wrapper,8)||wrapper!=anim+0x338||
       !memoryRead(wrapper,&tree,8)||!tree||!memoryRead(tree,&data,8)||!data||
       !memoryRead(anim+0x2728+field,&descriptor,8)||!descriptor||!memoryRead(descriptor-8,&name,8)||!name||
       !memoryRead(name,actualName,std::strlen(expected)+1)||std::strcmp(actualName,expected)||
       !memoryRead(descriptor,&actualType,4)||actualType!=type||!memoryRead(descriptor+4,&offset,4)||offset<0||offset>0x100000)return 0;
    MEMORY_BASIC_INFORMATION region{};const auto target=data+offset;
    if(!VirtualQuery(reinterpret_cast<void*>(target),&region,sizeof(region))||region.State!=MEM_COMMIT||
       (region.Protect&(PAGE_GUARD|PAGE_NOACCESS))||!(region.Protect&(PAGE_READWRITE|PAGE_WRITECOPY|PAGE_EXECUTE_READWRITE|PAGE_EXECUTE_WRITECOPY))||
       target+bytes>reinterpret_cast<std::uintptr_t>(region.BaseAddress)+region.RegionSize)return 0;
    return target;
}
bool evaluateAnimation(void* component,void* actor,std::uint32_t delta) {
    LocomotionPose state{};
    AcquireSRWLockShared(&locomotionLock);state=locomotionPose;ReleaseSRWLockShared(&locomotionLock);
    if(state.anim==reinterpret_cast<std::uintptr_t>(component)&&state.actor==reinterpret_cast<std::uintptr_t>(actor)&&
       networkFrameOwned.load(std::memory_order_acquire)&&GetTickCount64()<state.expires) {
        std::uintptr_t vt=0,owner=0;std::uint32_t handle=0;
        if(memoryRead(state.anim,&vt,8)&&vt==reinterpret_cast<std::uintptr_t>(gameImage)+0x1b80408&&
           memoryRead(state.anim+0x10,&owner,8)&&owner==state.actor&&memoryRead(state.actor+0xa8,&handle,4)&&handle==state.handle) {
            const auto modern=locomotionParameter(state.anim,0x4c0,"locomotionInputData",21,0x1e8);
            const auto velocity=locomotionParameter(state.anim,0x228,"targLinearVelocity",10,12);
            const auto forward=locomotionParameter(state.anim,0x220,"forwardOrientation",10,12);
            const auto desired=locomotionParameter(state.anim,0x2a0,"desiredOrientation",10,12);
            if(modern&&velocity&&forward&&desired) {
                const auto* v=state.velocity;
                // Only the evaluation sees our overrides. Restore the original
                // inputs afterward, including the request's facing flag, so
                // disabling/freshness loss leaves no lingering animation input.
                unsigned char savedPrefix[0x48]{},savedRequest[0xc9]{},savedFlag=0;
                float savedVelocity[3]{},savedForward[3]{},savedDesired[3]{};
                std::memcpy(savedPrefix,reinterpret_cast<void*>(modern),sizeof(savedPrefix));
                std::memcpy(savedRequest,reinterpret_cast<void*>(modern+0x118),sizeof(savedRequest));
                std::memcpy(&savedFlag,reinterpret_cast<void*>(modern+0x118+0xc8),1);
                std::memcpy(savedVelocity,reinterpret_cast<void*>(velocity),12);
                std::memcpy(savedForward,reinterpret_cast<void*>(forward),12);
                std::memcpy(savedDesired,reinterpret_cast<void*>(desired),12);
                // Recovered from native 0x54ff00 and 0x469040: position,
                // orientation, linear velocity and angular velocity prefix.
                // Preserve the native event/stance/pathfinding storage after it.
                std::memcpy(reinterpret_cast<void*>(modern+0x30),v,12);
                std::memcpy(reinterpret_cast<void*>(velocity),v,12);
                std::memcpy(reinterpret_cast<void*>(modern),state.position,12);
                std::memcpy(reinterpret_cast<void*>(modern+12),state.orientation,36);
                std::memcpy(reinterpret_cast<void*>(forward),state.orientation,12);
                std::memcpy(reinterpret_cast<void*>(desired),state.orientation,12);
                // Native request setters: vector velocity and facing direction.
                reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x486d40)(reinterpret_cast<void*>(modern+0x118),v);
                reinterpret_cast<void(*)(void*,const float*)>(gameImage+0x486bc0)(reinterpret_cast<void*>(modern+0x118),state.orientation);
                // Explicit clone ownership changes both graph axes together.
                // Native 0x484ab0 may retain a previous pending graph during a
                // scheduled switch. Immediate requests select the owned graph
                // without repeatedly replaying the NPC transition.
                if(state.stance>=0&&state.equipment>=0){
                    reinterpret_cast<void(*)(void*,int,bool)>(gameImage+0x486c50)(reinterpret_cast<void*>(modern+0x118),state.stance,true);
                    reinterpret_cast<void(*)(void*,int,bool)>(gameImage+0x486b00)(reinterpret_cast<void*>(modern+0x118),state.equipment,true);
                }
                // The inherited NPC can request a locomotion reset every update.
                // 0x486500 tests bit 4 and calls 0x486270 to reset its state.
                // Keep our owned gait continuous; restore the native flag below.
                *reinterpret_cast<unsigned char*>(modern+0x118+0xc8)&=~0x10;
                locomotionAppliedSpeed.store(std::sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]));
                locomotionApplied.fetch_add(1);
                const auto actionBlend=shadowActionParameter(state.anim);float savedBlend=0;
                const bool action=actionBlend&&memoryRead(actionBlend,&savedBlend,4)&&std::isfinite(savedBlend);
                if(action)*reinterpret_cast<float*>(actionBlend)=1.f;
                const bool result=realEvaluateAnimation(component,actor,delta);
                if(action)*reinterpret_cast<float*>(actionBlend)=savedBlend;
                std::memcpy(reinterpret_cast<void*>(modern),savedPrefix,sizeof(savedPrefix));
                std::memcpy(reinterpret_cast<void*>(modern+0x118),savedRequest,sizeof(savedRequest));
                std::memcpy(reinterpret_cast<void*>(modern+0x118+0xc8),&savedFlag,1);
                std::memcpy(reinterpret_cast<void*>(velocity),savedVelocity,12);
                std::memcpy(reinterpret_cast<void*>(forward),savedForward,12);
                std::memcpy(reinterpret_cast<void*>(desired),savedDesired,12);
                applyBodyProbe(component,actor);
                applyArmProbe(component,actor);
                applyGuardBody(component,actor);
                return result;
            }else locomotionRefused.fetch_add(1);
        }else locomotionRefused.fetch_add(1);
    }else if(state.anim==reinterpret_cast<std::uintptr_t>(component))locomotionAppliedSpeed.store(0);
    const bool result=realEvaluateAnimation(component,actor,delta);applyBodyProbe(component,actor);applyArmProbe(component,actor);applyGuardBody(component,actor);return result;
}
void publishLocomotion(ULONGLONG tick,const float* position,const float* orientation) {
    if(!locomotionEnabled)return;
    std::uintptr_t list=0,anim=0;std::uint32_t count=0;
    if(!memoryRead(probeActor+0x268,&list,8)||!list||!memoryRead(probeActor+0x270,&count,4)||(count&0xffffff)>256)return;
    for(unsigned i=0;i<(count&0xffffff);++i) {
        std::uintptr_t c=0,vt=0;
        if(memoryRead(list+i*8,&c,8)&&c&&memoryRead(c,&vt,8)&&vt==reinterpret_cast<std::uintptr_t>(gameImage)+0x1b80408)anim=c;
    }
    if(!anim)return;
    if(remote.sequence!=locomotionLastSequence) {
        const auto elapsed=tick-locomotionLastTick;float candidate[3]{},speedSquared=0;
        if(locomotionLastSequence&&elapsed>=10&&elapsed<=250) {
            for(int i=0;i<2;++i){candidate[i]=(remote.position[i]-locomotionLastPosition[i])*1000.f/static_cast<float>(elapsed);speedSquared+=candidate[i]*candidate[i];}
            // Teleports and excessive arrival gaps do not turn into gait speed.
            if(speedSquared>100.f)std::memset(candidate,0,sizeof(candidate));
        }
        const float blend=(std::min)(1.f,static_cast<float>(elapsed)/100.f);
        for(int i=0;i<3;++i)locomotionVelocity[i]+=(candidate[i]-locomotionVelocity[i])*blend;
        // Quantize sub-5mm/s smoothing residue to exact rest. Runtime evidence
        // shows this alone does not fix the native female Sword idle issue.
        if(locomotionVelocity[0]*locomotionVelocity[0]+locomotionVelocity[1]*locomotionVelocity[1]<0.000025f)
            std::memset(locomotionVelocity,0,sizeof(locomotionVelocity));
        locomotionLastSequence=remote.sequence;locomotionLastTick=tick;std::memcpy(locomotionLastPosition,remote.position,12);
    }
    LocomotionPose next{};next.actor=probeActor;next.anim=anim;next.handle=probeHandle;next.expires=lastReceive+250;
    if(shadowLocoUntil>tick&&shadowLocoGame==probeGame&&shadowLocoActor==probeActor&&shadowLocoHandle==probeHandle&&
       shadowLocoEpoch==outgoing.worldEpoch&&shadowLocoRemoteEpoch==remote.worldEpoch&&shadowLocoRemoteSession==remote.session){
        NativeSwordEquipment sword{};
        const auto scope=localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily;
        const unsigned char stanceBytes[]={0x80,0xa1,0xc8,0,0,0,0xfb};
        const unsigned char equipBytes[]={0x80,0xa1,0xc8,0,0,0,0xf7};
        if(!std::memcmp(gameImage+0x486c50,stanceBytes,sizeof(stanceBytes))&&!std::memcmp(gameImage+0x486b00,equipBytes,sizeof(equipBytes))&&
           readNativeSwordEquipment(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),probeHandle,sword,scope)&&sword.actor==probeActor&&sword.equipped&&sword.started){
            next.stance=shadowLocoName(anim,false,shadowLocoCombat?"Combat":"Relaxed");next.equipment=shadowLocoName(anim,true,"Sword");
        }
    }
    std::memcpy(next.position,position,12);std::memcpy(next.orientation,orientation,36);std::memcpy(next.velocity,locomotionVelocity,12);
    AcquireSRWLockExclusive(&locomotionLock);locomotionPose=next;ReleaseSRWLockExclusive(&locomotionLock);
}

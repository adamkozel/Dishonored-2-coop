#pragma once
// Exact B671 retail binary. Native397fe0 reads phase0 ankles and applies the
// native terrain result; 398de0 schedules native foot casts. Their flag4 branch
// uses the current pose instead of the NPC graph's anticipated planted step.
// Scope the branch to one owned authored clone and restore the bit on return.
// No world-root, damage, AI, shared declaration or player-physics writes.
using MovementFootSolve=void(*)(void*,float);
using MovementFootCast=void(*)(void*,float,unsigned,unsigned);
using MovementFootReset=void(*)(void*,const float*,const float*,bool);
using MovementFootTarget=void(*)(void*);
MovementFootSolve realMovementFootSolve=nullptr;
MovementFootCast realMovementFootCast=nullptr;
MovementFootReset realMovementFootReset=nullptr;
MovementFootTarget realMovementFootTarget=nullptr;
thread_local void* movementSolvingFoot=nullptr;
std::atomic<std::uint64_t> movementFeetCasts{0},movementFeetSolves{0};
std::atomic<std::uint64_t> movementFeetResetsAvoided{0};
std::atomic<std::uint64_t> movementFeetTargetsAdapted{0};
void movementFootTarget(void* foot){
    // In current-pose mode native397fe0 stores the floor correction at +244,
    // then398b20 turns that into the absolute IK target. The authored ankle
    // already includes boot height and the lift of the swinging foot. Add its
    // height only while the target is copied; retain native floor history for
    // the pelvis solver. No new casts or duplicate IK requests are created.
    auto* bytes=static_cast<unsigned char*>(foot);
    if(movementSolvingFoot!=foot){realMovementFootTarget(foot);return;}
    float height=0,ground=0;std::memcpy(&height,bytes+0x1ec,4);std::memcpy(&ground,bytes+0x24c,4);
    if(!std::isfinite(height)||!std::isfinite(ground)||height<0||height>.8f||std::fabs(ground)>.35f){realMovementFootTarget(foot);return;}
    const float corrected=height+ground;std::memcpy(bytes+0x24c,&corrected,4);
    realMovementFootTarget(foot);std::memcpy(bytes+0x24c,&ground,4);++movementFeetTargetsAdapted;
}
void movementFootReset(void* component,const float* position,const float* rotation,bool resetVelocity){
    // Entity_SetTransform notifies this callback even for tiny network root
    // updates. Native54e8c0 zeroes pelvis smoothing and reseeds both feet.
    // Preserve that history only for continuous, authored, owned movement.
    // Genuine relocations and every unrelated actor keep the native reset.
    bool continuous=false;AcquireSRWLockShared(&movementLock);const auto& p=movementPlayback;const auto tick=GetTickCount64();
    if(p.currentFeet&&p.nativeFeet&&p.end>tick&&p.lease>tick&&p.clock.weight>.99f&&movementFootGrounded(p)&&
       networkFrameOwned.load()&&p.animation==reinterpret_cast<std::uintptr_t>(component)&&p.game==observedGame){
        std::uintptr_t owner=0,vt=0,frame=0;float old[12]{},next[3]{},axis[9]{};
        const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
        if(memoryRead(p.animation,&vt,8)&&vt==base+0x1b80408&&memoryRead(p.animation+0x10,&owner,8)&&owner==p.actor&&
           memoryRead(owner+0x358,&frame,8)&&controlledFrame.load()==reinterpret_cast<void*>(frame)&&
           memoryRead(owner+0x300,old,sizeof(old))&&memoryRead(reinterpret_cast<std::uintptr_t>(position),next,12)&&
           memoryRead(reinterpret_cast<std::uintptr_t>(rotation),axis,36)){
            float distance=0,dot=0;bool finite=true;
            for(unsigned i=0;i<3;++i){distance+=(next[i]-old[i])*(next[i]-old[i]);dot+=axis[i]*old[3+i];finite=finite&&std::isfinite(next[i]);}
            for(float v:axis)finite=finite&&std::isfinite(v);
            continuous=finite&&distance<.0625f&&dot>.94f;
        }
    }
    ReleaseSRWLockShared(&movementLock);
    if(continuous){++movementFeetResetsAvoided;return;}realMovementFootReset(component,position,rotation,resetVelocity);
}
bool movementCurrentFootOwned(void* pointer){
    const auto& p=movementPlayback;const auto tick=GetTickCount64();
    if(!p.currentFeet||!p.nativeFeet||!p.end||tick>=p.end||tick>=p.lease||p.clock.weight<.99f||
       !movementFootGrounded(p)||!networkFrameOwned.load())return false;
    const auto foot=reinterpret_cast<std::uintptr_t>(pointer),base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t owner=0,vt=0,feet=0,actor=0;unsigned count=0;short ankle=0,ik=0;
    return memoryRead(foot,&vt,8)&&vt==base+0x1b3b1b0&&memoryRead(foot+0x78,&owner,8)&&owner==p.animation&&
        memoryRead(owner,&vt,8)&&vt==base+0x1b80408&&memoryRead(owner+0x10,&actor,8)&&actor==p.actor&&
        memoryRead(owner+0x2d08,&feet,8)&&feet&&memoryRead(owner+0x2d10,&count,4)&&count==2&&
        (foot==feet||foot==feet+0x280)&&memoryRead(foot+0x90,&ik,2)&&memoryRead(foot+0x92,&ankle,2)&&
        ik==ankle&&(ankle==21||ankle==25);
}
struct MovementFootScope {
    unsigned char* flags=nullptr;unsigned char previous=0;
    explicit MovementFootScope(void* foot){
        AcquireSRWLockShared(&movementLock);
        if(movementCurrentFootOwned(foot)){
            flags=static_cast<unsigned char*>(foot)+0x270;previous=*flags;
            *flags=static_cast<unsigned char>(previous|4);
        }
    }
    ~MovementFootScope(){if(flags)*flags=static_cast<unsigned char>((*flags&~4)|(previous&4));ReleaseSRWLockShared(&movementLock);}
};
void movementFootSolve(void* foot,float weight){MovementFootScope scope(foot);void* previous=movementSolvingFoot;if(scope.flags){++movementFeetSolves;movementSolvingFoot=foot;}realMovementFootSolve(foot,weight);movementSolvingFoot=previous;}
void movementFootCast(void* foot,float weight,unsigned group,unsigned mask){MovementFootScope scope(foot);if(scope.flags)++movementFeetCasts;realMovementFootCast(foot,weight,group,mask);}
bool installMovementFootHooks(){
    const unsigned char solve[]={0x48,0x8b,0xc4,0xf3,0x0f,0x11,0x48,0x10,0x55,0x53,0x56,0x57,0x41,0x57};
    const unsigned char cast[]={0x48,0x89,0x5c,0x24,0x10,0x48,0x89,0x6c,0x24,0x18,0x56,0x57,0x41,0x56};
    const unsigned char reset[]={0x80,0xa1,0xa8,0x2c,0,0,0xfd,0x41,0x80,0xe1,1,0x48,0x81,0xc1,8,0x2d,0,0};
    const unsigned char target[]={0x48,0x89,0x7c,0x24,0x10,0x55,0x48,0x8d,0x6c,0x24,0xa9,0x48,0x81,0xec,0xd0,0,0,0};
    if(std::memcmp(gameImage+0x397fe0,solve,14)||std::memcmp(gameImage+0x398de0,cast,14)||std::memcmp(gameImage+0x54bbc0,reset,18)||std::memcmp(gameImage+0x398b20,target,18))return false;
    auto hook=[](unsigned rva,const unsigned char* expected,unsigned length,void* wrapper,void** original){
        auto* entry=gameImage+rva;auto* trampoline=static_cast<unsigned char*>(VirtualAlloc(nullptr,64,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE));
        if(!trampoline)return false;std::memcpy(trampoline,expected,length);
        unsigned char jump[18]{0xff,0x25,0,0,0,0};for(unsigned i=14;i<18;++i)jump[i]=0x90;
        void* resume=entry+length;std::memcpy(jump+6,&resume,8);std::memcpy(trampoline+length,jump,14);
        DWORD old=0,unused=0;if(!VirtualProtect(trampoline,64,PAGE_EXECUTE_READ,&old))return false;FlushInstructionCache(GetCurrentProcess(),trampoline,64);
        *original=trampoline;std::memcpy(jump+6,&wrapper,8);
        if(!VirtualProtect(entry,length,PAGE_EXECUTE_READWRITE,&old))return false;
        std::memcpy(entry,jump,length);FlushInstructionCache(GetCurrentProcess(),entry,length);return VirtualProtect(entry,length,old,&unused)!=0;
    };
    return hook(0x397fe0,solve,14,reinterpret_cast<void*>(&movementFootSolve),reinterpret_cast<void**>(&realMovementFootSolve))&&
        hook(0x398de0,cast,14,reinterpret_cast<void*>(&movementFootCast),reinterpret_cast<void**>(&realMovementFootCast))&&
        hook(0x54bbc0,reset,18,reinterpret_cast<void*>(&movementFootReset),reinterpret_cast<void**>(&realMovementFootReset))&&
        hook(0x398b20,target,18,reinterpret_cast<void*>(&movementFootTarget),reinterpret_cast<void**>(&realMovementFootTarget));
}

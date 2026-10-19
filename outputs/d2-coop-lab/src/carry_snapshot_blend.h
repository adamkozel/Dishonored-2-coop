#pragma once
// Optional, bounded Mayhew experiment. Native AI recovery 596110 calls
// 54de30 to capture local joints into AnimEvolved+2c60 and binds that snapshot
// through 700ff0. 700fc0 controls its graph weight. No AI recovery state,
// health, gameplay events or first-person animation is invoked here.
struct CarrySnapshotBlend {
    std::uintptr_t actor=0,animation=0,buffer=0,input=0,weight=0;
    unsigned handle=0,bytes=0;ULONGLONG started=0,restoredAt=0;
    std::array<unsigned char,16> oldInput{};
    float oldWeight=0;bool bound=false,captured=false;
};
bool carrySnapshotApi(){
    constexpr unsigned char capture[]={0x48,0x89,0x5c,0x24,8,0x48,0x89,0x74,0x24,0x10,0x57,0x48,0x81,0xec,0xa0,0,0,0};
    constexpr unsigned char weight[]={0x40,0x53,0x48,0x83,0xec,0x20,0x48,0x8b,0x91,0xb0,4,0,0,0x48,0x85,0xd2};
    constexpr unsigned char binding[]={0x40,0x53,0x48,0x83,0xec,0x30,0x48,0x8b,0xc2,0x48,0x8b,0x91,0xa8,4,0,0};
    return gameImage&&!std::memcmp(gameImage+0x54de30,capture,sizeof(capture))&&
        !std::memcmp(gameImage+0x700fc0,weight,sizeof(weight))&&!std::memcmp(gameImage+0x700ff0,binding,sizeof(binding));
}
BodyMatrix carryRootMatrix(const float* root){
    auto m=bodyIdentity();for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)m.v[c*4+r]=root[3+c*3+r];
    for(unsigned r=0;r<3;++r)m.v[12+r]=root[r];return m;
}
bool carrySnapshotOwned(const CarrySnapshotBlend& s){
    std::uintptr_t p=0;unsigned bytes=0;
    return s.animation&&worldIdentityEntity(memoryRead,observedGame,s.handle)==s.actor&&
        memoryRead(s.animation,&p,8)&&p==reinterpret_cast<std::uintptr_t>(gameImage)+0x1b80408&&
        memoryRead(s.animation+0x10,&p,8)&&p==s.actor&&memoryRead(s.animation+0x2c60,&p,8)&&p==s.buffer&&
        memoryRead(s.animation+0x2c68,&bytes,4)&&bytes==s.bytes&&
        locomotionParameter(s.animation,0x4a8,"recoverFromRagdollSnapshotInput",13,16)==s.input&&
        locomotionParameter(s.animation,0x4b0,"recoverFromRagdollBlendInput",1,4)==s.weight;
}
bool stopCarrySnapshot(CarrySnapshotBlend& s){
    if(!s.bound)return true;
    if(!carrySnapshotOwned(s))return false;
    std::uintptr_t pointer=0;
    if(!memoryRead(s.input,&pointer,8)||pointer!=s.animation+0x2c60)return false;
    // Restore only our own binding, using the game's setters to invalidate
    // graph inputs. The reserved native snapshot allocation stays allocated.
    reinterpret_cast<bool(*)(void*,float)>(gameImage+0x700fc0)(reinterpret_cast<void*>(s.animation+0x2728),s.oldWeight);
    reinterpret_cast<bool(*)(void*,const void*)>(gameImage+0x700ff0)(reinterpret_cast<void*>(s.animation+0x2728),s.oldInput.data());
    *reinterpret_cast<unsigned char*>(s.animation+0x2c6c)=0; // Restore the initially unused snapshot marker.
    s.bound=false;s.restoredAt=GetTickCount64();return true;
}
// 1214ds: the step that refused the last snapshot start, 0 when it started (1277v: the host's product attempts were
// refused after the capture). 1 api or skeletons, 2 graph inputs, 3 buffer, 4 snapshot already marked, 5 old input or
// weight, 6 a native recovery owns the input, 7 buffer memory, 8 roots, 9 body displaced over 4 m, 10 ownership after
// the capture, 11 marker not set by the capture, 12 input not bound to the buffer, 13 the native weight setter refused.
unsigned carrySnapshotRefusal=0;
// 1214dt: the input binding found at the last snapshot start (16 bytes; the first 8 are the bound buffer).
std::array<unsigned char,16> carrySnapshotOldInput{};bool carrySnapshotReusedNative=false;
bool startCarrySnapshot(CarrySnapshotBlend& out,const NativeSkeleton& victim,const NativeSkeleton& carrier){
    carrySnapshotRefusal=1;
    if(!carrySnapshotApi()||!victim.stable||!carrier.stable||victim.parents.size()>256)return false;
    CarrySnapshotBlend s{};s.actor=victim.actor;s.animation=victim.animation;s.handle=victim.handle;
    s.input=locomotionParameter(s.animation,0x4a8,"recoverFromRagdollSnapshotInput",13,16);
    s.weight=locomotionParameter(s.animation,0x4b0,"recoverFromRagdollBlendInput",1,4);
    unsigned char valid=0;
    carrySnapshotRefusal=2;if(!s.input||!s.weight)return false;
    carrySnapshotRefusal=3;if(!memoryRead(s.animation+0x2c60,&s.buffer,8)||!s.buffer||(s.buffer&15)||
       !memoryRead(s.animation+0x2c68,&s.bytes,4)||s.bytes<victim.parents.size()*48||s.bytes>0x40000)return false;
    carrySnapshotRefusal=4;if(!memoryRead(s.animation+0x2c6c,&valid,1)||valid)return false;
    carrySnapshotRefusal=5;if(!memoryRead(s.input,s.oldInput.data(),16)||!memoryRead(s.weight,&s.oldWeight,4)||s.oldWeight!=0)return false;
    // 1214dt (#11 carry): on the NPC-authority host the choked body keeps an idle native recovery binding of its own
    // snapshot buffer (weight 0, marker clear; 1277w, confirmed runtime), which refused every product attachment there.
    // That binding is reused: the capture writes the same buffer and stop restores the old binding, weight and marker
    // exactly. A binding of any other buffer is still a native recovery owner and is left alone.
    carrySnapshotRefusal=6;carrySnapshotOldInput=s.oldInput;carrySnapshotReusedNative=false;
    {bool empty=true;for(unsigned char v:s.oldInput)if(v)empty=false;
     std::uintptr_t bound=0;std::memcpy(&bound,s.oldInput.data(),8);
     // 1214dv: 1277x read [0x0, 0x201] there: nothing bound, only flag bits. Only a non-null binding of another buffer
     // is a native recovery owner.
     if(bound&&bound!=s.animation+0x2c60)return false;if(!empty)carrySnapshotReusedNative=true;}
    carrySnapshotRefusal=7;MEMORY_BASIC_INFORMATION region{};
    if(!VirtualQuery(reinterpret_cast<void*>(s.buffer),&region,sizeof(region))||region.State!=MEM_COMMIT||
       (region.Protect&(PAGE_GUARD|PAGE_NOACCESS))||!(region.Protect&(PAGE_READWRITE|PAGE_WRITECOPY))||
       s.buffer+s.bytes>reinterpret_cast<std::uintptr_t>(region.BaseAddress)+region.RegionSize)return false;
    carrySnapshotRefusal=8;const auto oldRoot=carryRootMatrix(victim.root),newRoot=carryRootMatrix(carrier.root);
    if(!bodyRigid(oldRoot,10000)||!bodyRigid(newRoot,10000))return false;
    const auto delta=bodyMultiply(bodyInverse(newRoot),oldRoot);
    carrySnapshotRefusal=9;if(!bodyRigid(delta,4.f))return false; // Never blend a remotely displaced floor body into reach.
    float rotation[9]{};for(unsigned c=0;c<3;++c)for(unsigned r=0;r<3;++r)rotation[c*3+r]=delta.v[c*4+r];
    reinterpret_cast<void(*)(void*,const float*,const float*)>(gameImage+0x54de30)(reinterpret_cast<void*>(s.animation),delta.v+12,rotation);
    s.bound=s.captured=true;s.started=GetTickCount64();out=s;
    std::uintptr_t pointer=0;
    carrySnapshotRefusal=10;if(!carrySnapshotOwned(s))return false;
    carrySnapshotRefusal=11;if(!memoryRead(s.animation+0x2c6c,&valid,1)||valid!=1)return false;
    carrySnapshotRefusal=12;if(!memoryRead(s.input,&pointer,8)||pointer!=s.animation+0x2c60)return false;
    carrySnapshotRefusal=13;if(!reinterpret_cast<bool(*)(void*,float)>(gameImage+0x700fc0)(reinterpret_cast<void*>(s.animation+0x2728),1.f))return false;
    carrySnapshotRefusal=0;return true;
}
bool tickCarrySnapshotAmount(CarrySnapshotBlend& s,float u){
    if(!s.bound)return true;
    if(!std::isfinite(u)||u<0||u>1||!carrySnapshotOwned(s))return false;
    if(u>=1)return stopCarrySnapshot(s);
    const float weight=1-u*u*(3-2*u);
    return reinterpret_cast<bool(*)(void*,float)>(gameImage+0x700fc0)(reinterpret_cast<void*>(s.animation+0x2728),weight);
}
bool tickCarrySnapshot(CarrySnapshotBlend& s,ULONGLONG tick){
    return tickCarrySnapshotAmount(s,std::clamp(float(tick>s.started?tick-s.started:0)/800.f,0.f,1.f));
}

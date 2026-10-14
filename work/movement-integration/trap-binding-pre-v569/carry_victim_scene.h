#pragma once
// Explicit experiment only: an AIGroup1-only derivative of D2's native
// carry loop. Never bind the local player, arms, camera or an arbitrary NPC.
constexpr const char* kCarryVictimScene="midnight/d2coop/carry_victim_loop.midnightscene";
const char* carryVictimResource(unsigned variant){
    if(variant==2)return kCarryVictimScene;
    if(variant==4)return "midnight/d2coop/carry_victim_drop_low_stand.midnightscene";
    if(variant==5)return "midnight/d2coop/carry_victim_drop_fetal_hold.midnightscene";
    if(variant==6)return "midnight/d2coop/carry_victim_drop_low_sneak.midnightscene";
    if(variant==8)return "midnight/d2coop/carry_victim_drop_high.midnightscene";
    return nullptr;
}
struct CarryVictimScene {unsigned handle=0,variant=2;std::uintptr_t declaration=0;bool stopping=false,verified=false;};
bool carryVictimApis(){
    constexpr unsigned char a[]={0x48,0x89,0x74,0x24,0x20,0x4c,0x89,0x44,0x24,0x18,0x57,0x41,0x54,0x41,0x55,0x41};
    constexpr unsigned char b[]={0x48,0x8b,0xc4,0x57,0x48,0x83,0xec,0x40,0x48,0xc7,0x40,0xd8,0xfe,0xff,0xff,0xff};
    constexpr unsigned char c[]={0x48,0x83,0xec,0x38,0x45,0x33,0xc9,0x41,0xb8,0,6,0,0,0xc6,0x44,0x24};
    constexpr unsigned char lookup[]={0x44,0x88,0x44,0x24,0x18,0x55,0x56,0x57};
    constexpr unsigned char resolve[]={0x40,0x53,0x55,0x57,0x48,0x83,0xec,0x20,0x8b,0xb9,0xb8,0,0,0};
    return gameImage&&!std::memcmp(gameImage+0x7d6040,a,sizeof(a))&&!std::memcmp(gameImage+0x7d9270,b,sizeof(b))&&
        !std::memcmp(gameImage+0x7d9590,c,sizeof(c))&&!std::memcmp(gameImage+0x1e030,lookup,sizeof(lookup))&&
        !std::memcmp(gameImage+0x197a60,resolve,sizeof(resolve));
}
bool carryVictimInspect(const CarryVictimScene& s,const NativeWorldNpc& target,std::uintptr_t& instance,unsigned& phase){
    instance=0;phase=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!s.handle||!s.declaration||worldIdentityEntity(memoryRead,observedGame,target.handle)!=target.actor)return false;
    const auto manager=observedGame+0x1f5338;
    for(unsigned offset:{0u,0x610u}){std::uintptr_t list=0;unsigned flags=0;
        if(!memoryRead(manager+offset,&list,8)||!memoryRead(manager+offset+8,&flags,4)||(flags&0xffffff)>512||((flags&0xffffff)&&!list))return false;
        for(unsigned i=0;i<(flags&0xffffff);++i){std::uintptr_t p=0;unsigned id=0;
            if(!memoryRead(list+i*8,&p,8)||!p||!memoryRead(p+0x158,&id,4))return false;
            if(id==s.handle){if(instance&&instance!=p)return false;instance=p;}
        }
    }
    if(!instance)return true; // Native stop has retired this exact handle.
    std::uintptr_t decl=0,bindings=0,actor=0,vt=0,name=0;unsigned count=0;std::string text;
    return memoryRead(instance,&decl,8)&&decl==s.declaration&&memoryRead(decl,&vt,8)&&vt==base+0x1affbc8&&
        memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,text)&&carryVictimResource(s.variant)&&text==carryVictimResource(s.variant)&&
        memoryRead(instance+0x10,&bindings,8)&&bindings&&memoryRead(instance+0x18,&count,4)&&(count&0xffffff)==1&&
        memoryRead(bindings+8,&actor,8)&&actor==target.actor&&memoryRead(instance+0x27c,&phase,4)&&phase>=1&&phase<=8;
}
bool startCarryVictimScene(CarryVictimScene& s,const NativeWorldNpc& target,unsigned variant=2,unsigned elapsed=0){
    const char* resource=carryVictimResource(variant);
    if(s.handle||!resource||elapsed>700||!carryVictimApis()||target.handle==1||GetCurrentThreadId()!=engineThread.load())return false;
    s.variant=variant;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x32cf0b0,resource,false,false);
    std::uintptr_t vt=0,name=0,info=0,groups=0,groupName=0,spawn=0,aiGroup=0;unsigned count=0;std::string text;
    // Verify the installed derivative has exactly one NPC group and cannot
    // auto-spawn or bind first-person tracks. Original assets remain intact.
    if(!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1affbc8||!memoryRead(decl+0x10,&info,8)||info!=base+0x32cf0b0||
       !memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,text)||text!=resource||
       !memoryRead(decl+0xb0,&groups,8)||!groups||!memoryRead(decl+0xb8,&count,4)||(count&0xffffff)!=1||
       !memoryRead(groups+0x10,&groupName,8)||!worldIdentityString(memoryRead,groupName,text)||text!="AIGroup1"||
       !memoryRead(groups+0x328,&spawn,8)||spawn||!memoryRead(base+0x32ce5d0,&aiGroup,8)||aiGroup!=groupName)return false;
    // A newly parsed declaration has names at GroupInfo+68, but no resolved
    // MD6 list at +18. Native 197a60 resolves those resources; 7db9a0 needs
    // them to select a compatible skeleton before creating the Anim track.
    reinterpret_cast<void(*)(void*,bool)>(gameImage+0x197a60)(reinterpret_cast<void*>(decl),true);
    std::uintptr_t models=0;unsigned modelCount=0;
    if(!memoryRead(groups+0x18,&models,8)||!models||!memoryRead(groups+0x20,&modelCount,4)||
       !(modelCount&0xffffff)||(modelCount&0xffffff)>8)return false;
    for(unsigned i=0;i<(modelCount&0xffffff);++i){std::uintptr_t model=0,modelVt=0;
        if(!memoryRead(models+i*8,&model,8)||!model||!memoryRead(model,&modelVt,8)||modelVt!=base+0x1b05218)return false;}
    struct Binding{std::uintptr_t name;unsigned flags,pad;const unsigned* handles;unsigned count,capacity;std::uint64_t inlineHandles;std::uint64_t tail;};
    static_assert(sizeof(Binding)==0x30);
    const unsigned targetHandle=target.handle;
    Binding binding{aiGroup,0,0,&targetHandle,0x5000001,0x1ff,0,0};
    struct List{const Binding* data;unsigned count,capacity;} list{&binding,0x5000001,0x140};
    std::array<unsigned char,8> options{};s.declaration=decl;
    reinterpret_cast<unsigned*(*)(void*,unsigned*,void*,void*,void*,void*,void*)>(gameImage+0x7d6040)(
        reinterpret_cast<void*>(observedGame+0x1f5338),&s.handle,reinterpret_cast<void*>(decl),&list,options.data(),nullptr,nullptr);
    std::uintptr_t instance=0;unsigned phase=0;
    if(!s.handle||!carryVictimInspect(s,target,instance,phase)||!instance)return false;
    // 7d2160 stores argument3's value as the initial scene clock (+160).
    // Argument4 is a separate start/seek behavior flag, not elapsed time.
    int start=static_cast<int>(elapsed),time=0;
    const bool ok=reinterpret_cast<bool(*)(void*,unsigned,int*,int*,int*,unsigned)>(gameImage+0x7d9270)(
        reinterpret_cast<void*>(observedGame+0x1f5338),s.handle,&start,&time,nullptr,0x100002);
    s.verified=ok&&carryVictimInspect(s,target,instance,phase)&&instance&&phase==3;
    return s.verified;
}
bool stopCarryVictimScene(CarryVictimScene& s,const NativeWorldNpc& target){
    if(!s.handle)return true;std::uintptr_t instance=0;unsigned phase=0;
    if(!carryVictimApis()||!carryVictimInspect(s,target,instance,phase))return false;
    if(!instance){s={};return true;}
    if(!s.stopping){reinterpret_cast<void(*)(void*,unsigned)>(gameImage+0x7d9590)(reinterpret_cast<void*>(observedGame+0x1f5338),s.handle);s.stopping=true;}
    if(!carryVictimInspect(s,target,instance,phase))return false;
    if(!instance){s={};return true;}return false;
}

#pragma once
// Cosmetic441 native FX/material tracks only; body pose/root stay network-owned.
using DoppelgangerFxTarget=NativeDoppelgangerVisual;
const char* doppelgangerFxResource(unsigned variant){
 return variant==1?"midnight/d2coop/doppelganger_fx_spawn.midnightscene":variant==2?"midnight/d2coop/doppelganger_fx_despawn.midnightscene":nullptr;
}
unsigned doppelgangerFxSceneStage=0,doppelgangerFxPhaseBefore=0,doppelgangerFxPhaseAfter=0;
int doppelgangerFxStartError=-1;bool doppelgangerFxStartOk=false;
struct DoppelgangerFxScene {unsigned handle=0,variant=1;std::uintptr_t declaration=0;bool stopping=false,verified=false;};
bool doppelgangerFxSceneInspect(const DoppelgangerFxScene& s,const DoppelgangerFxTarget& target,std::uintptr_t& instance,unsigned& phase){
    instance=0;phase=0;const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!s.handle||!s.declaration||!currentDoppelgangerVisual(target,observedGame))return false;
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
        memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,text)&&doppelgangerFxResource(s.variant)&&text==doppelgangerFxResource(s.variant)&&
        memoryRead(instance+0x10,&bindings,8)&&bindings&&memoryRead(instance+0x18,&count,4)&&(count&0xffffff)==1&&
        memoryRead(bindings+8,&actor,8)&&actor==target.actor&&memoryRead(instance+0x27c,&phase,4)&&phase>=1&&phase<=8;
}
bool startDoppelgangerFxScene(DoppelgangerFxScene& s,const DoppelgangerFxTarget& target,unsigned variant=1,unsigned elapsed=0){
    doppelgangerFxSceneStage=1;
    const char* resource=doppelgangerFxResource(variant);
    if(s.handle||!resource||elapsed>=doppelgangerFxDuration(variant)||!currentDoppelgangerVisual(target,observedGame)||!carryVictimApis()||target.handle==1||GetCurrentThreadId()!=engineThread.load())return false;
    s.variant=variant;
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    doppelgangerFxSceneStage=2;
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x32cf0b0,resource,false,false);
    std::uintptr_t vt=0,name=0,info=0,groups=0,groupName=0,spawn=0;unsigned count=0;std::string text;
    // Verify the installed derivative has exactly one cosmetic model group and cannot
    // auto-spawn or bind first-person tracks. Original assets remain intact.
    if(!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1affbc8||!memoryRead(decl+0x10,&info,8)||info!=base+0x32cf0b0||
       !memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,text)||text!=resource||
       !memoryRead(decl+0xb0,&groups,8)||!groups||!memoryRead(decl+0xb8,&count,4)||(count&0xffffff)!=1||
       !memoryRead(groups+0x10,&groupName,8)||!worldIdentityString(memoryRead,groupName,text)||text!="AIGroup1"||
       !memoryRead(groups+0x328,&spawn,8)||spawn)return false;
    // A newly parsed declaration has names at GroupInfo+68, but no resolved
    // MD6 list at +18. Native 197a60 resolves those resources; 7db9a0 needs
    // them to select a compatible skeleton before creating the Anim track.
    doppelgangerFxSceneStage=3;
    reinterpret_cast<void(*)(void*,bool)>(gameImage+0x197a60)(reinterpret_cast<void*>(decl),true);
    std::uintptr_t models=0;unsigned modelCount=0;
    if(!memoryRead(groups+0x18,&models,8)||!models||!memoryRead(groups+0x20,&modelCount,4)||
       !(modelCount&0xffffff)||(modelCount&0xffffff)>8)return false;
    for(unsigned i=0;i<(modelCount&0xffffff);++i){std::uintptr_t model=0,modelVt=0;
        if(!memoryRead(models+i*8,&model,8)||!model||!memoryRead(model,&modelVt,8)||modelVt!=base+0x1b05218)return false;}
    doppelgangerFxSceneStage=4;
    struct Binding{std::uintptr_t name;unsigned flags,pad;const unsigned* handles;unsigned count,capacity;std::uint64_t inlineHandles;std::uint64_t tail;};
    static_assert(sizeof(Binding)==0x30);
    const unsigned targetHandle=target.handle;
    Binding binding{groupName,0,0,&targetHandle,0x5000001,0x1ff,0,0};
    struct List{const Binding* data;unsigned count,capacity;} list{&binding,0x5000001,0x140};
    std::array<unsigned char,8> options{};
    // 7ce370 copies option byte4 to instance+2b4 and its force-update bit +2b2.
    // 7cf580 otherwise LOD-pauses offscreen groups; 7d9fa0/7da0b0 then pause
    // the entire scene directly. These bounded FX/material-only scenes must
    // keep source time and finish even when the network-owned clone is culled.
    options[4]=1;s.declaration=decl;
    reinterpret_cast<unsigned*(*)(void*,unsigned*,void*,void*,void*,void*,void*)>(gameImage+0x7d6040)(
        reinterpret_cast<void*>(observedGame+0x1f5338),&s.handle,reinterpret_cast<void*>(decl),&list,options.data(),nullptr,nullptr);
    doppelgangerFxSceneStage=5;
    std::uintptr_t instance=0;unsigned phase=0;
    if(!s.handle||!doppelgangerFxSceneInspect(s,target,instance,phase)||!instance)return false;
    // 7d2160 stores argument3's value as the initial scene clock (+160).
    // Argument4 is a separate start/seek behavior flag, not elapsed time.
    doppelgangerFxPhaseBefore=phase;
    doppelgangerFxSceneStage=6;
    int start=static_cast<int>(elapsed),time=0;
    const bool ok=reinterpret_cast<bool(*)(void*,unsigned,int*,int*,int*,unsigned)>(gameImage+0x7d9270)(
        reinterpret_cast<void*>(observedGame+0x1f5338),s.handle,&start,&time,&doppelgangerFxStartError,0x100002);
    doppelgangerFxSceneStage=7;
    doppelgangerFxStartOk=ok;
    const bool inspected=doppelgangerFxSceneInspect(s,target,instance,phase);doppelgangerFxPhaseAfter=phase;
    s.verified=ok&&inspected&&instance&&phase==3;
    if(s.verified)doppelgangerFxSceneStage=8;
    return s.verified;
}
bool stopDoppelgangerFxScene(DoppelgangerFxScene& s,const DoppelgangerFxTarget& target){
    if(!s.handle)return true;std::uintptr_t instance=0;unsigned phase=0;
    if(!carryVictimApis()||!doppelgangerFxSceneInspect(s,target,instance,phase))return false;
    if(!instance){s={};return true;}
    if(!s.stopping){reinterpret_cast<void(*)(void*,unsigned)>(gameImage+0x7d9590)(reinterpret_cast<void*>(observedGame+0x1f5338),s.handle);s.stopping=true;}
    if(!doppelgangerFxSceneInspect(s,target,instance,phase))return false;
    if(!instance){s={};return true;}return false;
}

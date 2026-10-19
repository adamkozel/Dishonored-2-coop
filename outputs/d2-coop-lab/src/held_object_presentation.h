#pragma once
// 1214ea (#11 carry_object_place_throw): the observer's copy of the source's held native movable. While the source holds
// it (phase 1) the copy is pinned 7 cm past the partner's presented right wrist along the forearm, upright and facing
// along it; during the flight after the release (phase 2) and when it rests (phase 3) the copy takes the source's world
// transform (map entities share handles across the two worlds; the name key must match); when the source's copy is gone
// (phase 4) the observer breaks its copy with one native explosion-damage hit on the object's health component. Engine
// thread only, through the native entity transform setter (0x87d200, as guard_link's nudge).
bool breakPartnerHeldObject(std::uint32_t handle,const float* at){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeHealth h{};
    if(!realNativeDamage||!swordDamageApiMatches()||!readNativeHealth(memoryRead,base,handle,h)||!h.component||h.current<=0)return false;
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,"components/equipment/damage/explosion_grenade.damage",false,false);
    std::uintptr_t vt=0,manager=0;if(!decl||!memoryRead(decl,&vt,8)||vt!=base+0x1c5d138||!memoryRead(decl+0x10,&manager,8)||manager!=base+0x3990b60)return false;
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    const unsigned regions=1,region=0;const float direction[3]={0,0,-1};float position[3]{at[0],at[1],at[2]+.2f};const std::uint32_t attacker=0x3fff0010;
    std::memcpy(params.data()+0x20,&regions,4);std::memcpy(params.data()+0x4c,&region,4);
    std::memcpy(params.data()+0x24,position,12);std::memcpy(params.data()+0x3c,direction,12);std::memcpy(params.data()+0x54,&attacker,4);
    realNativeDamage(reinterpret_cast<void*>(h.component),params.data(),result.data());return true;
}
void servicePartnerHeldObject(ULONGLONG tick){
    if(!nativeMutationAllowed||GetCurrentThreadId()!=engineThread.load())return;
    const auto r=remoteHeld;auto& s=partnerHeld;
    if(!r.phase||!r.at||tick-r.at>600){s.lastPhase=0;return;}
    const auto actor=worldIdentityEntity(memoryRead,observedGame,r.handle);std::uintptr_t text=0;std::string name;
    const bool same=actor&&memoryRead(actor+0x20,&text,8)&&text&&worldIdentityString(memoryRead,text,name)&&carryIdentityKey(name)==r.key;
    if(r.phase==4){if(s.lastPhase!=4){s.lastPhase=4;float at[3]{};if(same&&memoryRead(actor+0x300,at,12)&&breakPartnerHeldObject(r.handle,at)){++s.broken;s.result="broken";}else s.result="break_skipped";}return;}
    if(!same){++s.refused;s.result="object_not_found";return;}
    float position[3]{},axis[9]{};
    if(r.phase==1){PartnerHeldWrist w{};AcquireSRWLockShared(&partnerHeldWristLock);w=partnerHeldWrist;ReleaseSRWLockShared(&partnerHeldWristLock);
        if(!w.tick||tick-w.tick>250){++s.refused;s.result="no_wrist";return;}
        float f[3]{},fl=0;for(unsigned i=0;i<3;++i){f[i]=w.wrist[i]-w.elbow[i];fl+=f[i]*f[i];}fl=std::sqrt(fl);
        if(!std::isfinite(fl)||fl<.05f||fl>1.f){++s.refused;s.result="wrist_invalid";return;}
        for(unsigned i=0;i<3;++i){f[i]/=fl;position[i]=w.wrist[i]+f[i]*.07f;}
        float hx=f[0],hy=f[1];const float hl=std::sqrt(hx*hx+hy*hy);if(hl<.05f){hx=1;hy=0;}else{hx/=hl;hy/=hl;}
        const float upright[9]={hx,hy,0,-hy,hx,0,0,0,1};std::memcpy(axis,upright,36);++s.applied;s.result="held";
    }else{std::memcpy(position,r.position,12);std::memcpy(axis,r.axis,36);
        if(r.phase==2){++s.followed;s.result="following";}else{++s.settled;s.result="settled";}}
    reinterpret_cast<void(*)(void*,const float*,const float*,bool,int)>(gameImage+0x87d200)(reinterpret_cast<void*>(actor),position,axis,true,0);
    s.handle=r.handle;s.lastPhase=r.phase;
}

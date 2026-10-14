#pragma once
// Native reconstruction of the independently captured nonfatal player sword
// hit. Only the fixed declaration and observed small-guard regions are used.
bool nativePlayerSwordDeclaration(std::uintptr_t decl){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vt=0,manager=0,name=0;std::uint64_t type=0;float amount=0;unsigned reactions[2]{};std::string path;
    return decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&
        memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&path==kPlayerSwordDamageDecl&&
        memoryRead(decl+0x5c,&amount,4)&&amount==10&&memoryRead(decl+0x60,&type,8)&&type==0x80&&
        memoryRead(decl+0x68,reactions,8)&&reactions[0]==1&&reactions[1]==1;
}
bool applyNativePlayerSword(const NativeHealth& victim,std::uint32_t attacker,const EnemyMeleeGeometry& geometry,float& inflicted){
    if(GetCurrentThreadId()!=engineThread.load()||!swordDamageApiMatches()||!validEnemyMeleeGeometry(geometry)||victim.handle==1||
       victim.logic!=1||victim.current!=40||victim.maximum!=40||victim.game!=observedGame||!worldIdentityEntity(memoryRead,victim.game,attacker))return false;
    NativeHealth current{};
    if(!readNativeHealth(memoryRead,reinterpret_cast<std::uintptr_t>(gameImage),victim.handle,current)||current.game!=victim.game||current.actor!=victim.actor||
       current.component!=victim.component||current.declaration!=victim.declaration||current.logic!=1||current.current!=40||current.maximum!=40)return false;
    float location[3]{};if(!memoryRead(current.actor+0x300,location,12))return false;
    for(unsigned i=0;i<3;++i){location[i]+=geometry.offset[i];if(!std::isfinite(location[i]))return false;}
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,kPlayerSwordDamageDecl,false,false);
    if(!nativePlayerSwordDeclaration(decl))return false;
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    std::memcpy(params.data()+0x20,&geometry.regions,4);std::memcpy(params.data()+0x24,location,12);
    std::memcpy(params.data()+0x30,geometry.normal,12);std::memcpy(params.data()+0x3c,geometry.direction,12);
    std::memcpy(params.data()+0x4c,&geometry.region,4);std::memcpy(params.data()+0x54,&attacker,4);
    reinterpret_cast<NativeDamage>(gameImage+0x717950)(reinterpret_cast<void*>(current.component),params.data(),result.data());
    std::memcpy(&inflicted,result.data(),4);return std::isfinite(inflicted);
}

#pragma once
// Narrow native reconstruction for the observed NPC sword hit. Declarations
// are resolved locally; a raw declaration pointer is never a wire identity.
constexpr const char* kSwordDamageDecl="components/equipment/damage/npc_sword.damage";
bool swordDamageApiMatches(){
    const unsigned char lookup[]={0x44,0x88,0x44,0x24,0x18,0x55,0x56,0x57,0x41,0x54,0x41,0x55,0x41,0x56,0x41,0x57};
    const unsigned char ctor[]={0x48,0x89,0x5c,0x24,0x08,0x57,0x48,0x83,0xec,0x20,0x48,0x89,0x11};
    const unsigned char result[]={0x33,0xc0,0x48,0x89,0x01,0x89,0x41,0x08,0x48,0x89,0x41,0x10};
    return gameImage&&realNativeDamage&&!std::memcmp(gameImage+0x1e030,lookup,sizeof(lookup))&&
        !std::memcmp(gameImage+0x780260,ctor,sizeof(ctor))&&!std::memcmp(gameImage+0x780470,result,sizeof(result));
}
bool nativeSwordDeclaration(std::uintptr_t decl){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vt=0,manager=0,name=0;std::uint64_t type=0;float amount=0;unsigned reactions[2]{};std::string path;
    return decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&
        memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&path==kSwordDamageDecl&&
        memoryRead(decl+0x5c,&amount,4)&&amount==20&&memoryRead(decl+0x60,&type,8)&&type==0x80&&
        memoryRead(decl+0x68,reactions,8)&&reactions[0]==1&&reactions[1]==1;
}
// The first local reconstruction uses a validated authored guard as attacker
// and no inflictor. Sword ownership resolution is not yet implemented.
bool applyNativeSwordSample(const NativeHealth& player,std::uint32_t attacker,const float* location,const float* direction,float& inflicted){
    if(GetCurrentThreadId()!=engineThread.load()||!swordDamageApiMatches()||player.handle!=1||player.logic!=1||player.current!=56||player.maximum!=70)return false;
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,kSwordDamageDecl,false,false);
    if(!nativeSwordDeclaration(decl))return false;
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    const unsigned regions=1,region=0;
    std::memcpy(params.data()+0x20,&regions,4);std::memcpy(params.data()+0x4c,&region,4);
    std::memcpy(params.data()+0x24,location,12);std::memcpy(params.data()+0x3c,direction,12);
    std::memcpy(params.data()+0x54,&attacker,4);
    // Use the hooked entry to preserve the independent native damage record.
    reinterpret_cast<NativeDamage>(gameImage+0x717950)(reinterpret_cast<void*>(player.component),params.data(),result.data());
    std::memcpy(&inflicted,result.data(),4);return std::isfinite(inflicted);
}
std::uint64_t swordSampleEpoch=0,swordSampleApplied=0,swordSampleRefused=0;
float swordSampleBefore=0,swordSampleAfter=0,swordSampleInflicted=0;
DWORD swordSampleThread=0;const char* swordSampleResult="idle";
void beginSwordDamageSample(bool behind=false){
    const auto refuse=[&](const char* reason){++swordSampleRefused;swordSampleResult=reason;};
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);NativeHealth player{};NativeWorldNpc guard{};
    if(!localRole||!guardPairReady(GetTickCount64())||swordSampleEpoch==outgoing.worldEpoch||
       !readNativeHealth(memoryRead,base,1,player)||player.game!=observedGame||player.current!=56||player.maximum!=70){refuse("scope");return;}
    std::vector<NativeWorldNpc> catalog;unsigned unbound=0,matches=0;
    if(!readWorldNpcCatalog(memoryRead,base,observedGame,catalog,unbound)){refuse("catalog");return;}
    for(const auto& n:catalog)if(n.spawnerName=="ld_spawner_guard_escape_1"&&n.definition=="models/characters/def/generic/guard_male_small_01.def"&&n.unique&&n.reciprocal){guard=n;++matches;}
    float position[3]{},direction[3]{};
    if(matches!=1||!memoryRead(player.actor+0x300,position,12)||!memoryRead(player.actor+0x30c,direction,12)){refuse("identity");return;}
    for(float v:position)if(!std::isfinite(v)){refuse("position");return;}
    float length=0;for(float v:direction){if(!std::isfinite(v)){refuse("direction");return;}length+=v*v;}
    if(length<.99f||length>1.01f){refuse("direction");return;}
    // Native 717800 uses forward dot incoming direction to select the
    // difficulty's front/back multiplier (647ff0 / 647fc0 respectively).
    if(!behind)for(float& v:direction)v=-v;position[2]+=1;
    swordSampleEpoch=outgoing.worldEpoch;swordSampleBefore=player.current;swordSampleThread=GetCurrentThreadId();
    if(!applyNativeSwordSample(player,guard.handle,position,direction,swordSampleInflicted)){refuse("native_call");return;}
    NativeHealth after{};
    if(!readNativeHealth(memoryRead,base,1,after)||after.actor!=player.actor||after.component!=player.component){refuse("readback");return;}
    swordSampleAfter=after.current;++swordSampleApplied;swordSampleResult="applied";
}
void writeSwordSampleStatus(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.sword-sample.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"result\":\"%s\",\"epoch\":%llu,\"applied\":%llu,\"refused\":%llu,\"thread\":%lu,\"before\":%.9g,\"after\":%.9g,\"inflicted\":%.9g}\n",swordSampleResult,swordSampleEpoch,swordSampleApplied,swordSampleRefused,swordSampleThread,swordSampleBefore,swordSampleAfter,swordSampleInflicted);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

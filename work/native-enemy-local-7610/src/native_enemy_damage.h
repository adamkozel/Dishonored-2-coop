#pragma once
// Local, once-per-world research of the declaration independently observed
// during a real player assassination. This is not a network outcome adapter.
constexpr char kAssassinationDamage[]="components/characters/player/base/damage/assassination_damage.damage";
bool nativeAssassinationDeclaration(std::uintptr_t decl){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    std::uintptr_t vt=0,manager=0,name=0;std::uint64_t type=0;float amount=0;unsigned reactions[2]{};std::string path;
    return decl&&memoryRead(decl,&vt,8)&&vt==base+0x1c5d138&&memoryRead(decl+0x10,&manager,8)&&manager==base+0x3990b60&&
        memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,path)&&path==kAssassinationDamage&&
        memoryRead(decl+0x5c,&amount,4)&&amount==0&&memoryRead(decl+0x60,&type,8)&&type==0x8c&&
        memoryRead(decl+0x68,reactions,8)&&!reactions[0]&&!reactions[1];
}
bool applyNativeAssassination(const NativeHealth& victim,std::uint32_t attacker,float& inflicted){
    if(GetCurrentThreadId()!=engineThread.load()||!swordDamageApiMatches()||victim.handle==1||victim.logic!=1||victim.current!=40||victim.maximum!=40||
       !worldIdentityEntity(memoryRead,victim.game,attacker))return false;
    const auto decl=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030)(gameImage+0x3990b60,kAssassinationDamage,false,false);
    if(!nativeAssassinationDeclaration(decl))return false;
    alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
    reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(decl));
    reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
    // Preserve native defaults (including absent inflictor and no hit region).
    // Arbitrary bytes from the source damage callback are never replayed.
    std::memcpy(params.data()+0x54,&attacker,4);
    reinterpret_cast<NativeDamage>(gameImage+0x717950)(reinterpret_cast<void*>(victim.component),params.data(),result.data());
    std::memcpy(&inflicted,result.data(),4);return std::isfinite(inflicted);
}
std::uint64_t enemySampleEpoch=0,enemySampleApplied=0,enemySampleRefused=0;
float enemySampleBefore=0,enemySampleAfter=0,enemySampleInflicted=0;
std::uint32_t enemySampleHandle=0;DWORD enemySampleThread=0;const char* enemySampleResult="idle";
void beginEnemyDamageSample(){
    const auto refuse=[&](const char* reason){++enemySampleRefused;enemySampleResult=reason;};
    NativeHealth victim{},player{};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);
    if(!localRole||GetCurrentThreadId()!=engineThread.load()||enemySampleEpoch==outgoing.worldEpoch||!guardPairReady(GetTickCount64())||
       guardLink.phase!=GuardPhase::Active||guardLink.key.mode!=1||!guardReadLocal()||!guardAiPaused()||!guardAttackInactive()||
       !guardReadHealth(victim)||victim.current!=40||!readNativeHealth(memoryRead,base,1,player)||player.current!=56){refuse("scope");return;}
    const auto epoch=outgoing.worldEpoch;
    // Return AI and frame ownership before native death changes component
    // states. Do not keep network pose/weapon writes alive on a dying actor.
    stopGuardLink("enemy_damage_sample");
    NativeHealth current{};
    if(epoch!=outgoing.worldEpoch||!readNativeHealth(memoryRead,base,victim.handle,current)||current.actor!=victim.actor||
       current.component!=victim.component||current.game!=victim.game||current.current!=40){refuse("replacement");return;}
    enemySampleEpoch=epoch;enemySampleBefore=current.current;enemySampleHandle=current.handle;enemySampleThread=GetCurrentThreadId();
    if(!applyNativeAssassination(current,1,enemySampleInflicted)){refuse("native_call");return;}
    NativeHealth after{};
    if(!readNativeHealth(memoryRead,base,current.handle,after)||after.actor!=current.actor||after.component!=current.component){refuse("readback");return;}
    enemySampleAfter=after.current;++enemySampleApplied;enemySampleResult="applied";
}
void writeEnemySampleStatus(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.enemy-sample.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"localExperiment\":true,\"result\":\"%s\",\"epoch\":%llu,\"applied\":%llu,\"refused\":%llu,\"handle\":%u,\"thread\":%lu,\"before\":%.9g,\"after\":%.9g,\"inflicted\":%.9g}\n",
        enemySampleResult,enemySampleEpoch,enemySampleApplied,enemySampleRefused,enemySampleHandle,enemySampleThread,enemySampleBefore,enemySampleAfter,enemySampleInflicted);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

#pragma once
// Developer-only, fixed six-call Dunwall test. No arbitrary target or damage
// declaration input and no packet-triggered calls. Once per world incarnation.
struct FriendlyFireProbe {
    std::uint64_t epoch=0,applied=0,refused=0;
    DWORD thread=0;unsigned count=0;const char* result="idle";
    float playerBefore=0,playerAfter=0,partnerBefore=0,partnerAfter=0;
    struct Row {unsigned target=0,attacker=0,inflictor=0;float inflicted=0;bool assassination=false,blocked=false;};
    std::array<Row,6> rows{};
} friendlyFireProbe;
void probeFriendlyFire(){
    auto& p=friendlyFireProbe;const auto refuse=[&](const char* why){++p.refused;p.result=why;};
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);const auto identity=shadowDamageIdentity();
    NativeHealth player{},partner{};NativePlayerSword localSword{};NativeSwordEquipment remoteSword{};
    if(GetCurrentThreadId()!=engineThread.load()||!swordDamageApiMatches()||!guardPairReady(GetTickCount64())||
       !avatarAiHeld||p.epoch==outgoing.worldEpoch||!validCoopDamageAvatar(memoryRead,base,identity)||
       identity.actor!=probeActor||identity.game!=observedGame||identity.handle!=probeHandle||
       !readNativeHealth(memoryRead,base,1,player)||player.logic!=1||player.current!=56||player.maximum!=70||
       !readNativeHealth(memoryRead,base,probeHandle,partner)||partner.logic!=1||partner.current!=40||partner.maximum!=40){refuse("scope");return;}
    if(!readNativePlayerSword(memoryRead,base,localSword)||
       !readNativeSwordEquipment(memoryRead,base,probeHandle,remoteSword,localRole?NativeSwordScope::DoppelgangerCorvo:NativeSwordScope::DoppelgangerEmily)){
        refuse("equipment");return;
    }
    const auto lookup=reinterpret_cast<std::uintptr_t(*)(void*,const char*,bool,bool)>(gameImage+0x1e030);
    const auto sword=lookup(gameImage+0x3990b60,kSwordDamageDecl,false,false);
    const auto assassination=lookup(gameImage+0x3990b60,kAssassinationDamage,false,false);
    if(!nativeSwordDeclaration(sword)||!nativeAssassinationDeclaration(assassination)){refuse("declaration");return;}
    p.epoch=outgoing.worldEpoch;p.thread=GetCurrentThreadId();p.count=0;p.rows={};p.playerBefore=player.current;p.partnerBefore=partner.current;
    for(unsigned n=0;n<p.rows.size();++n){
        auto& row=p.rows[n];const bool toPlayer=n==2||n==3||n==5;
        row.target=toPlayer?1:probeHandle;row.assassination=n>=4;
        row.attacker=n==1||n==3?0:(toPlayer?probeHandle:1);
        row.inflictor=n==1?localSword.sword.weaponHandle:(n==3?remoteSword.weaponHandle:0);
        const auto component=toPlayer?player.component:partner.component;
        if(classifyCoopFriendlyHit(memoryRead,base,identity,component,row.inflictor,row.attacker)==CoopFriendlyHit::None){refuse("attribution");return;}
        const auto before=toPlayer?friendlyFirePartnerBlocked.load():friendlyFireLocalBlocked.load();
        alignas(16) std::array<unsigned char,144> params{};alignas(16) std::array<unsigned char,32> result{};
        reinterpret_cast<void(*)(void*,void*)>(gameImage+0x780260)(params.data(),reinterpret_cast<void*>(row.assassination?assassination:sword));
        reinterpret_cast<void(*)(void*)>(gameImage+0x780470)(result.data());
        std::memcpy(params.data()+0x50,&row.inflictor,4);std::memcpy(params.data()+0x54,&row.attacker,4);
        reinterpret_cast<NativeDamage>(gameImage+0x717950)(reinterpret_cast<void*>(component),params.data(),result.data());
        std::memcpy(&row.inflicted,result.data(),4);
        row.blocked=(toPlayer?friendlyFirePartnerBlocked.load():friendlyFireLocalBlocked.load())==before+1&&row.inflicted==0;
        ++p.count;if(!row.blocked){refuse("native_filter");return;}
    }
    NativeHealth afterPlayer{},afterPartner{};
    if(!readNativeHealth(memoryRead,base,1,afterPlayer)||afterPlayer.actor!=player.actor||afterPlayer.component!=player.component||
       !readNativeHealth(memoryRead,base,probeHandle,afterPartner)||afterPartner.actor!=partner.actor||afterPartner.component!=partner.component){refuse("readback");return;}
    p.playerAfter=afterPlayer.current;p.partnerAfter=afterPartner.current;
    if(p.playerAfter!=p.playerBefore||p.partnerAfter!=p.partnerBefore){refuse("health_changed");return;}
    ++p.applied;p.result="passed";
}
void writeFriendlyFireProbe(){
    const auto& p=friendlyFireProbe;char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.friendly-fire.json",statusPath)>=sizeof(path)||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=sizeof(temp))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"localExperiment\":true,\"result\":\"%s\",\"epoch\":%llu,\"applied\":%llu,\"refused\":%llu,\"thread\":%lu,\"playerBefore\":%.9g,\"playerAfter\":%.9g,\"partnerBefore\":%.9g,\"partnerAfter\":%.9g,\"cases\":[",p.result,p.epoch,p.applied,p.refused,p.thread,p.playerBefore,p.playerAfter,p.partnerBefore,p.partnerAfter);
    for(unsigned n=0;n<p.count;++n){const auto& r=p.rows[n];fprintf(f,"%s{\"target\":%u,\"attacker\":%u,\"inflictor\":%u,\"assassination\":%s,\"inflicted\":%.9g,\"blocked\":%s}",n?",":"",r.target,r.attacker,r.inflictor,r.assassination?"true":"false",r.inflicted,r.blocked?"true":"false");}
    fputs("]}\n",f);const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

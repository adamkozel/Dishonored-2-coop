#pragma once
// 1181 product session controller. In product mode (installed release, no lab
// environment) or with the lab-only "coop_product_auto on", the DLL itself arms
// the standard co-op feature set for every new shared world, using the same
// validated commands the lab scripts send: cinematic sync/barrier, world
// observation, the partner avatar (Doppelganger rig when the partner assets are
// installed) with follow/locomotion/movement bank/shadow stream, avatar
// perception, door/usable/coin sessions, NPC stream/combat/attack, mission
// checkpoints and trigger mirroring. Commands for systems already armed are
// refused harmlessly by their owners (sessions renew across checkpoints).
// Product hotkeys (game window focused): F7 hosts (Steam lobby + invite
// overlay), F8 leaves the session. Joins come from Steam's own UI or the
// "+connect_lobby" launch argument.
// 1203: the controller also arms the movement resume policy (native pause and
// readable menus restore the partner presentation in place) and runs a
// presentation watchdog: if the partner avatar/stream/bank ended in the same
// world while both players are back in gameplay (cinematic, tutorial modal,
// resume fault), the presentation steps are re-run (guarded commands refuse
// whatever is still armed).
void dispatchCoopCommand(void* system,const char* text);
bool productPresentationState(ULONGLONG tick,bool& resuming,unsigned& missing);
bool productPeerStreamWaiting(ULONGLONG tick);
extern std::uint64_t bulkCongestionSkips;extern long long bulkQueueUsMax;
bool productAutoArm=false,productLaunchChecked=false;ULONGLONG productSteamRetryAt=0;
struct ProductArm {
    std::uint64_t epoch=0,peerEpoch=0,map=0;unsigned step=0,worlds=0;ULONGLONG nextAt=0;
    std::uint64_t dispatched=0,hostKeys=0,leaveKeys=0,maintenance=0,repairs=0,quickRepairs=0;ULONGLONG lostAt=0,liveSince=0,repairBackoff=0;unsigned missing=0,repairMask=0,repairStep=0;
    const char* result="idle";const char* last="";
};
ProductArm productArm{};
SHORT productKeyF7=0,productKeyF8=0;
const char* productStep(unsigned i){
    switch(i){
    case 0:return "coop_cinematic_sync on";
    case 1:return "coop_cinematic_barrier on";
    case 2:return "coop_world_observe on";
    case 3:return partnerAssetsEnabled?(localRole?"coop_spawn_probe shadow_corvo":"coop_spawn_probe shadow_emily"):(localRole?"coop_spawn_probe corvo":"coop_spawn_probe emily");
    case 4:return "coop_probe_follow exact";
    case 5:return "coop_probe_locomotion on";
    case 6:return "coop_movement_bank on";
    case 7:return "coop_shadow_stream on";
    case 8:return "coop_avatar_perception on";
    case 9:return "coop_door_sync session";
    case 10:return "coop_usable_sync session";
    case 11:return "coop_coin_sync on";
    case 12:return "coop_npc_stream on";
    case 13:return "coop_npc_combat on";
    case 14:return "coop_npc_attack on";
    case 15:return "coop_checkpoint_sync on";
    case 16:return "coop_trigger_mirror on";
    case 17:return "coop_movement_resume on";
    case 18:return "coop_mission_end_mirror on"; // 1214bb: mission end, both-ready hold, chaos
    default:return nullptr;
    }
}
bool productWindowFocused(){DWORD pid=0;GetWindowThreadProcessId(GetForegroundWindow(),&pid);return pid==GetCurrentProcessId();}
void serviceProductSession(void* system,ULONGLONG tick){
    sourcePoseTimeline.rebaseOnSustainedLate=productMode||productAutoArm;
    if(!productMode&&!productAutoArm)return;
    auto& a=productArm;
    if(productMode){
        // 1193: acquire the game's Steam interfaces at boot (retry each second
        // until the game's own SteamAPI is up) so GameLobbyJoinRequested is
        // registered before the player accepts an invite or uses Join Game.
        if(!onlineSteam.ready&&tick>=productSteamRetryAt){productSteamRetryAt=tick+1000;acquireOnlineSteam();}
        if(!productLaunchChecked){productLaunchChecked=true;if(const auto lobby=onlineLaunchLobby())onlineSession.requestedLobby=lobby;}
        const bool focused=productWindowFocused();
        const SHORT f7=focused?GetAsyncKeyState(VK_F7):0,f8=focused?GetAsyncKeyState(VK_F8):0;
        if((f7&0x8000)&&!(productKeyF7&0x8000)){++a.hostKeys;startOnlineHost();}
        if((f8&0x8000)&&!(productKeyF8&0x8000)){++a.leaveKeys;stopOnlineSession("left_by_player");a={};}
        productKeyF7=f7;productKeyF8=f8;
    }
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000;
    if(!connected||!coopWorldPairReady(outgoing,remote)||outgoing.mapId!=remote.mapId){a.result=connected?"waiting_shared_gameplay":"waiting_partner";return;}
    if(a.epoch!=outgoing.worldEpoch||a.peerEpoch!=remote.worldEpoch||a.map!=outgoing.mapId){
        const std::uint64_t keep=a.dispatched,hk=a.hostKeys,lk=a.leaveKeys,rp=a.repairs;// repair backoff restarts per worldconst unsigned worlds=a.worlds+1;a={};a.dispatched=keep;a.worlds=worlds;a.hostKeys=hk;a.leaveKeys=lk;a.repairs=rp;
        a.epoch=outgoing.worldEpoch;a.peerEpoch=remote.worldEpoch;a.map=outgoing.mapId;a.nextAt=tick+3000;a.result="world_settling";return;
    }
    if(tick<a.nextAt)return;
    const char* command=productStep(a.step);
    if(!command){
        // Maintenance: re-issue idempotent session commands every 5 s so a
        // system refused transiently (e.g. a checkpoint before the mission
        // autosave lands, or a session that ended) is armed again.
        static const char* maintain[]={"coop_checkpoint_sync on","coop_door_sync session","coop_usable_sync session","coop_coin_sync on","coop_npc_stream on","coop_trigger_mirror on","coop_movement_resume on"};
        constexpr unsigned kMaintain=sizeof(maintain)/sizeof(*maintain);
        a.result="armed";
        // 1203b targeted repair: only the missing pieces, in order, never a
        // follow re-enable while following and never a bank reload while one
        // is active or loading (a dormant bank is reactivated in place). The
        // grace (3 s) doubles after every repair up to 15 s (1203e; was 6/60 s)
        // and resets after 10 s of live presentation, so peers cannot ping-pong.
        if(a.repairMask){
            if(tick<a.nextAt)return;
            static const unsigned bits[]={1u,2u,2u,4u,8u,0u};
            while(a.repairStep<6&&bits[a.repairStep]&&!(a.repairMask&bits[a.repairStep]))++a.repairStep;
            const char* cmd=nullptr;
            switch(a.repairStep){
            case 0:cmd=productStep(3);break;      // spawn (refused if the avatar exists)
            case 1:cmd="coop_probe_follow exact";break;
            case 2:cmd="coop_probe_locomotion on";break;
            case 3:cmd="coop_movement_bank on";break;
            case 4:cmd="coop_shadow_stream on";break;
            case 5:cmd="coop_movement_resume on";break;
            default:break;
            }
            if(!cmd){a.repairMask=0;a.repairStep=0;a.result="armed";return;}
            dispatchCoopCommand(system,cmd);++a.dispatched;a.last=cmd;++a.repairStep;a.nextAt=tick+600;a.result="repairing_presentation";return;
        }
        bool resuming=false;unsigned missing=0;const bool live=productPresentationState(tick,resuming,missing);a.missing=missing;
        if(live){if(!a.liveSince)a.liveSince=tick;if(tick-a.liveSince>=10000)a.repairBackoff=0;}else a.liveSince=0;
        if(live||resuming||!coopGameplayReady(outgoing)||!coopGameplayReady(remote))a.lostAt=0;
        else if(!a.lostAt)a.lostAt=tick;
        else if(!missing)a.lostAt=tick; // only a bank load in flight: keep waiting
        // 1203c: bank/stream-only losses are cheap to repair (dormant bank
        // reactivation, stream handshake re-pin): repair after 1 s, at once
        // when the peer is already saying hello, and without backoff.
        else if(!(missing&3)&&(productPeerStreamWaiting(tick)||tick-a.lostAt>=1000)){
            a.lostAt=0;++a.quickRepairs;a.repairMask=missing;a.repairStep=0;a.nextAt=tick;a.result="repairing_presentation";return;
        }
        else if(tick-a.lostAt>=(a.repairBackoff?a.repairBackoff:3000)){
            a.lostAt=0;++a.repairs;a.repairBackoff=(std::min)(static_cast<ULONGLONG>((a.repairBackoff?a.repairBackoff:3000)*2),static_cast<ULONGLONG>(15000));
            // Missing avatar means a full presentation rebuild; missing follow
            // also needs the bank/stream that end with it.
            a.repairMask=(missing&1)?15u:(missing&2)?14u:missing;a.repairStep=0;a.nextAt=tick;a.result="repairing_presentation";return;
        }
        if(tick<a.nextAt)return;
        dispatchCoopCommand(system,maintain[a.maintenance%kMaintain]);++a.maintenance;++a.dispatched;a.last=maintain[(a.maintenance-1)%kMaintain];a.nextAt=tick+(a.maintenance%kMaintain?800:5000);return;}
    dispatchCoopCommand(system,command);++a.dispatched;a.last=command;++a.step;a.nextAt=tick+400;a.result="arming";
}
void writeMissionFollow();
// 1203f: send tally (see coopSendMessage). Rates are bytes/s since the last write.
void writeSendTally(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.send-tally.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    const auto tick=GetTickCount64();const double seconds=coopSendWindowAt?(std::max)(0.001,(tick-coopSendWindowAt)/1000.0):1.0;coopSendWindowAt=tick;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;
    fprintf(f,"{\"tick\":%llu,\"entries\":[",tick);bool first=true;
    for(auto& s:coopSendTally){if(!s.magic&&!s.messages)break;const auto rate=static_cast<std::uint64_t>(s.windowBytes/seconds);s.windowPeak=(std::max)(s.windowPeak,rate);
        char ascii[5]{};for(int b=0;b<4;++b){const char c=static_cast<char>((s.magic>>(8*b))&0xff);ascii[b]=(c>=32&&c<127&&c!='"'&&c!='\\')?c:'.';}
        fprintf(f,"%s{\"magic\":\"%08x\",\"ascii\":\"%s\",\"messages\":%llu,\"reliableBytes\":%llu,\"unreliableBytes\":%llu,\"rate\":%llu,\"peakRate\":%llu}",first?"":",",s.magic,ascii,s.messages,s.reliableBytes,s.unreliableBytes,rate,s.windowPeak);
        first=false;s.windowBytes=0;}
    fprintf(f,"]}\n");const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
void writeProductSession(){
    writeMissionFollow();writeSendTally();
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.product.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& a=productArm;
    fprintf(f,"{\"productMode\":%s,\"autoArm\":%s,\"result\":\"%s\",\"step\":%u,\"worlds\":%u,\"dispatched\":%llu,\"last\":\"%s\",\"hostKeys\":%llu,\"leaveKeys\":%llu,\"repairs\":%llu,\"quickRepairs\":%llu,\"bulkCongestionSkips\":%llu,\"bulkQueueUsMax\":%lld,\"lateRebases\":%llu,\"presentationLost\":%s,\"missing\":%u,\"repairBackoffMs\":%llu,\"role\":%u,\"partnerAssets\":%s,\"dataDir\":\"%s\"}\n",
        productMode?"true":"false",productAutoArm?"true":"false",a.result,a.step,a.worlds,a.dispatched,a.last,a.hostKeys,a.leaveKeys,a.repairs,a.quickRepairs,bulkCongestionSkips,bulkQueueUsMax,sourcePoseTimeline.lateRebases,a.lostAt?"true":"false",a.missing,a.repairBackoff,localRole,partnerAssetsEnabled?"true":"false",productMode?"set":"");
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}
// 1188 join-follow: the host publishes its current mission (idCommonLocal +0x2b8
// = current idDeclMapInfo, name at +8; chaos from the loaded map name); a
// joiner on another map loads the same mission natively with
// "campaign <mapinfo> <low|high> emily". Host plays Corvo (role 0).
#pragma pack(push,1)
struct MissionFollowPacket {
    std::uint32_t magic=0x464D3244; // "D2MF"
    std::uint16_t version=1,size=sizeof(MissionFollowPacket);
    std::uint32_t role=0,highChaos=0;
    std::uint64_t session=0,mapId=0,sequence=0;
    char mapinfo[160]{};
};
#pragma pack(pop)
static_assert(sizeof(MissionFollowPacket)==200);
bool followHostEnabled=false;
struct MissionFollow {
    char hostMapinfo[160]{};std::uint32_t hostHigh=0;std::uint64_t hostMap=0,sequence=0,peerSequence=0,sent=0,received=0,loads=0,refused=0;
    ULONGLONG sentAt=0,loadAt=0;std::uint64_t loadedForMap=0;const char* result="idle";
};
MissionFollow missionFollow{};
bool readCurrentMapinfo(char* out,std::size_t size,std::uint32_t& high){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t common=0,decl=0,name=0,vt=0;
    if(!memoryRead(base+0x2274420,&common,8)||!common||!memoryRead(common,&vt,8)||vt!=base+0x1ac4d88||!memoryRead(common+0x2b8,&decl,8)||!decl)return false;
    std::string text;if(!memoryRead(decl+8,&name,8)||!worldIdentityString(memoryRead,name,text)||text.size()>=size||text.rfind("campaign/",0)!=0)return false;
    std::snprintf(out,size,"%s",text.c_str());high=std::strstr(mapName,"highchaos")?1u:0u;return true;
}
void receiveMissionFollow(const MissionFollowPacket& p){
    auto& m=missionFollow;++m.received;
    if(p.magic!=0x464D3244||p.version!=1||p.size!=sizeof(p)||p.role!=0||localRole!=1||p.session!=remote.session||p.sequence<=m.peerSequence||!std::memchr(p.mapinfo,0,sizeof(p.mapinfo))||
       std::strncmp(p.mapinfo,"campaign/",9)||std::strpbrk(p.mapinfo," ;\"")){++m.refused;return;}
    m.peerSequence=p.sequence;std::snprintf(m.hostMapinfo,sizeof(m.hostMapinfo),"%s",p.mapinfo);m.hostHigh=p.highChaos?1u:0u;m.hostMap=p.mapId;
}
void serviceMissionFollow(void* system,ULONGLONG tick){
    if(!productMode&&!followHostEnabled)return;
    auto& m=missionFollow;
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000;
    if(!connected)return;
    if(localRole==0){
        if(tick-m.sentAt<1000)return;m.sentAt=tick;
        MissionFollowPacket p{};std::uint32_t high=0;if(!readCurrentMapinfo(p.mapinfo,sizeof(p.mapinfo),high)||outgoing.gameLogicState!=1)return;
        p.highChaos=high;p.session=outgoing.session;p.mapId=outgoing.mapId;p.sequence=++m.sequence;
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){++m.sent;m.result="publishing";}
        return;
    }
    // Joiner: follow the host's mission once per host map, from ordinary
    // gameplay or a menu; never while loading or in a cinematic.
    if(!*m.hostMapinfo||!m.hostMap){m.result="waiting_host_mission";return;}
    if(outgoing.mapId==m.hostMap){m.result="same_mission";m.loadedForMap=m.hostMap;return;}
    if(m.loadedForMap==m.hostMap&&tick-m.loadAt<60000){m.result="loading_host_mission";return;}
    const unsigned logic=outgoing.gameLogicState;
    if(outgoing.pendingGameLogicState||(logic!=1&&logic!=2&&logic!=3)){m.result="waiting_native_phase";return;}
    char command[220]{};std::snprintf(command,sizeof(command),"campaign %s %s emily",m.hostMapinfo,m.hostHigh?"high":"low");
    dispatchCoopCommand(system,command);++m.loads;m.loadAt=tick;m.loadedForMap=m.hostMap;m.result="loading_host_mission";
}
void writeMissionFollow(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.follow.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& m=missionFollow;
    fprintf(f,"{\"enabled\":%s,\"result\":\"%s\",\"hostMapinfo\":\"%s\",\"hostHigh\":%u,\"hostMap\":\"%llu\",\"localMap\":\"%llu\",\"sent\":%llu,\"received\":%llu,\"loads\":%llu,\"refused\":%llu,\"role\":%u}\n",
        (productMode||followHostEnabled)?"true":"false",m.result,m.hostMapinfo,m.hostHigh,m.hostMap,outgoing.mapId,m.sent,m.received,m.loads,m.refused,localRole);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

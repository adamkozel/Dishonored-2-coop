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
#include "product_arm.h"
ProductArm productArm{};
// 1214cp (#104): power world visuals that were scoped to the isolated lab fixture also run while the product
// controller holds an armed session (installed release or lab coop_product_auto).
bool frontEndTitlePassed=false; // 1214ha (#146): a key was pressed at the title screen since boot
bool coopSavesReadyForSession();void requestCoopSaves(); // 1214hs (#156, native_menu_ui.h)
int coopPartnerCharacterForJoin(ULONGLONG tick,std::uint64_t joinKey); // 1214hx (#158, coop_character.h)
bool coopCharacterChoiceEnabled=false; // 1214hx: the lab toggle coop_character_choice on|off (product mode always chooses)
bool productSessionArmed(){return productArm.result&&!std::strcmp(productArm.result,"armed");}
SHORT productKeyF7=0,productKeyF8=0;
const char* productStep(unsigned i){
    switch(i){
    case 0:return "coop_cinematic_sync on";
    case 1:return "coop_cinematic_barrier on";
    case 2:return "coop_world_observe on";
    case 3:return partnerAssetsEnabled?(remoteIsCorvo()?"coop_spawn_probe shadow_corvo":"coop_spawn_probe shadow_emily"):(remoteIsCorvo()?"coop_spawn_probe corvo":"coop_spawn_probe emily");
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
    case 19:return "coop_gameflow_mirror on"; // 1214bg: objectives follow the host
    case 20:return "coop_kiscule_mirror on"; // 1214bi: story grants (Mark, powers, loadouts) in both worlds
    case 21:return "coop_breakable_sync on"; // 1214fq (#132): broken glass and barriers in both worlds
    case 22:return "coop_prop_rest on"; // 1214ip (#178): props come to rest in the same place in both worlds
    case 23:return "coop_use_mirror on"; // 1214jn (#187): buttons and levers outside the shared toggles in both worlds
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
        if((f7&0x8000)&&!(productKeyF7&0x8000)){++a.hostKeys;requestCoopSaves();startOnlineHost();} // 1214hs: co-op saves
        if((f8&0x8000)&&!(productKeyF8&0x8000)){++a.leaveKeys;stopOnlineSession("left_by_player");a={};}
        productKeyF7=f7;productKeyF8=f8;
    }
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&lastReceive&&tick-lastReceive<1000;
    if(!connected||!coopWorldPairReady(outgoing,remote)||outgoing.mapId!=remote.mapId){a.result=connected?"waiting_shared_gameplay":"waiting_partner";return;}
    if(a.epoch!=outgoing.worldEpoch||a.peerEpoch!=remote.worldEpoch||a.map!=outgoing.mapId){
        productNewWorld(a,outgoing.worldEpoch,remote.worldEpoch,outgoing.mapId,tick);return; // 1214ga (#145, product_arm.h)
    }
    if(tick<a.nextAt)return;
    if(a.step==3){static ULONGLONG characterWaitSince=0; // 1214hy (#159)
        if(remoteCharacterKnown())characterWaitSince=0;else{if(!characterWaitSince)characterWaitSince=tick;if(tick-characterWaitSince<3000){a.result="waiting_partner_character";return;}}}
    const char* command=productStep(a.step);
    if(!command){
        // Maintenance: re-issue idempotent session commands every 5 s so a
        // system refused transiently (e.g. a checkpoint before the mission
        // autosave lands, or a session that ended) is armed again.
        static const char* maintain[]={"coop_checkpoint_sync on","coop_door_sync session","coop_usable_sync session","coop_coin_sync on","coop_npc_stream on","coop_trigger_mirror on","coop_movement_resume on","coop_breakable_sync on","coop_prop_rest on","coop_use_mirror on"};
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
// 1214hs (#156, native_menu_ui.h): a session never reads or writes the player's own saves. In product mode the partner's
// game switches to co-op saves (savegame_coop and its profile) before it follows the host's mission (the native
// "campaign" load would otherwise start a campaign in her own slots), and F7 puts the host in co-op mode too.
#include "join_warp.h"
struct MissionFollow {
    char hostMapinfo[160]{};std::uint32_t hostHigh=0;std::uint64_t hostMap=0,sequence=0,peerSequence=0,sent=0,received=0,loads=0,refused=0;
    // 1214fr (#135, join_warp.h): the map this partner loaded by following, the map it was placed on, and when both
    // worlds were last seen in gameplay together.
    std::uint64_t followedMap=0,warpedMap=0,warps=0;ULONGLONG togetherSince=0;float warpDistance=0;unsigned warpedConnection=0;
    ULONGLONG sentAt=0,loadAt=0;std::uint64_t loadedForMap=0;const char* result="idle";
    ULONGLONG receivedAt=0; // 1214ju (#174): when the host's last publication arrived
};
MissionFollow missionFollow{};
// 1214bu: idCommonLocal+0x2b8 (the idDeclMapInfo) is null during a mission
// started with the native `campaign` command (held take route-institute-v1303am:
// null in both worlds, so the mission end was never sent and D2MF published
// nothing). Without it, the mapinfo is derived from the loaded map's name: the
// map's directory plus its name without _p, _p_lowchaos or _p_highchaos, which
// names an existing mapinfo for all 35 campaign maps (confirmed static). The last
// identity read in this world covers moments without a loaded map name (a movie).
char lastMissionMapinfo[160]{};std::uint32_t lastMissionHigh=0;
bool mapinfoFromMapName(const char* map,char* out,std::size_t size){
    if(std::strncmp(map,"campaign/",9)||!std::strncmp(map,"campaign/menu/",14))return false;
    const char* slash=std::strrchr(map,'/');if(!slash||!slash[1])return false;
    std::string leaf(slash+1);
    for(const char* suffix:{"_p_lowchaos","_p_highchaos","_p"}){const std::size_t n=std::strlen(suffix);
        if(leaf.size()>n&&!leaf.compare(leaf.size()-n,n,suffix)){leaf.resize(leaf.size()-n);
            const int w=std::snprintf(out,size,"%.*s/%s.mapinfo",static_cast<int>(slash-map),map,leaf.c_str());return w>0&&static_cast<std::size_t>(w)<size;}}
    return false;
}
bool readCurrentMapinfo(char* out,std::size_t size,std::uint32_t& high){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t common=0,decl=0,name=0,vt=0;std::string text;
    const bool fromDecl=memoryRead(base+0x2274420,&common,8)&&common&&memoryRead(common,&vt,8)&&vt==base+0x1ac4d88&&memoryRead(common+0x2b8,&decl,8)&&decl&&
        memoryRead(decl+8,&name,8)&&worldIdentityString(memoryRead,name,text)&&text.size()<size&&text.rfind("campaign/",0)==0;
    char derived[160]{};
    if(fromDecl)std::snprintf(out,size,"%s",text.c_str());
    else if(mapinfoFromMapName(mapName,derived,sizeof(derived))&&std::strlen(derived)<size)std::snprintf(out,size,"%s",derived);
    else if(*lastMissionMapinfo&&std::strlen(lastMissionMapinfo)<size){std::snprintf(out,size,"%s",lastMissionMapinfo);high=lastMissionHigh;return true;}
    else return false;
    high=std::strstr(mapName,"highchaos")?1u:0u;
    std::snprintf(lastMissionMapinfo,sizeof(lastMissionMapinfo),"%s",out);lastMissionHigh=high;return true;
}
void receiveMissionFollow(const MissionFollowPacket& p){
    auto& m=missionFollow;++m.received;
    if(p.magic!=0x464D3244||p.version!=1||p.size!=sizeof(p)||p.role!=0||localRole!=1||p.session!=remote.session||p.sequence<=m.peerSequence||!std::memchr(p.mapinfo,0,sizeof(p.mapinfo))||
       std::strncmp(p.mapinfo,"campaign/",9)||std::strpbrk(p.mapinfo," ;\"")){++m.refused;return;}
    m.peerSequence=p.sequence;std::snprintf(m.hostMapinfo,sizeof(m.hostMapinfo),"%s",p.mapinfo);m.hostHigh=p.highChaos?1u:0u;m.hostMap=p.mapId;
    m.receivedAt=GetTickCount64();
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
    if(outgoing.mapId==m.hostMap){m.result="same_mission";m.loadedForMap=m.hostMap;
        if(joinWarpDue(m.followedMap,outgoing.mapId,m.warpedMap,m.warpedConnection,static_cast<unsigned>(connection))){ // 1214ga (#135): again after a rejoin
            const bool together=outgoing.gameLogicState==1&&!outgoing.pendingGameLogicState&&remote.gameLogicState==1&&!remote.pendingGameLogicState&&
                remote.mapId==outgoing.mapId&&tick-lastReceive<500;
            if(!together){m.togetherSince=0;return;}
            if(!m.togetherSince){m.togetherSince=tick;return;}
            if(tick-m.togetherSince<kJoinWarpSettleMs)return;
            const auto w=joinWarpTarget(outgoing.position,remote.position,remote.orientation);m.warpDistance=w.distance;m.warpedMap=outgoing.mapId;m.warpedConnection=static_cast<unsigned>(connection);
            if(!w.warp){m.result="same_mission_near_host";return;}
            char command[160]{};std::snprintf(command,sizeof(command),"setviewpos %.3f %.3f %.3f %.2f 0",w.x,w.y,w.z,w.yaw);
            dispatchCoopCommand(system,command);++m.warps;m.result="placed_beside_host";}
        return;}
    if(m.loadedForMap==m.hostMap&&tick-m.loadAt<60000){m.result="loading_host_mission";return;}
    // 1214ju (#174): only a fresh publication starts a load. The host publishes its mission every second in gameplay and
    // nothing from the main menu, so a partner whose tutorial had ended on the menu with the host's reloaded the mission
    // the host had just left (route-training-join-v1882a).
    if(!m.receivedAt||tick-m.receivedAt>3000){m.result="waiting_host_mission";return;}
    const unsigned logic=outgoing.gameLogicState;
    // 1214cf: also from the front-end main menu (campaign/menu/menu_p, logic 6), where a partner waits for the
    // invite; the 0.7.0 clean-PC take 1314a stayed there (join-follow had only covered gameplay and pause menus).
    const bool frontEnd=logic==6&&!std::strcmp(mapName,"campaign/menu/menu_p");
    if(outgoing.pendingGameLogicState||(logic!=1&&logic!=2&&logic!=3&&!frontEnd)){m.result="waiting_native_phase";return;}
    // 1214ha (#146): never from the title screen. Its game has no signed-in user until a key is pressed there, and the
    // followed mission could not save ("No connected user", product-two-account-v1696a/v1699a). It waits, showing the
    // title, until the player presses a key.
    if(frontEnd&&!frontEndTitlePassed){m.result="waiting_title_key";return;}
    if(productMode&&!coopSavesReadyForSession()){requestCoopSaves();m.result="waiting_coop_saves";return;} // 1214hs
    // 1214hx (#158): the partner chooses Corvo or Emily on every join (coop_character.h); lab default stays Emily.
    // The first mission has the game's own choice screen (its script switches the character), so it asks no question.
    int character=2;const bool nativeChoice=!std::strcmp(m.hostMapinfo,"campaign/dunwall/escape/tower/dunwall_escape_tower.mapinfo");
    if((productMode||coopCharacterChoiceEnabled)&&!nativeChoice){character=coopPartnerCharacterForJoin(tick,static_cast<std::uint64_t>(connection));if(!character){m.result="choosing_character";return;}}
    char command[220]{};std::snprintf(command,sizeof(command),"campaign %s %s %s",m.hostMapinfo,m.hostHigh?"high":"low",character==1?"corvo":"emily");
    dispatchCoopCommand(system,command);++m.loads;m.loadAt=tick;m.loadedForMap=m.hostMap;m.followedMap=m.hostMap;m.result="loading_host_mission";
}
void writeMissionFollow(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.follow.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& m=missionFollow;
    fprintf(f,"{\"enabled\":%s,\"result\":\"%s\",\"hostMapinfo\":\"%s\",\"hostHigh\":%u,\"hostMap\":\"%llu\",\"localMap\":\"%llu\",\"sent\":%llu,\"received\":%llu,\"loads\":%llu,\"refused\":%llu,\"role\":%u,\"warps\":%llu,\"warpDistance\":%.2f}\n",
        (productMode||followHostEnabled)?"true":"false",m.result,m.hostMapinfo,m.hostHigh,m.hostMap,outgoing.mapId,m.sent,m.received,m.loads,m.refused,localRole,m.warps,m.warpDistance);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

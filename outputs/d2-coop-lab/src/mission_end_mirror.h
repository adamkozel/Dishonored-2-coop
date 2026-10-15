#pragma once
// 1214ax stage 6 mission end (chapters 89 and 97, audit 1291). A gameflow
// mission node reaching state 3 (complete) with its result screen enabled
// switches the game to logic 11 (Results); Continue then loads the mission's
// m_nextMissionMap (confirmed runtime, take mission-end-carriage-v1291a).
// When this world enters Results with its own mission complete, it tells the
// partner. A partner on the same mapinfo, in ordinary gameplay, completes its
// OWN active mission node (state 2) through the native gameflow set-state
// routine (0x1404e9bd0: manager game+0x1f7780, &node id, state; confirmed
// static). Its world then shows its own result screen and loads the next
// mission natively with its own character state. Node ids never cross
// processes (they are localization string ids): each world finds its own.
// 1214ay adds the both-ready hold at the next map's load end (below); 1214az
// keeps the window responsive while it holds; 1214ba makes chaos follow the
// host (below).
#pragma pack(push,1)
struct MissionEndPacket {
    std::uint32_t magic=0x454D3244; // "D2ME"
    std::uint16_t version=1,size=sizeof(MissionEndPacket);
    std::uint32_t role=0,highChaos=0;
    std::uint64_t session=0,epoch=0,mapId=0,sequence=0;
    char mapinfo[160]{};
};
#pragma pack(pop)
static_assert(sizeof(MissionEndPacket)==208);
inline bool validMissionEnd(const MissionEndPacket& p,unsigned receiver){
    return p.magic==0x454D3244&&p.version==1&&p.size==sizeof(p)&&p.role<=1&&p.role!=receiver&&p.highChaos<=1&&
        p.session&&p.epoch&&p.mapId&&p.sequence&&std::memchr(p.mapinfo,0,sizeof(p.mapinfo))&&
        !std::strncmp(p.mapinfo,"campaign/",9)&&!std::strpbrk(p.mapinfo," ;\"");
}
const unsigned char gameflowSetStatePrefix[16]={0x40,0x57,0x48,0x83,0xec,0x20,0x4c,0x63,0x0a,0x48,0x63,0x51,0x28,0x48,0x8b,0x41};
using NativeGameflowSetState=void(*)(void*,const std::uint32_t*,int);
using NativeGameflowType=std::uintptr_t(*)(void*);
struct MissionNodeScan {std::uintptr_t node=0;std::uint32_t id=0;int state=-1;unsigned missions=0;};
// This world's mission nodes in the gameflow manager (game+0x1f7780): bucket
// array at +0x18, mask at +0x28; chain entries {key, node at +8, next at
// +0x10}; node id at +8, state at +0xc. A node is a mission when its type's
// class index (+0x60) lies in [+0x39e3240, +0x39e3244], the same test the
// native set-state routine makes.
bool scanMissionNodes(int wantState,MissionNodeScan& out){
    out={};const auto base=reinterpret_cast<std::uintptr_t>(gameImage);if(!observedGame)return false;
    const auto manager=observedGame+0x1f7780;std::uintptr_t buckets=0;std::uint32_t mask=0;int low=0,high=0;
    if(!memoryRead(manager+0x18,&buckets,8)||!buckets||!memoryRead(manager+0x28,&mask,4)||mask>0xffff||(mask&(mask+1))||
       !memoryRead(base+0x39e3240,&low,4)||!memoryRead(base+0x39e3244,&high,4)||low>high)return false;
    for(std::uint32_t b=0;b<=mask;++b){
        std::uintptr_t entry=0;if(!memoryRead(buckets+b*8ull,&entry,8))return false;
        for(unsigned hops=0;entry&&hops<256;++hops){
            std::uintptr_t node=0,next=0,vt=0,getter=0;
            if(!memoryRead(entry+8,&node,8)||!memoryRead(entry+0x10,&next,8))return false;
            if(node&&memoryRead(node,&vt,8)&&vt>base&&vt<base+0x4000000&&memoryRead(vt,&getter,8)&&getter>base&&getter<base+0x2000000){
                const auto type=reinterpret_cast<NativeGameflowType>(getter)(reinterpret_cast<void*>(node));int cls=0;
                if(type&&memoryRead(type+0x60,&cls,4)&&cls>=low&&cls<=high){
                    ++out.missions;std::uint32_t id=0;int state=-1;
                    if(memoryRead(node+8,&id,4)&&memoryRead(node+0xc,&state,4)&&state==wantState&&!out.node){out.node=node;out.id=id;out.state=state;}
                }
            }
            entry=next;
        }
    }
    return true;
}
// 1214bf: counts of all gameflow nodes by state (0..6) in this world, so
// campaign takes can check that both worlds' objectives advance together.
bool countGameflowStates(unsigned counts[7]){
    for(unsigned i=0;i<7;++i)counts[i]=0;if(!observedGame)return false;
    const auto manager=observedGame+0x1f7780;std::uintptr_t buckets=0;std::uint32_t mask=0;
    if(!memoryRead(manager+0x18,&buckets,8)||!buckets||!memoryRead(manager+0x28,&mask,4)||mask>0xffff||(mask&(mask+1)))return false;
    for(std::uint32_t b=0;b<=mask;++b){std::uintptr_t entry=0;if(!memoryRead(buckets+b*8ull,&entry,8))return false;
        for(unsigned hops=0;entry&&hops<256;++hops){std::uintptr_t node=0,next=0;int state=-1;
            if(!memoryRead(entry+8,&node,8)||!memoryRead(entry+0x10,&next,8))return false;
            if(node&&memoryRead(node+0xc,&state,4)&&state>=0&&state<7)++counts[state];entry=next;}}
    return true;
}
bool missionEndMirrorEnabled=false;
struct MissionEndMirror {
    std::uint64_t sequence=0,peerSequence=0,sent=0,received=0,applied=0,refused=0,noActive=0,notSameMission=0,scanFailed=0,sentEpoch=0,appliedEpoch=0,pendingAt=0;
    bool pending=false;unsigned lastLogic=0,missions=0;int appliedState=-1;MissionEndPacket incoming{};const char* result="idle";
};
MissionEndMirror missionEnd{};
// 1214ay: both-ready hold at the next map's load end after a mirrored mission
// end. Each world, at its native FinishLoad switch out of logic 15 (caller
// 0x7879cd, phase 3), sends D2MR with the loaded map name and waits (reading
// state and pumping transport only, as the checkpoint and loading holds do)
// until the partner reports the same map, for at most 90 s, or until the
// connection drops. The native call stack stays intact while it waits.
#pragma pack(push,1)
struct MapReadyPacket {
    std::uint32_t magic=0x524D3244; // "D2MR"
    std::uint16_t version=1,size=sizeof(MapReadyPacket);
    std::uint32_t role=0,reserved=0;
    std::uint64_t session=0,sequence=0;
    char map[160]{};
};
#pragma pack(pop)
static_assert(sizeof(MapReadyPacket)==192);
struct MissionTransitionHold {
    bool expect=false,held=false;
    std::uint64_t expectAt=0,sequence=0,peerSequence=0,holds=0,released=0,timeouts=0,aborted=0,sent=0,received=0,refused=0,startedAt=0,releasedAt=0,lastWaitMs=0;
    unsigned caller=0,from=0,to=0,pending=0;char localMap[160]{},peerMap[160]{};const char* result="idle";
    bool ownsWindow=false;std::uint64_t peeks=0;
};
MissionTransitionHold transitionHold{};
void expectMissionTransition(std::uint64_t tick){auto& h=transitionHold;h.expect=true;h.expectAt=tick;*h.peerMap=0;*h.localMap=0;h.result="expecting";}
void receiveMapReady(const MapReadyPacket& p){
    auto& h=transitionHold;++h.received;
    if(p.magic!=0x524D3244||p.version!=1||p.size!=sizeof(p)||p.role>1||p.role==localRole||p.session!=remote.session||p.sequence<=h.peerSequence||
       !std::memchr(p.map,0,sizeof(p.map))||std::strncmp(p.map,"campaign/",9)){++h.refused;return;}
    h.peerSequence=p.sequence;std::snprintf(h.peerMap,sizeof(h.peerMap),"%s",p.map);
}
// 1214be: the ready is re-sent every second while holding and for 30 s after
// release. Across machines a single send could arrive before the partner
// knew this world's new session and be refused, so the partner held to its
// timeout (take mission-end-vm-v1296c).
std::uint64_t mapReadySentAt=0;
void sendMapReady(std::uint64_t tick){
    auto& h=transitionHold;if(!*h.localMap)return;
    MapReadyPacket p{};p.role=localRole;p.session=outgoing.session;p.sequence=++h.sequence;std::snprintf(p.map,sizeof(p.map),"%s",h.localMap);
    if(sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&
       coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK)++h.sent;
    mapReadySentAt=tick;
}
bool missionTransitionBoundary(const NativeSwitchEvent& e){
    auto& h=transitionHold;if(!h.expect||h.held)return false;
    if(e.tick-h.expectAt>600000){h.expect=false;h.result="expect_expired";return false;}
    if(e.thread!=engineThread.load()||e.caller!=0x7879cd||e.from!=15||e.phase!=3||!e.game)return false;
    std::uintptr_t name=0;char map[192]{};
    if(!memoryRead(e.game+0x337138,&name,8)||!nativeName(name,map,sizeof(map))||std::strncmp(map,"campaign/",9)||std::strlen(map)>=sizeof(h.localMap))return false;
    h.expect=false;h.held=true;h.startedAt=e.tick;h.releasedAt=0;++h.holds;
    h.caller=static_cast<unsigned>(e.caller);h.from=e.from;h.to=static_cast<unsigned>(e.to);h.pending=e.pending;
    std::snprintf(h.localMap,sizeof(h.localMap),"%s",map);
    sendMapReady(e.tick);
    h.result="holding";networkFrameOwned.store(false,std::memory_order_release);
    return true;
}
// Windows flags a window "Not Responding" when its thread has not checked its
// queue for 5 s (take 1291c: a 21 s hold). When the holding thread owns a
// visible game window it peeks without removing every 500 ms; input stays
// queued for the game.
bool holdThreadOwnsWindow(){
    struct Ctx{bool found=false;} c;
    EnumThreadWindows(GetCurrentThreadId(),[](HWND w,LPARAM l)->BOOL{if(IsWindowVisible(w)){reinterpret_cast<Ctx*>(l)->found=true;return FALSE;}return TRUE;},reinterpret_cast<LPARAM>(&c));
    return c.found;
}
void waitMissionTransition(){
    auto& h=transitionHold;const bool previousMutation=nativeMutationAllowed;nativeMutationAllowed=false;
    h.ownsWindow=holdThreadOwnsWindow();std::uint64_t lastPeek=0;
    for(;;){
        if(*h.peerMap&&!std::strcmp(h.peerMap,h.localMap)){h.result="released";break;}
        const auto tick=GetTickCount64();
        if(tick-h.startedAt>90000){++h.timeouts;h.result=*h.peerMap?"timeout_other_map":"timeout";break;}
        if(!sockets||!connection||connectionState!=k_ESteamNetworkingConnectionState_Connected){++h.aborted;h.result="connection_lost";break;}
        if(h.ownsWindow&&tick-lastPeek>=500){MSG msg{};PeekMessageW(&msg,nullptr,0,0,PM_NOREMOVE);lastPeek=tick;++h.peeks;}
        if(tick-mapReadySentAt>=1000)sendMapReady(tick);
        pumpTransport(tick);writeStatus(tick);Sleep(10);
    }
    h.held=false;h.releasedAt=GetTickCount64();h.lastWaitMs=h.releasedAt-h.startedAt;++h.released;
    nativeMutationAllowed=previousMutation;
}
void receiveMissionEnd(const MissionEndPacket& p){
    auto& m=missionEnd;++m.received;
    if(!(missionEndMirrorEnabled||productMode)||!validMissionEnd(p,localRole)||p.session!=remote.session||p.sequence<=m.peerSequence){++m.refused;m.result="refused";return;}
    m.peerSequence=p.sequence;m.incoming=p;m.pending=true;m.pendingAt=GetTickCount64();m.result="received";
}
void serviceMissionEndMirror(std::uint64_t tick){
    if(GetCurrentThreadId()!=engineThread.load())return;
    auto& m=missionEnd;if(!(missionEndMirrorEnabled||productMode)){m.pending=false;m.lastLogic=0;return;}
    const unsigned logic=outgoing.gameLogicState;
    // Sender: this world has just entered its own result screen.
    if(logic==11&&m.lastLogic!=11&&m.sentEpoch!=outgoing.worldEpoch){
        MissionNodeScan s{};MissionEndPacket p{};std::uint32_t high=0;
        if(!scanMissionNodes(3,s)){++m.scanFailed;m.result="scan_failed";}
        else if(!s.node){m.result="results_without_mission";}
        else if(readCurrentMapinfo(p.mapinfo,sizeof(p.mapinfo),high)&&sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected){
            p.role=localRole;p.highChaos=high;p.session=outgoing.session;p.epoch=outgoing.worldEpoch;p.mapId=outgoing.mapId;p.sequence=++m.sequence;
            if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){++m.sent;m.sentEpoch=outgoing.worldEpoch;m.result="sent";expectMissionTransition(tick);}
        }
    }
    m.lastLogic=logic;
    {const auto& h=transitionHold;if(!h.held&&h.releasedAt&&*h.localMap&&tick-h.releasedAt<30000&&tick-mapReadySentAt>=1000)sendMapReady(tick);}
    if(!m.pending)return;
    if(tick-m.pendingAt>120000){m.pending=false;m.result="expired";return;}
    if(m.appliedEpoch==outgoing.worldEpoch){m.pending=false;m.result="already_applied";return;}
    if(logic==11||logic==15){m.pending=false;m.result="local_already_ending";return;}
    // Only from ordinary gameplay: the native logic switch to Results can
    // refuse from other states. Scenes, prompts and menus wait.
    if(logic!=1||outgoing.pendingGameLogicState){m.result="waiting_native_phase";return;}
    char here[160]{};std::uint32_t high=0;
    if(!readCurrentMapinfo(here,sizeof(here),high)||std::strcmp(here,m.incoming.mapinfo)){++m.notSameMission;m.pending=false;m.result="not_same_mission";return;}
    MissionNodeScan s{};if(!scanMissionNodes(2,s)){++m.scanFailed;m.result="scan_failed";return;}
    m.missions=s.missions;
    if(!s.node){++m.noActive;m.pending=false;m.result="no_active_mission";return;}
    if(std::memcmp(gameImage+0x4e9bd0,gameflowSetStatePrefix,sizeof(gameflowSetStatePrefix))){m.pending=false;m.result="code_mismatch";return;}
    const std::uint32_t id=s.id;
    reinterpret_cast<NativeGameflowSetState>(gameImage+0x4e9bd0)(reinterpret_cast<void*>(observedGame+0x1f7780),&id,3);
    int after=-1;memoryRead(s.node+0xc,&after,4);m.appliedState=after;
    // This world's own Results must not echo back to the sender.
    m.appliedEpoch=outgoing.worldEpoch;m.sentEpoch=outgoing.worldEpoch;
    ++m.applied;m.pending=false;m.result="applied";expectMissionTransition(tick);
}
// 1214ba: chaos follows the host, as the world's authority. Each world picks
// the next map's low/high chaos variant from its own chaos level, computed at
// mission completion from the player's arkCpntPlayerChaos points (+0x24;
// level at +0x20; vtable +0x1b63248, confirmed runtime) against the mission's
// thresholds (0x1404ea390, confirmed static). While both peers are on the
// same map, the host publishes its points about once a second and the
// partner keeps its own equal, so either player's mission end computes the
// same level and loads the same variant.
#pragma pack(push,1)
struct ChaosSyncPacket {
    std::uint32_t magic=0x48433244; // "D2CH"
    std::uint16_t version=1,size=sizeof(ChaosSyncPacket);
    std::uint32_t role=0;std::int32_t points=0,level=0,reserved=0;
    std::uint64_t session=0,mapId=0,sequence=0;
};
#pragma pack(pop)
static_assert(sizeof(ChaosSyncPacket)==48);
struct ChaosSync {
    std::uint64_t sequence=0,peerSequence=0,sent=0,received=0,refused=0,writes=0,missing=0,sentAt=0,hostMap=0;
    std::int32_t hostPoints=-1,hostLevel=-1,localPoints=-1,localLevel=-1,before=-1;bool pending=false;const char* result="idle";
};
ChaosSync chaosSync{};
std::uintptr_t findPlayerChaos(){
    const auto base=reinterpret_cast<std::uintptr_t>(gameImage);std::uintptr_t list=0;unsigned count=0;
    if(!observedPlayer||!memoryRead(observedPlayer+0x268,&list,8)||!list||!memoryRead(observedPlayer+0x270,&count,4)||!(count&0xffffff)||(count&0xffffff)>256)return 0;
    for(unsigned i=0;i<(count&0xffffff);++i){std::uintptr_t c=0,vt=0,owner=0;
        if(memoryRead(list+i*8,&c,8)&&c&&memoryRead(c,&vt,8)&&vt==base+0x1b63248&&memoryRead(c+0x10,&owner,8)&&owner==observedPlayer)return c;}
    return 0;
}
// Lab only (refused in product mode): set this world's chaos points, so a
// take can make the two worlds' chaos differ.
std::uint64_t labChaosWrites=0,labChaosRefused=0;
void labSetChaosPoints(const char* text){
    char* end=nullptr;const long v=std::strtol(text,&end,10);const auto component=findPlayerChaos();
    if(productMode||!end||*end||v<0||v>100000||!component||GetCurrentThreadId()!=engineThread.load()){++labChaosRefused;return;}
    *reinterpret_cast<std::int32_t*>(component+0x24)=static_cast<std::int32_t>(v);++labChaosWrites;
}
void receiveChaosSync(const ChaosSyncPacket& p){
    auto& c=chaosSync;++c.received;
    if(p.magic!=0x48433244||p.version!=1||p.size!=sizeof(p)||p.role!=0||localRole!=1||p.session!=remote.session||p.sequence<=c.peerSequence||
       p.points<0||p.points>1000000||p.level<0||p.level>2||!p.mapId){++c.refused;return;}
    c.peerSequence=p.sequence;c.hostPoints=p.points;c.hostLevel=p.level;c.hostMap=p.mapId;c.pending=true;
}
void serviceChaosSync(std::uint64_t tick){
    if(GetCurrentThreadId()!=engineThread.load())return;
    auto& c=chaosSync;if(!(missionEndMirrorEnabled||productMode)){c.pending=false;return;}
    const auto component=findPlayerChaos();
    if(component){memoryRead(component+0x20,&c.localLevel,4);memoryRead(component+0x24,&c.localPoints,4);}
    const bool connected=sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected;
    if(localRole==0){
        if(!connected||!component||tick-c.sentAt<1000||!coopGameplayReady(outgoing))return;
        ChaosSyncPacket p{};p.role=0;p.points=c.localPoints;p.level=c.localLevel;p.session=outgoing.session;p.mapId=outgoing.mapId;p.sequence=++c.sequence;
        if(coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_UnreliableNoNagle,nullptr)==k_EResultOK){++c.sent;c.sentAt=tick;c.result="publishing";}
        return;
    }
    if(!c.pending)return;
    if(c.hostMap!=outgoing.mapId){c.result="other_map";return;}
    // Ordinary gameplay or a scene only: never during loads or result screens.
    const unsigned logic=outgoing.gameLogicState;
    if(outgoing.pendingGameLogicState||(logic!=1&&logic!=5)){c.result="waiting_native_phase";return;}
    if(!component){++c.missing;c.result="component_missing";return;}
    c.pending=false;
    if(c.localPoints==c.hostPoints){c.result="equal";return;}
    c.before=c.localPoints;*reinterpret_cast<std::int32_t*>(component+0x24)=c.hostPoints;c.localPoints=c.hostPoints;++c.writes;c.result="matched_host";
}
void writeMissionEndMirror(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.missionend.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    unsigned states[7]{};const bool statesOk=GetCurrentThreadId()==engineThread.load()&&countGameflowStates(states);
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& m=missionEnd;
    if(statesOk)fprintf(f,"{\"gameflowStates\":[%u,%u,%u,%u,%u,%u,%u],",states[0],states[1],states[2],states[3],states[4],states[5],states[6]);else fprintf(f,"{\"gameflowStates\":null,");
    fprintf(f,"\"enabled\":%s,\"result\":\"%s\",\"role\":%u,\"sent\":%llu,\"received\":%llu,\"applied\":%llu,\"refused\":%llu,\"noActive\":%llu,\"notSameMission\":%llu,\"scanFailed\":%llu,\"missions\":%u,\"appliedState\":%d,\"pending\":%s,\"peerMapinfo\":\"%s\",\"peerHigh\":%u,\"checkpointContinues\":%llu,\"hold\":{\"result\":\"%s\",\"expect\":%s,\"held\":%s,\"holds\":%llu,\"released\":%llu,\"timeouts\":%llu,\"aborted\":%llu,\"sent\":%llu,\"received\":%llu,\"refused\":%llu,\"lastWaitMs\":%llu,\"caller\":\"0x%x\",\"from\":%u,\"to\":%u,\"pending\":%u,\"localMap\":\"%s\",\"peerMap\":\"%s\",\"ownsWindow\":%s,\"peeks\":%llu},\"chaos\":{\"result\":\"%s\",\"sent\":%llu,\"received\":%llu,\"refused\":%llu,\"writes\":%llu,\"missing\":%llu,\"hostPoints\":%d,\"hostLevel\":%d,\"localPoints\":%d,\"localLevel\":%d,\"before\":%d,\"labWrites\":%llu,\"labRefused\":%llu}}\n",
        (missionEndMirrorEnabled||productMode)?"true":"false",m.result,localRole,m.sent,m.received,m.applied,m.refused,m.noActive,m.notSameMission,m.scanFailed,m.missions,m.appliedState,m.pending?"true":"false",m.incoming.mapinfo,m.incoming.highChaos,checkpointMissionContinues,
        transitionHold.result,transitionHold.expect?"true":"false",transitionHold.held?"true":"false",transitionHold.holds,transitionHold.released,transitionHold.timeouts,transitionHold.aborted,transitionHold.sent,transitionHold.received,transitionHold.refused,transitionHold.lastWaitMs,transitionHold.caller,transitionHold.from,transitionHold.to,transitionHold.pending,transitionHold.localMap,transitionHold.peerMap,transitionHold.ownsWindow?"true":"false",transitionHold.peeks,
        chaosSync.result,chaosSync.sent,chaosSync.received,chaosSync.refused,chaosSync.writes,chaosSync.missing,chaosSync.hostPoints,chaosSync.hostLevel,chaosSync.localPoints,chaosSync.localLevel,chaosSync.before,labChaosWrites,labChaosRefused);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

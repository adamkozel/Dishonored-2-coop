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
// keeps the window responsive while it holds.
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
bool missionTransitionBoundary(const NativeSwitchEvent& e){
    auto& h=transitionHold;if(!h.expect||h.held)return false;
    if(e.tick-h.expectAt>600000){h.expect=false;h.result="expect_expired";return false;}
    if(e.thread!=engineThread.load()||e.caller!=0x7879cd||e.from!=15||e.phase!=3||!e.game)return false;
    std::uintptr_t name=0;char map[192]{};
    if(!memoryRead(e.game+0x337138,&name,8)||!nativeName(name,map,sizeof(map))||std::strncmp(map,"campaign/",9)||std::strlen(map)>=sizeof(h.localMap))return false;
    h.expect=false;h.held=true;h.startedAt=e.tick;h.releasedAt=0;++h.holds;
    h.caller=static_cast<unsigned>(e.caller);h.from=e.from;h.to=static_cast<unsigned>(e.to);h.pending=e.pending;
    std::snprintf(h.localMap,sizeof(h.localMap),"%s",map);
    MapReadyPacket p{};p.role=localRole;p.session=outgoing.session;p.sequence=++h.sequence;std::snprintf(p.map,sizeof(p.map),"%s",map);
    if(sockets&&connection&&connectionState==k_ESteamNetworkingConnectionState_Connected&&
       coopSendMessage(connection,&p,sizeof(p),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK)++h.sent;
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
void writeMissionEndMirror(){
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.missionend.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& m=missionEnd;
    fprintf(f,"{\"enabled\":%s,\"result\":\"%s\",\"role\":%u,\"sent\":%llu,\"received\":%llu,\"applied\":%llu,\"refused\":%llu,\"noActive\":%llu,\"notSameMission\":%llu,\"scanFailed\":%llu,\"missions\":%u,\"appliedState\":%d,\"pending\":%s,\"peerMapinfo\":\"%s\",\"peerHigh\":%u,\"hold\":{\"result\":\"%s\",\"expect\":%s,\"held\":%s,\"holds\":%llu,\"released\":%llu,\"timeouts\":%llu,\"aborted\":%llu,\"sent\":%llu,\"received\":%llu,\"refused\":%llu,\"lastWaitMs\":%llu,\"caller\":\"0x%x\",\"from\":%u,\"to\":%u,\"pending\":%u,\"localMap\":\"%s\",\"peerMap\":\"%s\",\"ownsWindow\":%s,\"peeks\":%llu}}\n",
        (missionEndMirrorEnabled||productMode)?"true":"false",m.result,localRole,m.sent,m.received,m.applied,m.refused,m.noActive,m.notSameMission,m.scanFailed,m.missions,m.appliedState,m.pending?"true":"false",m.incoming.mapinfo,m.incoming.highChaos,
        transitionHold.result,transitionHold.expect?"true":"false",transitionHold.held?"true":"false",transitionHold.holds,transitionHold.released,transitionHold.timeouts,transitionHold.aborted,transitionHold.sent,transitionHold.received,transitionHold.refused,transitionHold.lastWaitMs,transitionHold.caller,transitionHold.from,transitionHold.to,transitionHold.pending,transitionHold.localMap,transitionHold.peerMap,transitionHold.ownsWindow?"true":"false",transitionHold.peeks);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

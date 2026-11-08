#pragma once
// Stage 8 (#253, mission_decision_protocol.h): one choice at a shared mission end, the host's. Before, each world made
// its own: the host's Replay Mission was consumed as a user reload (the checkpoint rules) and the host stayed on its
// result screen, while the partner's Continue loaded the next mission (take replay-dunwall-vm-v2044a). Now, on a result
// screen whose mission end both worlds share (mission_end_mirror.h, D2ME), the host's Continue or Replay runs and is sent
// as D2MD. The partner's own presses wait for it while connected; on D2MD the partner dispatches the same native command:
// RestartFromMemoryCheckpoint, or LoadGame of its own mission-start save (slot type 2) of the host's replayed map. A
// partner without that save continues instead, and mission follow (session_product.h) then loads the host's mission.
// Both worlds expect the transition, so the both-ready hold meets them at the new map's load end. The host's setting
// holds for both players (control hello flag 16): mission_decision=0 in d2coop.cfg, or the lab switch
// coop_mission_decision off in the host's game, restores each world's own choice (the mutation twin).
#include "mission_decision_protocol.h"
struct MissionDecision {
    bool enabled=true,pending=false,held=false;
    unsigned heldKind=0,appliedKind=0;int heldSlot=-1,loadedSlot=-1;
    std::uint64_t sequence=0,peerSequence=0,sent=0,resent=0,received=0,refused=0,duplicates=0,hostContinues=0,hostReplays=0,unreadable=0;
    std::uint64_t partnerHeld=0,continued=0,replayed=0,missingSaves=0,localReleases=0;
    std::uint64_t decidedEpoch=0,decidedAt=0,sentAt=0,receivedAt=0,appliedEndEpoch=0,partnerAppliedEpoch=0;
    MissionDecisionPacket last{},incoming{};const char* result="idle";
    bool awaitingHostMap=false;std::uint64_t awaitSince=0,followContinues=0,followFallbacks=0; // 2103 (#264, M1)
    std::uint64_t ownEndContinues=0; // 2191
    bool ownFirst=false;std::uint64_t ownFirstEpoch=0,ownFirstSince=0,ownFirstSame=0,ownFirstFollowed=0; // 2193
};
MissionDecision missionDecision{};
bool coopMissionDecisionFlag(){return localRole==0&&missionDecision.enabled;}
// A product partner follows the host's setting from its control hello; a lab partner keeps its own switch.
bool missionDecisionOn(){return localRole==1&&(productMode||productAutoArm)?coopControl.helloOk&&coopControl.peerMissionDecision:missionDecision.enabled;}
// One slot's game.details: its map name and slot type, read with delete sharing and never while the native save thread
// is writing it (checkpoint_mission.h, 1214km).
struct MissionSlotDetails {char map[160]{};int type=-1;std::uint64_t stamp=0;};
bool missionSlotDetails(const char* root,int slot,MissionSlotDetails& d) {
    d={};char path[MAX_PATH]{};
    if(slot<0||slot>99||std::snprintf(path,sizeof(path),"%s\\SLOT%02d\\game.details",root,slot)>=static_cast<int>(sizeof(path)))return false;
    std::uint64_t stamp=0,size=0;bool busy=false;
    if(!missionDetailsEntry(path,stamp,size,busy)||busy)return false;
    HANDLE file=CreateFileA(path,GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE,nullptr,OPEN_EXISTING,0,nullptr);
    if(file==INVALID_HANDLE_VALUE)return false;
    char text[1024]{};DWORD count=0;const bool read=ReadFile(file,text,sizeof(text)-1,&count,nullptr)&&count>0;CloseHandle(file);
    if(!read)return false;text[count]=0;char type[8]{};
    if(!missionDetailsField(text,"mapName",d.map,sizeof(d.map))||!missionDetailsField(text,"slotType",type,sizeof(type)))return false;
    d.type=std::atoi(type);d.stamp=stamp;return true;
}
// The partner's save for the host's Replay: the host's slot number when it holds a mission-start save of the same map,
// else this save root's newest mission-start save of that map. An empty map (the host could not read its own) accepts
// the host's slot number alone.
int missionReplaySlot(int hostSlot,const char* startMap) {
    char root[MAX_PATH]{};if(!missionSaveRoot(root,sizeof(root)))return -1;
    MissionSlotDetails d{};
    if(missionSlotDetails(root,hostSlot,d)&&d.type==2&&(!*startMap||!std::strcmp(d.map,startMap)))return hostSlot;
    if(!*startMap)return -1;
    int best=-1;std::uint64_t newest=0;
    for(int slot=0;slot<100;++slot)if(missionSlotDetails(root,slot,d)&&d.type==2&&!std::strcmp(d.map,startMap)&&(best<0||d.stamp>newest)){best=slot;newest=d.stamp;}
    return best;
}
void sendMissionDecision(std::uint64_t tick) {
    auto& d=missionDecision;if(!d.last.sequence||!checkpointConnected())return;
    if(coopSendMessage(connection,&d.last,sizeof(d.last),k_nSteamNetworkingSend_ReliableNoNagle,nullptr)==k_EResultOK){if(d.sentAt)++d.resent;else ++d.sent;d.sentAt=tick;}
}
// The host repeats its choice every second for two minutes, from its services and from the both-ready hold; the partner
// counts the copies as duplicates.
void resendMissionDecision(std::uint64_t tick) {
    const auto& d=missionDecision;
    if(localRole==0&&d.last.sequence&&tickAge(tick,d.decidedAt)<120000&&tickAge(tick,d.sentAt)>=1000)sendMissionDecision(tick);
}
void missionDecisionHost(unsigned kind,int slot,std::uint64_t tick) {
    auto& d=missionDecision;if(d.decidedEpoch==outgoing.worldEpoch)return; // a native requeue of the same choice
    MissionDecisionPacket p{};p.decision=kind==2?kMissionReplay:kMissionContinue;p.session=outgoing.session;p.endEpoch=outgoing.worldEpoch;
    if(kind==2){
        p.slot=slot;char root[MAX_PATH]{};MissionSlotDetails s{};
        if(missionSaveRoot(root,sizeof(root))&&missionSlotDetails(root,slot,s)&&!std::strncmp(s.map,"campaign/",9)&&!std::strpbrk(s.map," ;\"\r\n\t"))std::snprintf(p.startMap,sizeof(p.startMap),"%s",s.map);
        else ++d.unreadable;
        ++d.hostReplays;d.loadedSlot=slot;
    }else ++d.hostContinues;
    p.sequence=++d.sequence;
    if(!validMissionDecision(p)){d.result="invalid";return;}
    d.last=p;d.decidedEpoch=outgoing.worldEpoch;d.decidedAt=tick;d.sentAt=0;d.result=kind==2?"replay_sent":"continue_sent";
    expectMissionTransition(tick);sendMissionDecision(tick);
}
int missionDecisionIntercept(unsigned kind,int slot) {
    auto& d=missionDecision;
    if(!missionDecisionOn()||GetCurrentThreadId()!=engineThread.load()||checkpointSync.expectBoundary)return -1;
    const bool shared=(missionEndMirrorEnabled||productMode)&&missionEnd.sentEpoch==outgoing.worldEpoch;
    const bool atResults=missionResultsScreen(outgoing.gameLogicState,shared);
    // The partner's dispatch of the host's choice, its native requeues while save I/O is busy and a Replay's memory continuation run.
    const bool permitted=d.partnerAppliedEpoch==outgoing.worldEpoch&&missionDecisionPermits(d.appliedKind,kind);
    const int rule=missionDecisionRule(localRole,kind,atResults,shared,checkpointConnected(),permitted);
    if(rule==0&&kind==1)++checkpointMissionContinues;
    if(rule==0&&localRole==0&&shared)missionDecisionHost(kind,slot,GetTickCount64());
    // 2110: the wait notice of 2108 is not shown: its box was still open when her continue-follow load came, and her game
    // faulted (firstmission-dunwall-v2111a, Dishonored2.exe+0x1f7dd28). Notice kind 2 stays in native_notice.h, unused.
    if(rule==1){if(!d.held)++d.partnerHeld;d.held=true;d.heldKind=kind;d.heldSlot=slot;d.result="waiting_for_host";}
    return rule;
}
void receiveMissionDecision(const MissionDecisionPacket& p) {
    auto& d=missionDecision;++d.received;
    if(localRole!=1||!validMissionDecision(p)||p.session!=remote.session){++d.refused;return;}
    if(p.sequence<=d.peerSequence||p.endEpoch==d.appliedEndEpoch){++d.duplicates;return;}
    d.peerSequence=p.sequence;d.incoming=p;d.pending=true;d.receivedAt=GetTickCount64();d.result="received";
}
void dispatchMissionDecision(void* system,unsigned kind,int slot) {
    auto& d=missionDecision;char command[40]{};
    if(kind==2)std::snprintf(command,sizeof(command),"LoadGame %d\n",slot);else std::snprintf(command,sizeof(command),"RestartFromMemoryCheckpoint\n");
    d.partnerAppliedEpoch=outgoing.worldEpoch;d.appliedKind=kind;d.held=false;
    reinterpret_cast<void(*)(void*,const char*)>(gameImage+0xfec70)(system,command);
}
// 2103 (#264, M1): a partner's Continue goes to the host's next mission. Her own RestartFromMemoryCheckpoint reloaded her
// last checkpoint of the ended mission (firstmission-dunwall-v2095a/b, v2099f: back on the streets, gameflow back at
// the boat captain), because only the host's world ran the native mission end. The host then held at its new map's
// load end for her (both-ready hold, 90 s) and published its mission only after, so mission follow brought her
// across 2 minutes late (firstmission-dunwall-v2100a). Now, with mission follow on, she waits on her result screen
// for the map the host's hold announces (D2MR, transitionHold.peerMap) and loads that mission at once, as a join
// does ("campaign <mapinfo>", her progression restored after it, partner_progress.h). No announcement in 60 s:
// her own Continue as before. Lab switch coop_continue_follow on|off (the twin).
bool missionContinueFollow=true;
// 2193: her own Continue first, then the host's map. Following by "campaign <mapinfo>" loads the next mission fresh, so
// a hub such as the Dreadful Wale lacks what her campaign carried (campaign-curator-vm-v2181b), and a finished mission
// node her world had is gone (campaign-madinventor-vm-v2191m: bf086e4f 4 on both, absent after the follow). Her native
// Continue loads the next mission start her world cached at Results and keeps all of it. When that is not the host's
// map (her world never ran the mission end: campaign-bunker-vm-v2181a, -empress, firstmission-dunwall-v2095a), the
// hold releases at once (mission_end_mirror.h) and she loads the host's mission. missionContinueOwnFirst is the switch.
bool missionFollowOn(){return productMode||followHostEnabled;}
bool continueToHostMission(void* system){
    auto& d=missionDecision;const char* peer=transitionHold.peerMap;
    if(!*peer||!std::strcmp(peer,mapName))return false;
    char mapinfo[160]{};if(!mapinfoFromMapName(peer,mapinfo,sizeof(mapinfo)))return false;
    const int character=localCharacter(); // she goes on as the character she plays
    char command[220]{};std::snprintf(command,sizeof(command),"campaign %s %s %s",mapinfo,std::strstr(peer,"highchaos")?"high":"low",character==1?"corvo":"emily");
    coopNativeChoicePick=character;partnerProgressJoinPending=true;partnerProgressJoinCharacter=character;
    d.partnerAppliedEpoch=outgoing.worldEpoch;d.appliedKind=1;d.held=false;d.awaitingHostMap=false;
    dispatchCoopCommand(system,command);++d.followContinues;++d.continued;d.result="continued_to_host_mission";
    return true;
}
void serviceMissionDecision(void* system,std::uint64_t tick) {
    if(GetCurrentThreadId()!=engineThread.load())return;
    auto& d=missionDecision;
    if(localRole==0){resendMissionDecision(tick);return;}
    const bool shared=(missionEndMirrorEnabled||productMode)&&missionEnd.sentEpoch==outgoing.worldEpoch;
    const bool atResults=missionResultsScreen(outgoing.gameLogicState,shared)&&!outgoing.pendingGameLogicState;
    if(d.held&&!(atResults&&shared))d.held=false;
    // A choice held for a host that is gone is the player's own again.
    if(d.held&&!d.pending&&!checkpointConnected()){
        const unsigned kind=d.heldKind;const int slot=d.heldSlot;
        if(kind==1||(kind==2&&slot>=0&&slot<=99)){dispatchMissionDecision(system,kind,slot);++d.localReleases;d.result="released_local";}
        else d.held=false;
        return;
    }
    if(d.ownFirst){
        if(tickAge(tick,d.ownFirstSince)>180000){d.ownFirst=false;d.result="own_first_timeout";return;}
        if(outgoing.worldEpoch==d.ownFirstEpoch||!coopGameplayReady(outgoing)||outgoing.pendingGameLogicState)return;
        const char* peer=transitionHold.peerMap;if(!*peer)return;
        d.ownFirst=false;
        if(!std::strcmp(peer,mapName)){++d.ownFirstSame;d.result="own_first_host_map";return;}
        // 2195: the product mission follow (session_product.h) loads the host's mission for a partner on another map, with
        // her chosen character; loading here as well queued two campaign loads in one frame and the partner's game vanished
        // (soak-hour-vm-v2186b, Edge of the World to The Good Doctor). So this only records it.
        ++d.ownFirstFollowed;d.result="own_first_other_map";
        return;
    }
    if(d.awaitingHostMap){
        if(!atResults||!shared){d.awaitingHostMap=false;d.result="await_left_results";return;}
        if(continueToHostMission(system))return;
        if(tickAge(tick,d.awaitSince)>60000){d.awaitingHostMap=false;++d.followFallbacks;expectMissionTransition(tick);dispatchMissionDecision(system,1,-1);++d.continued;d.result="continued_own_fallback";}
        return;
    }
    if(!d.pending)return;
    if(tickAge(tick,d.receivedAt)>120000){d.pending=false;d.result="expired";return;}
    if(!missionDecisionOn()){d.pending=false;d.result="off";return;}
    if(!atResults||!shared||d.partnerAppliedEpoch==outgoing.worldEpoch){d.result="waiting_results";return;}
    d.pending=false;d.appliedEndEpoch=d.incoming.endEpoch;
    if(d.incoming.decision==kMissionReplay){
        const int slot=missionReplaySlot(d.incoming.slot,d.incoming.startMap);
        if(slot>=0){d.loadedSlot=slot;expectMissionTransition(tick);dispatchMissionDecision(system,2,slot);++d.replayed;d.result="replayed";return;}
        // No mission-start save of that map: continue alone, without the hold, and follow the host's mission from there.
        ++d.missingSaves;transitionHold.expect=false;dispatchMissionDecision(system,1,-1);d.result="replay_save_missing";return;
    }
    if(missionContinueFollow&&missionFollowOn()&&missionContinueOwnFirst){d.ownFirst=true;d.ownFirstEpoch=outgoing.worldEpoch;d.ownFirstSince=tick;
        ++d.ownEndContinues;expectMissionTransition(tick);dispatchMissionDecision(system,1,-1);++d.continued;d.result="continued_own_first";return;}
    if(missionContinueFollow&&missionFollowOn()){d.awaitingHostMap=true;d.awaitSince=tick;expectMissionTransition(tick);d.result="awaiting_host_mission";
        continueToHostMission(system);return;}
    expectMissionTransition(tick);dispatchMissionDecision(system,1,-1);++d.continued;d.result="continued";
}
void writeMissionDecision() {
    char path[MAX_PATH]{},temp[MAX_PATH]{};
    if(std::snprintf(path,sizeof(path),"%s.missiondecision.json",statusPath)>=static_cast<int>(sizeof(path))||std::snprintf(temp,sizeof(temp),"%s.tmp",path)>=static_cast<int>(sizeof(temp)))return;
    FILE* f=nullptr;if(fopen_s(&f,temp,"wb")||!f)return;const auto& d=missionDecision;const auto& p=localRole?d.incoming:d.last;
    fprintf(f,"{\"enabled\":%s,\"role\":%u,\"result\":\"%s\",\"decision\":%u,\"slot\":%d,\"startMap\":\"%s\",\"loadedSlot\":%d,\"pending\":%s,\"held\":%s,\"heldKind\":%u,"
        "\"sent\":%llu,\"resent\":%llu,\"received\":%llu,\"refused\":%llu,\"duplicates\":%llu,\"hostContinues\":%llu,\"hostReplays\":%llu,\"unreadable\":%llu,"
        "\"partnerHeld\":%llu,\"continued\":%llu,\"replayed\":%llu,\"missingSaves\":%llu,\"localReleases\":%llu,\"continueFollow\":%s,\"awaitingHostMap\":%s,\"followContinues\":%llu,\"followFallbacks\":%llu,\"continueOwn\":%s,\"ownEndContinues\":%llu,\"ownFirst\":%s,\"ownFirstSame\":%llu,\"ownFirstFollowed\":%llu,\"otherMapReleases\":%llu}\n",
        missionDecisionOn()?"true":"false",localRole,d.result,p.decision,p.slot,p.startMap,d.loadedSlot,d.pending?"true":"false",d.held?"true":"false",d.heldKind,
        d.sent,d.resent,d.received,d.refused,d.duplicates,d.hostContinues,d.hostReplays,d.unreadable,d.partnerHeld,d.continued,d.replayed,d.missingSaves,d.localReleases,
        missionContinueFollow?"true":"false",d.awaitingHostMap?"true":"false",d.followContinues,d.followFallbacks,missionContinueOwnFirst?"true":"false",d.ownEndContinues,d.ownFirst?"true":"false",d.ownFirstSame,d.ownFirstFollowed,transitionOtherMapReleases);
    const bool ok=!ferror(f);const int closed=fclose(f);if(ok&&!closed)MoveFileExA(temp,path,MOVEFILE_REPLACE_EXISTING);
}

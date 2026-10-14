#include "checkpoint_protocol.h"
#include "checkpoint_barrier_policy.h"
#include "autosave_pair_protocol.h"
#include <cstdio>
#include <cstdlib>
void require(bool ok,const char* name){if(!ok){std::fprintf(stderr,"FAIL: %s\n",name);std::exit(1);}}
int main() {
    // 1153 paired autosaves: settled only when counts match, each side mirrored the
    // other's originated saves and no native mirror request is in flight.
    require(autosavePairSettled(0,0,0,false,0,0,0,false),"fresh arm is settled");
    require(!autosavePairSettled(1,1,0,false,0,0,0,false),"host-originated save not yet mirrored");
    require(!autosavePairSettled(1,1,0,false,0,0,0,true),"peer mirror in flight");
    require(autosavePairSettled(1,1,0,false,1,0,1,false),"peer mirrored the host save");
    require(autosavePairSettled(2,1,1,false,2,1,1,false),"both originated once and mirrored once");
    require(!autosavePairSettled(2,1,1,false,2,1,0,false),"one-sided mirror is not settled");
    {AutosavePairPacket p{};p.role=0;p.session=1;p.arm=1;p.epoch=1;p.mapId=1;p.saves=2;p.originated=1;p.mirrored=1;
     require(validAutosavePair(p,1)&&!validAutosavePair(p,0),"autosave pair packet comes from the peer role");
     p.mirrored=3;require(!validAutosavePair(p,1),"mirrored cannot exceed saves");p.mirrored=1;p.pending=2;require(!validAutosavePair(p,1),"pending is a flag");}
    require(checkpointFixedSlotRetry(true,true,true,false,CheckpointPhase::Loading,2,82,100,100,true),"busy native LoadGame retries retain their existing fixed-slot attempt");
    for(unsigned i=0;i<11;++i){bool enabled=true,permit=true,expected=true,entered=false,inTime=true;auto phase=CheckpointPhase::Loading;unsigned kind=2;int slot=82;std::uintptr_t oldGame=100,currentGame=100;
        switch(i){case 0:enabled=false;break;case 1:permit=false;break;case 2:expected=false;break;case 3:entered=true;break;case 4:phase=CheckpointPhase::Idle;break;case 5:phase=CheckpointPhase::Held;break;case 6:kind=1;break;case 7:slot=81;break;case 8:oldGame=0;break;case 9:currentGame=200;break;case 10:inTime=false;break;}
        require(!checkpointFixedSlotRetry(enabled,permit,expected,entered,phase,kind,slot,oldGame,currentGame,inTime),"native retry cannot authorize a different slot, new world, completed or expired transaction");}
    for(unsigned state=0;state<18;++state){
        require(checkpointBlocksWorldActions(true,CheckpointPhase::Idle,false,state,0,1,0)==(state==13),"only native death commits early barrier");
        require(checkpointBlocksWorldActions(true,CheckpointPhase::Idle,false,1,state,1,0)==(state==13),"pending local death blocks before protocol sees committed state");
        require(checkpointBlocksWorldActions(true,CheckpointPhase::Idle,false,1,0,state,0)==(state==13),"peer death blocks before reliable death packet");
        require(checkpointBlocksWorldActions(true,CheckpointPhase::Idle,false,1,0,1,state)==(state==13),"pending peer death blocks world actions");
    }
    for(unsigned phase=0;phase<=7;++phase){const auto p=static_cast<CheckpointPhase>(phase);
        require(!checkpointBlocksWorldActions(false,p,true,13,13,13,13),"disabled checkpoint cannot retain world-session intent");
        require(checkpointBlocksWorldActions(true,p,false,1,0,1,0)==(p!=CheckpointPhase::Idle&&p!=CheckpointPhase::Failed),"ordinary recovery barriers retained");
        require(checkpointBlocksWorldActions(true,p,true,13,0,13,0)==(p!=CheckpointPhase::Failed),"failed death protocol cannot retain forever");
    }
    CheckpointPacket p{};p.role=1;p.sequence=1;p.hostSession=11;p.clientSession=12;p.clientArm=3;p.mapId=42;p.digest[0]=5;
    require(checkpointPacketShape(p,0),"first hello can discover peer arm");
    auto q=p;q.version++;require(!checkpointPacketShape(q,0),"unknown version");
    q=p;q.size--;require(!checkpointPacketShape(q,0),"wrong size");
    q=p;q.sequence=0;require(!checkpointPacketShape(q,0),"zero sequence");
    require(!checkpointPacketShape(p,1),"same role");
    p.hostArm=4;p.hostEpoch=7;p.clientEpoch=8;p.kind=CheckpointMessage::Request;
    require(checkpointPacketShape(p,0),"client request");q=p;q.role=0;require(!checkpointPacketShape(q,1),"host cannot forge client request");
    p.transaction=9;p.kind=CheckpointMessage::Accept;
    require(checkpointPacketShape(p,0),"accept");
    require(checkpointSameAttempt(p,p),"same attempt");
    for(int field=0;field<9;++field){q=p;switch(field){case 0:++q.hostSession;break;case 1:++q.clientSession;break;case 2:++q.hostArm;break;case 3:++q.clientArm;break;case 4:++q.transaction;break;case 5:++q.hostEpoch;break;case 6:++q.clientEpoch;break;case 7:++q.mapId;break;case 8:++q.digest[31];break;}
        require(!checkpointSameAttempt(p,q),"attempt identity change rejected");}
    p.kind=CheckpointMessage::Ready;p.character=2;p.clientReady=18;
    require(checkpointPacketShape(p,0),"client ready");
    q=p;q.character=1;require(!checkpointPacketShape(q,0),"wrong character");
    q=p;q.hostReady=17;require(!checkpointPacketShape(q,0),"cannot advertise peer ready token");
    q=p;q.clientReady=0;require(!checkpointPacketShape(q,0),"empty ready");
    p.kind=CheckpointMessage::Release;p.role=0;p.character=0;p.hostReady=17;
    require(checkpointPacketShape(p,1),"release carries both tokens");
    q=p;q.clientReady=0;require(!checkpointPacketShape(q,1),"single-sided release refused");
    for(unsigned phase=0;phase<=7;++phase) {
        require(checkpointPhaseAccepts(static_cast<CheckpointPhase>(phase),CheckpointMessage::Release,1)==(phase==5),"client release only while held");
        require(!checkpointPhaseAccepts(static_cast<CheckpointPhase>(phase),CheckpointMessage::Release,0),"client cannot release host");
        require(checkpointPhaseAccepts(static_cast<CheckpointPhase>(phase),CheckpointMessage::Begin,1)==(phase==3),"begin only after acceptance");
    }
    require(!checkpointPhaseAccepts(CheckpointPhase::Held,CheckpointMessage::Prepare,1),"duplicate prepare cannot restart load");
    require(!checkpointPhaseAccepts(CheckpointPhase::Loading,CheckpointMessage::Accept,0),"duplicate acceptance cannot queue second load");
    require(!checkpointPhaseAccepts(CheckpointPhase::Failed,CheckpointMessage::Ready,0),"failed session cannot release");
    require(checkpointPhaseAccepts(CheckpointPhase::Held,CheckpointMessage::Abort,1),"abort retained call stack");
    p.kind=CheckpointMessage::Death;p.transaction=0;p.hostReady=p.clientReady=0;
    require(checkpointPacketShape(p,1),"host death notice");
    p.role=1;require(checkpointPacketShape(p,0),"client death notice");
    p.kind=CheckpointMessage::DeathReady;p.character=2;
    require(checkpointPacketShape(p,0),"client native death-screen acknowledgment");
    q=p;q.character=1;require(!checkpointPacketShape(q,0),"wrong death-screen character");
    q=p;q.transaction=1;require(!checkpointPacketShape(q,0),"death cannot authorize a load transaction");
    q=p;q.hostEpoch++;require(!checkpointSameWorld(p,q),"old death belongs to another world");
    q=p;q.clientArm++;require(!checkpointSameWorld(p,q),"old death cannot cross a rearm");
    std::puts("Checkpoint packet identity, direction, role, token and phase checks passed.");
}

#pragma once
#include <cstdint>
#include <cmath>
#include <algorithm>
#include "body_retarget.h"
#pragma pack(push,1)
struct CoopPosePacket {
    std::uint32_t magic=0x50433244; // D2CP
    std::uint16_t version=10,size=sizeof(CoopPosePacket);
    std::uint32_t build=0x67a50cde,role=0,sequence=0,flags=0;
    std::uint64_t session=0,worldEpoch=0,mapId=0;
    std::uint32_t gameLogicState=0,pendingGameLogicState=0;
    std::uint64_t cinematicSignature=0;
    std::uint32_t cinematicNodeCount=0,cinematicMetadataValid=0;
    std::uint64_t cinematicEpoch=0;
    std::uint32_t cinematicSyncEnabled=0,cinematicCanSkip=0;
    std::uint64_t sequenceSignature=0,sequenceBoundaryId=0;
    std::uint32_t sequenceCount=0,sequenceMetadataValid=0;
    std::uint32_t sequenceTimeMs=0,sequencePaused=0,barrierEnabled=0,barrierHeld=0;
    float position[3]{},orientation[9]{};
    // Physical player capsule stance, separate from the NPC animation enum.
    // Only live-mapped standing=0 and crouched=1 are currently advertised.
    std::uint32_t playerStanceValid=0,playerStance=0;
    std::uint32_t playerMotionValid=0,playerMotion=0;
    // Fixed 20-joint player-body semantic order, affine XYZ columns only.
    // Process-local native joint name IDs and pointers never cross the wire.
    std::uint32_t bodyPoseValid=0;
    float bodyPose[20][12]{};
};
#pragma pack(pop)
static_assert(sizeof(CoopPosePacket)==1156);
#pragma pack(push,1)
struct CoopSkipPacket {
    std::uint32_t magic=0x53433244; // D2CS
    std::uint16_t version=3,size=sizeof(CoopSkipPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0;
    std::uint64_t sourceWorldEpoch=0,targetWorldEpoch=0,sourceCinematicEpoch=0,targetCinematicEpoch=0;
    std::uint64_t mapId=0,cinematicSceneId=0,sequenceBoundaryId=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopSkipPacket)==96);
#pragma pack(push,1)
struct CoopBarrierPacket {
    std::uint32_t magic=0x42433244; // D2CB
    std::uint16_t version=1,size=sizeof(CoopBarrierPacket);
    std::uint32_t build=0x67a50cde,role=0;
    std::uint64_t sequence=0,sourceSession=0,targetSession=0;
    std::uint64_t sourceWorldEpoch=0,targetWorldEpoch=0,sourceCinematicEpoch=0,targetCinematicEpoch=0;
    std::uint64_t mapId=0,cinematicSceneId=0,sequenceBoundaryId=0;
};
#pragma pack(pop)
static_assert(sizeof(CoopBarrierPacket)==96);
inline std::uint64_t coopMapId(const char* name) {
    std::uint64_t value=14695981039346656037ull;
    for(;*name;++name){value^=static_cast<unsigned char>(*name);value*=1099511628211ull;}
    return value;
}
struct CoopCinematicKey {std::uint64_t ownerId;std::uint32_t nodeId;};
struct CoopSequenceKey {std::uint64_t assetId,subsceneId;std::uint32_t durationMs,channels;};
inline std::uint64_t coopSequenceSignature(const CoopSequenceKey* keys,unsigned count,std::uint64_t mapId=0,bool canonical=false) {
    if(!keys||!count||count>2)return 0;
    CoopSequenceKey ordered[2]{};std::copy(keys,keys+count,ordered);
    for(unsigned i=0;i<count;++i) {
        auto& k=ordered[i];
        if(!k.assetId||!k.subsceneId||!k.channels||k.channels>3)return 0;
        // Only the observed, authored Corvo/Emily alternatives at this exact
        // boundary are equivalent. A shared story observer alone is too broad.
        if(canonical&&mapId==coopMapId("campaign/dunwall/escape/tower/dunwall_escape_tower_p")&&
           k.assetId==coopMapId("midnight/campaign/dunwall/escape/tower/coup_main_post_fight.midnightscene")&&
           k.durationMs==28000&&k.subsceneId==coopMapId("player_is_emily"))k.subsceneId=coopMapId("player_is_corvo");
    }
    std::sort(ordered,ordered+count,[](const auto& a,const auto& b){
        if(a.assetId!=b.assetId)return a.assetId<b.assetId;
        if(a.subsceneId!=b.subsceneId)return a.subsceneId<b.subsceneId;
        if(a.durationMs!=b.durationMs)return a.durationMs<b.durationMs;
        return a.channels<b.channels;
    });
    std::uint64_t value=14695981039346656037ull;
    const auto mix=[&](std::uint64_t word,unsigned bytes){for(unsigned i=0;i<bytes;++i){value^=(word>>(i*8))&255;value*=1099511628211ull;}};
    mix(count,4);for(unsigned i=0;i<count;++i){mix(ordered[i].assetId,8);mix(ordered[i].subsceneId,8);mix(ordered[i].durationMs,4);mix(ordered[i].channels,4);}
    return value;
}
inline std::uint64_t coopCinematicSignature(const CoopCinematicKey* keys,unsigned count) {
    if(!keys||!count||count>8)return 0;
    CoopCinematicKey ordered[8]{};std::copy(keys,keys+count,ordered);
    std::sort(ordered,ordered+count,[](const auto& a,const auto& b){return a.ownerId<b.ownerId||(a.ownerId==b.ownerId&&a.nodeId<b.nodeId);});
    std::uint64_t value=14695981039346656037ull;
    const auto mix=[&](std::uint64_t word,unsigned bytes){for(unsigned i=0;i<bytes;++i){value^=(word>>(i*8))&255;value*=1099511628211ull;}};
    mix(count,4);
    for(unsigned i=0;i<count;++i){mix(ordered[i].ownerId,8);mix(ordered[i].nodeId,4);}
    return value;
}
inline bool validCoopPose(const CoopPosePacket& p,std::uint32_t localRole) {
    if(p.magic!=0x50433244 || p.version!=10 || p.size!=sizeof(p) || p.build!=0x67a50cde ||
       p.role>1 || p.role==localRole || !p.sequence || !p.session || (p.flags&~1u) || ((p.flags&1)&&(!p.worldEpoch||!p.mapId))) return false;
    if(p.gameLogicState>=17||p.pendingGameLogicState>=17)return false;
    if(p.playerStanceValid>1||p.playerStance>1||(!p.playerStanceValid&&p.playerStance)||
       (p.playerStanceValid&&(!(p.flags&1)||p.gameLogicState!=1||p.pendingGameLogicState)))return false;
    if(p.playerMotionValid>1||p.playerMotion>3||(!p.playerMotionValid&&p.playerMotion)||
       (p.playerMotionValid&&!p.playerStanceValid))return false;
    if(p.bodyPoseValid>1||(p.bodyPoseValid&&!p.playerMotionValid))return false;
    for(unsigned j=0;j<20;++j){BodyMatrix m{};
        for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r){const float f=p.bodyPose[j][c*3+r];
            if(!std::isfinite(f)||(!p.bodyPoseValid&&f!=0))return false;m.v[c*4+r]=f;}
        if(p.bodyPoseValid&&!bodyRigid(m,3.f))return false;
        if(p.bodyPoseValid&&j==0){const auto identity=bodyIdentity();for(unsigned c=0;c<4;++c)for(unsigned r=0;r<3;++r)if(std::fabs(m.v[c*4+r]-identity.v[c*4+r])>.001f)return false;}
    }
    if(p.cinematicMetadataValid>1||p.cinematicNodeCount>8||
       (!p.cinematicMetadataValid&&(p.cinematicNodeCount||p.cinematicSignature))||
       (p.cinematicMetadataValid&&((p.cinematicNodeCount==0)!=(p.cinematicSignature==0))))return false;
    if(p.sequenceMetadataValid>1||p.sequenceCount>2||
       (!p.sequenceMetadataValid&&(p.sequenceCount||p.sequenceSignature||p.sequenceBoundaryId))||
       (p.sequenceMetadataValid&&((p.sequenceCount==0)!=(p.sequenceSignature==0)||(p.sequenceCount==0)!=(p.sequenceBoundaryId==0))))return false;
    if(!p.cinematicEpoch||p.cinematicSyncEnabled>1||p.cinematicCanSkip>1||
       (p.cinematicCanSkip&&(p.gameLogicState!=5||p.pendingGameLogicState||!p.cinematicMetadataValid||!p.cinematicNodeCount||!p.sequenceMetadataValid||!p.sequenceCount||p.sequencePaused)))return false;
    if(p.sequenceTimeMs>86400000||p.sequencePaused>1||p.barrierEnabled>1||p.barrierHeld>1||
       ((!p.sequenceMetadataValid||!p.sequenceCount)&&(p.sequenceTimeMs||p.sequencePaused))||
       (p.barrierHeld&&(!p.barrierEnabled||!p.sequencePaused||p.sequenceCount!=1||p.gameLogicState!=5||p.pendingGameLogicState)))return false;
    for(float f:p.position)if(!std::isfinite(f)||std::fabs(f)>1000000.f)return false;
    for(float f:p.orientation)if(!std::isfinite(f)||std::fabs(f)>1.001f)return false;
    if(p.flags&1)for(int row=0;row<3;++row) {
        float length=0;for(int i=0;i<3;++i)length+=p.orientation[row*3+i]*p.orientation[row*3+i];
        if(length<0.98f||length>1.02f)return false;
    }
    if(p.flags&1) {
        for(int a=0;a<3;++a)for(int b=a+1;b<3;++b) {
            float dot=0;for(int i=0;i<3;++i)dot+=p.orientation[a*3+i]*p.orientation[b*3+i];
            if(std::fabs(dot)>0.02f)return false;
        }
        const auto* m=p.orientation;
        const float determinant=m[0]*(m[4]*m[8]-m[5]*m[7])-m[1]*(m[3]*m[8]-m[5]*m[6])+m[2]*(m[3]*m[7]-m[4]*m[6]);
        if(determinant<0.98f||determinant>1.02f)return false;
    }
    return true;
}
inline bool coopGameplayReady(const CoopPosePacket& p) {
    return (p.flags&1)&&p.gameLogicState==1&&p.pendingGameLogicState==0&&p.cinematicMetadataValid==1&&p.cinematicNodeCount==0;
}
inline std::uint64_t coopSkipSceneId(const CoopPosePacket& p) {
    if(!p.cinematicMetadataValid||!p.cinematicNodeCount)return 0;
    // Build-specific opening sequence: Emily retains the character-choice
    // observer while Corvo releases it. Both branches start node 190612608.
    // Match only the two exact observed sets on this exact map; arbitrary
    // overlapping node sets do not establish a shared skip boundary.
    static const auto openingMap=coopMapId("campaign/dunwall/escape/tower/dunwall_escape_tower_p");
    static const CoopCinematicKey postChoice[]={{coopMapId("ld_kiscule_coup"),190612608}};
    static const CoopCinematicKey emilyPostChoice[]={postChoice[0],{postChoice[0].ownerId,582079941}};
    static const auto postChoiceSignature=coopCinematicSignature(postChoice,1);
    static const auto emilySignature=coopCinematicSignature(emilyPostChoice,2);
    if(p.mapId==openingMap&&p.cinematicNodeCount==2&&p.cinematicSignature==emilySignature)return postChoiceSignature;
    return p.cinematicSignature;
}
inline bool coopSharedSkipReady(const CoopPosePacket& local,const CoopPosePacket& peer) {
    return local.cinematicSyncEnabled==1&&peer.cinematicSyncEnabled==1&&local.cinematicCanSkip==1&&peer.cinematicCanSkip==1&&
        local.mapId&&local.mapId==peer.mapId&&local.worldEpoch&&peer.worldEpoch&&
        local.gameLogicState==5&&peer.gameLogicState==5&&!local.pendingGameLogicState&&!peer.pendingGameLogicState&&
        local.cinematicMetadataValid==1&&peer.cinematicMetadataValid==1&&local.cinematicNodeCount>0&&peer.cinematicNodeCount>0&&
        coopSkipSceneId(local)==coopSkipSceneId(peer)&&local.sequenceMetadataValid==1&&peer.sequenceMetadataValid==1&&
        local.sequenceCount>0&&peer.sequenceCount>0&&!local.sequencePaused&&!peer.sequencePaused&&local.sequenceBoundaryId&&local.sequenceBoundaryId==peer.sequenceBoundaryId;
}
inline bool coopOpeningBarrierScene(const CoopPosePacket& p) {
    static const auto map=coopMapId("campaign/dunwall/escape/tower/dunwall_escape_tower_p");
    static const CoopSequenceKey key{coopMapId("midnight/campaign/dunwall/escape/tower/coup_main_post_fight.midnightscene"),coopMapId("player_is_corvo"),28000,3};
    static const auto boundary=coopSequenceSignature(&key,1,map,true);
    static const CoopCinematicKey observer{coopMapId("ld_kiscule_coup"),190612608};
    return p.mapId==map&&p.worldEpoch&&p.gameLogicState==5&&!p.pendingGameLogicState&&p.sequenceMetadataValid==1&&p.sequenceCount==1&&
        p.sequenceBoundaryId==boundary&&coopSkipSceneId(p)==coopCinematicSignature(&observer,1);
}
inline bool coopBarrierReady(const CoopPosePacket& local,const CoopPosePacket& peer) {
    return local.barrierEnabled==1&&peer.barrierEnabled==1&&local.barrierHeld==1&&peer.barrierHeld==1&&
        local.sequencePaused==1&&peer.sequencePaused==1&&coopOpeningBarrierScene(local)&&coopOpeningBarrierScene(peer);
}
inline bool validCoopBarrier(const CoopBarrierPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer) {
    auto releasingPeer=peer;releasingPeer.barrierHeld=1;releasingPeer.sequencePaused=1;
    return p.magic==0x42433244&&p.version==1&&p.size==sizeof(p)&&p.build==0x67a50cde&&p.sequence&&
        p.role==0&&peer.role==0&&local.role==1&&p.sourceSession==peer.session&&p.targetSession==local.session&&
        p.sourceWorldEpoch==peer.worldEpoch&&p.targetWorldEpoch==local.worldEpoch&&
        p.sourceCinematicEpoch==peer.cinematicEpoch&&p.targetCinematicEpoch==local.cinematicEpoch&&
        p.mapId==local.mapId&&p.cinematicSceneId==coopSkipSceneId(local)&&p.sequenceBoundaryId==local.sequenceBoundaryId&&
        coopBarrierReady(local,releasingPeer);
}
inline bool validCoopSkip(const CoopSkipPacket& p,const CoopPosePacket& local,const CoopPosePacket& peer) {
    // A source's subsequent pose may already report its skip committed. The
    // reliable confirmation is the evidence for its prior readiness; require
    // the receiving native scene to remain independently skippable.
    auto confirmedPeer=peer;confirmedPeer.cinematicCanSkip=1;
    return p.magic==0x53433244&&p.version==3&&p.size==sizeof(p)&&p.build==0x67a50cde&&p.sequence&&
        p.role==peer.role&&p.role!=local.role&&p.sourceSession==peer.session&&p.targetSession==local.session&&
        p.sourceWorldEpoch==peer.worldEpoch&&p.targetWorldEpoch==local.worldEpoch&&
        p.sourceCinematicEpoch==peer.cinematicEpoch&&p.targetCinematicEpoch==local.cinematicEpoch&&
        p.mapId==local.mapId&&p.cinematicSceneId==coopSkipSceneId(local)&&p.sequenceBoundaryId==local.sequenceBoundaryId&&coopSharedSkipReady(local,confirmedPeer);
}

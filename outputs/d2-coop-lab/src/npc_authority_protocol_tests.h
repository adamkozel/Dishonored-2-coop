#pragma once
#include "npc_authority_protocol.h"
void npcAuthorityProtocolTests(){
    CoopPosePacket host{},client{};host.role=0;client.role=1;host.session=11;client.session=22;host.worldEpoch=3;client.worldEpoch=5;
    host.mapId=client.mapId=9;host.flags=client.flags=1;host.gameLogicState=client.gameLogicState=1;host.cinematicMetadataValid=client.cinematicMetadataValid=1;
    host.sampleTimeUs=client.sampleTimeUs=1000000;host.orientation[0]=host.orientation[4]=host.orientation[8]=1;client.orientation[0]=client.orientation[4]=client.orientation[8]=1;
    host.sequence=client.sequence=1;host.cinematicEpoch=client.cinematicEpoch=1;
    CoopNpcControl p{};p.h.size=sizeof(p);p.h.sequence=1;p.h.sourceSession=11;p.h.targetSession=22;p.h.sourceEpoch=3;p.h.targetEpoch=5;p.h.map=9;
    p.h.sourceArm=7;p.h.sourceHandle=0x10003;p.h.rig=456;p.h.joints=224;std::strcpy(p.spawner,"authored_actor");std::strcpy(p.definition,"arbitrary_native_npc.def");
    require(validNpcControl(p,client,host),"generic NPC hello carries scoped native rig and authored identity");
    for(unsigned i=0;i<18;++i){auto b=p;switch(i){case 0:++b.h.sourceEpoch;break;case 1:++b.h.targetEpoch;break;case 2:++b.h.sourceSession;break;case 3:++b.h.targetSession;break;case 4:++b.h.map;break;
        case 5:b.h.sourceArm=0;break;case 6:b.h.sourceHandle=0;break;case 7:b.h.rig=0;break;case 8:b.h.joints=257;break;case 9:b.h.joints=0;break;case 10:--b.h.size;break;
        case 11:b.h.targetArm=9;break;case 12:b.h.reserved=1;break;case 13:b.h.reserved2=1;break;case 14:b.h.role=1;break;case 15:b.h.sequence=0;break;case 16:b.spawner[191]='x';break;case 17:b.h.kind=NpcAuthorityMessage::Pose;break;}
        require(!validNpcControl(b,client,host),"malformed or stale NPC negotiation refused");}
    {// 1213u (#46): the client's paired takedown hold/knockout, validated on the host.
     CoopNpcControl q=p;q.h.role=1;q.h.sourceSession=22;q.h.targetSession=11;q.h.sourceEpoch=5;q.h.targetEpoch=3;q.h.targetArm=8;q.h.targetHandle=0x50009;
     q.h.kind=NpcAuthorityMessage::PairedHold;q.h.reserved=2;require(validNpcControl(q,host,client),"client paired hold names the host NPC and its choke phase");
     q.h.reserved=5;require(validNpcControl(q,host,client),"client paired hold for the fast-back lethal");
     q.h.reserved=6;require(validNpcControl(q,host,client),"back lethal (6) is a paired hold variant");
     q.h.reserved=58;require(validNpcControl(q,host,client)&&npcPairedKnockoutVariant(58)&&npcPairedKnockoutVariant(3)&&!npcPairedKnockoutVariant(5),"a drop choke (58) is a paired knockout scene");
     q.h.reserved=57;require(!validNpcControl(q,host,client),"choke cancel (57) is not a paired hold variant");
     q.h.reserved=119;require(validNpcControl(q,host,client)&&!npcPairedKnockoutVariant(119),"a sword lock (119) holds the host NPC and never asks for a knockout");
     q.h.reserved=105;require(validNpcControl(q,host,client),"a counter finisher (105) is a paired hold variant");
     require(npcPairedCorpseFollows(105,true,true,10000,0,10200),"a client finisher hold on a fresh host corpse follows the client victim (take 1258l)");
     require(!npcPairedCorpseFollows(105,true,true,10000,0,13500),"a corpse older than 3 s with no window does not start following");
     require(npcPairedCorpseFollows(105,true,true,10000,13600,13500),"an open window keeps following past 3 s while holds arrive");
     require(!npcPairedCorpseFollows(3,true,true,10000,0,10200)&&!npcPairedCorpseFollows(119,true,true,10000,0,10200),"chokes and sword locks never move a corpse");
     require(!npcPairedCorpseFollows(105,true,false,0,0,10200)&&!npcPairedCorpseFollows(105,false,true,10000,0,10200),"a living or unready NPC takes the ordinary hold path");
     {auto k=q;k.h.kind=NpcAuthorityMessage::PairedKnockout;k.h.reserved=73;require(validNpcControl(k,host,client),"a slide choke (73) asks for the knockout (take 1254a: 1214c capped it at 3 and never sent it)");
      k.h.reserved=5;require(!validNpcControl(k,host,client),"a lethal never asks for a knockout");k.h.reserved=1;require(!validNpcControl(k,host,client),"a choke entry never asks for a knockout");}
     q.h.reserved=kNativeTakedownSceneCount+1;require(!validNpcControl(q,host,client),"unknown takedown scene refused");
     q.h.reserved=4;require(!validNpcControl(q,host,client),"breakout (4) is not a paired hold variant");
     q.h.reserved=0;require(!validNpcControl(q,host,client),"paired hold needs a variant");
     q.h.kind=NpcAuthorityMessage::PairedKnockout;q.h.reserved=3;require(validNpcControl(q,host,client),"client knockout request at choke_win");
     q.h.reserved=5;require(!validNpcControl(q,host,client),"a lethal is never a knockout");
     q.h.reserved=3;q.h.role=0;require(!validNpcControl(q,host,client),"the host never sends a paired request");
     q.h.role=1;q.h.targetArm=0;require(!validNpcControl(q,host,client),"paired request names its host channel");}
    {// 1213v: the host tells the client its NPC is natively unconscious.
     CoopNpcControl u=p;u.h.kind=NpcAuthorityMessage::Unconscious;u.h.targetArm=8;u.h.targetHandle=0x50009;
     require(validNpcControl(u,client,host),"host reports an unconscious NPC to the client");
     u.h.reserved=3;require(!validNpcControl(u,client,host),"unconscious carries no variant");u.h.reserved=0;
     u.h.targetArm=0;require(!validNpcControl(u,client,host),"unconscious names the client channel");}
    {// 1214ac: fair relevance at the cap.
     std::vector<std::array<float,2>> d;for(unsigned i=0;i<24;++i)d.push_back({200.f*200.f,float(i+1)*float(i+1)});d.push_back({5.f*5.f,190.f*190.f});
     const auto pr=npcRelevancePriorities(d,55.f*55.f);
     require(pr[24]==0&&pr[0]==0&&pr[23]==23,"each player ranks its own nearest NPCs; the guard 5 m from the host has rank 0");
     require(npcRelevanceDisplaces(pr[24],pr[23])&&!npcRelevanceDisplaces(pr[24],pr[1]),"the host's guard displaces the client's 24th NPC, not its nearest");
     require(!npcRelevanceDisplaces(5,6)&&npcRelevanceDisplaces(3,6),"two-rank margin against churn");
     require(!npcRelevanceDisplaces(kNpcRelevanceUnranked,kNpcRelevanceUnranked)&&npcRelevanceDisplaces(0,kNpcRelevanceUnranked),"unranked candidates never displace; unranked bound NPCs always give way");
     require(npcRelevanceCivilianRank(0)==8&&npcRelevanceCivilianRank(kNpcRelevanceUnranked)==kNpcRelevanceUnranked,"1214ey: civilians rank 8 places behind");
     require(!npcRelevanceDisplaces(npcRelevanceCivilianRank(2),9)&&npcRelevanceDisplaces(npcRelevanceCivilianRank(0),20),"1214ey: a nearby civilian displaces only a far combat NPC");}
    {// 1214ae: the client reports the body it carries.
     CoopNpcControl k=p;k.h.kind=NpcAuthorityMessage::Carry;k.h.role=1;k.h.targetArm=8;k.h.targetHandle=0x50009;
     std::swap(k.h.sourceSession,k.h.targetSession);std::swap(k.h.sourceEpoch,k.h.targetEpoch);
     require(validNpcControl(k,host,client),"the client reports a carried bound NPC to the host");
     auto x=k;x.h.reserved=1;require(!validNpcControl(x,host,client),"carry carries no variant");
     x=k;x.h.targetArm=0;require(!validNpcControl(x,host,client),"carry names the host channel");}
    {// 1214ad: Life roster for unbound NPCs, either role.
     CoopNpcControl l=p;l.h.kind=NpcAuthorityMessage::Life;l.h.reserved=2;l.h.targetArm=0;l.h.targetHandle=0;l.h.rig=1;l.h.joints=1;l.h.sourceArm=l.h.sourceHandle=0x50009;
     require(validNpcControl(l,client,host),"the host reports an unbound NPC's death");
     auto x=l;x.h.reserved=1;require(validNpcControl(x,client,host),"or its knockout");
     x=l;x.h.reserved=3;require(!validNpcControl(x,client,host),"only unconscious or dead");x.h.reserved=0;require(!validNpcControl(x,client,host),"never alive");
     x=l;x.h.targetArm=8;require(!validNpcControl(x,client,host),"names no channel");
     x=l;x.h.sourceArm=7;require(!validNpcControl(x,client,host),"carries the sender NPC handle only");
     require(npcLifeApplies(0,2)&&npcLifeApplies(1,2)&&npcLifeApplies(0,1)&&!npcLifeApplies(2,1)&&!npcLifeApplies(1,1)&&!npcLifeApplies(0,3),"only a more severe known state applies");}
    {// 1214n: the host tells the client its NPC died outside the shared combat outcome.
     CoopNpcControl d=p;d.h.kind=NpcAuthorityMessage::Dead;d.h.targetArm=8;d.h.targetHandle=0x50009;
     require(validNpcControl(d,client,host),"host reports an unshared death to the client");
     d.h.reserved=5;require(!validNpcControl(d,client,host),"dead carries no variant");d.h.reserved=0;
     d.h.targetHandle=0;require(!validNpcControl(d,client,host),"dead names the client replica");d.h.targetHandle=0x50009;
     d.h.role=1;require(!validNpcControl(d,host,client),"the client never reports a death");
     {CoopNpcControl a=d;a.h.role=1;a.h.sourceSession=22;a.h.targetSession=11;a.h.sourceEpoch=5;a.h.targetEpoch=3;
      require(!validNpcControl(a,host,client),"1214fl: without ASH1 the client still never reports a death");
      a.h.reserved=kNpcDeadAshed;require(validNpcControl(a,host,client),"1214fl: the client reports its copy turned to ash");
      a.h.reserved=7;require(!validNpcControl(a,host,client),"1214fl: a client Dead carries ASH1 or nothing");}
     d.h.role=0;d.h.reserved=kNpcDeadAshed;require(validNpcControl(d,client,host),"1214fl: the host reports its body turned to ash");d.h.role=1;d.h.reserved=0;
     require(!npcDeadReconcileDue(true,true,true,1000,2400,0,0,0),"dead waits out the shared-death grace");
     require(npcDeadReconcileDue(true,true,true,1000,2500,0,0,0),"an unshared host corpse is reconciled");
     require(!npcDeadReconcileDue(true,true,true,1000,2500,1,0,0)&&!npcDeadReconcileDue(true,true,true,1000,2500,2,0,0),"a pending or committed combat death is never reconciled");
     require(!npcDeadReconcileDue(false,true,true,1000,2500,0,0,0),"only the host reconciles");
     require(!npcDeadReconcileDue(true,true,true,1000,2700,0,1,2500)&&npcDeadReconcileDue(true,true,true,1000,3000,0,1,2500),"repeats are spaced");
     require(!npcDeadReconcileDue(true,true,true,1000,9000,0,5,2500),"at most five sends");
     require(!npcDeadReconcileDue(true,true,true,0,9000,0,0,0)&&!npcDeadReconcileDue(true,false,true,1000,9000,0,0,0),"needs a timed corpse");}
    CoopNpcPose pose{};pose.h=p.h;pose.h.kind=NpcAuthorityMessage::Pose;pose.h.size=static_cast<unsigned short>(npcPoseBytes(pose.h.joints));pose.h.targetArm=8;pose.h.targetHandle=0x50009;
    pose.sampleUs=1000000;pose.health=pose.maximum=50;pose.root[3]=pose.root[7]=pose.root[11]=1;
    for(unsigned j=0;j<pose.h.joints;++j)pose.local[j][0]=pose.local[j][4]=pose.local[j][8]=1;
    require(validNpcPose(pose,client,host),"host sends a224-joint50HP actor to a different client handle");
    {auto down=pose;down.h.reserved=1;require(validNpcPose(down,client,host),"1214en: a host pose may say the NPC is down");
     down.h.reserved=2;require(!validNpcPose(down,client,host),"1214en: no other pose flag");}
    {auto corpse=pose;corpse.health=0;require(validNpcPose(corpse,client,host),"corpse pose carries health0 after a shared death (1113)");corpse.health=-1;require(!validNpcPose(corpse,client,host),"negative health refused");}
    {auto original=pose;const float angle=.713f;original.local[37][0]=original.local[37][4]=std::cos(angle);original.local[37][1]=std::sin(angle);original.local[37][3]=-std::sin(angle);original.local[37][9]=-.123456f;
        NpcPoseWire wire{};CoopNpcPose decoded{};require(encodeNpcPose(original,wire)&&decodeNpcPose(wire.data(),original.h.size,decoded)&&validNpcPose(decoded,client,host),"compact full rig round-trips as a valid native pose");
        float error=0;for(unsigned j=0;j<original.h.joints;++j)for(unsigned i=0;i<12;++i)error=(std::max)(error,std::fabs(original.local[j][i]-decoded.local[j][i]));
        require(error<.0001f&&original.h.size==3312,"compact224-joint wire is3312bytes with sub0.1mm translation error");
        require(!decodeNpcPose(wire.data(),original.h.size-1,decoded)&&!decodeNpcPose(nullptr,original.h.size,decoded),"truncated or absent compact payload refused");
        auto bad=wire;std::memset(bad.data()+176,0,8);require(!decodeNpcPose(bad.data(),original.h.size,decoded),"zero encoded quaternion refused");
        bad=wire;const std::int16_t outOfRange=32767;std::memcpy(bad.data()+176+8,&outOfRange,2);require(!decodeNpcPose(bad.data(),original.h.size,decoded),"out-of-range encoded joint translation refused");
    }
    auto b=pose;b.h.role=1;require(!validNpcPose(b,client,host),"client cannot send host-authoritative body poses");
    {// 1213x: the client's paired victim pose, validated on the host.
     auto v=pose;v.h.kind=NpcAuthorityMessage::PairedPose;v.h.role=1;v.h.sourceSession=22;v.h.targetSession=11;v.h.sourceEpoch=5;v.h.targetEpoch=3;
     require(validNpcPose(v,host,client),"client streams its paired victim pose to the host");
     v.h.role=0;v.h.sourceSession=11;v.h.targetSession=22;v.h.sourceEpoch=3;v.h.targetEpoch=5;require(!validNpcPose(v,client,host),"the host never sends a paired victim pose");
     NpcPoseWire wire{};CoopNpcPose decoded{};auto w=pose;w.h.kind=NpcAuthorityMessage::PairedPose;require(encodeNpcPose(w,wire)&&decodeNpcPose(wire.data(),w.h.size,decoded)&&decoded.h.kind==NpcAuthorityMessage::PairedPose,"paired victim pose round-trips the compact wire");}
    b=pose;--b.h.size;require(!validNpcPose(b,client,host),"wire length must match joint count");
    b=pose;b.local[223][0]=-1;require(!validNpcPose(b,client,host),"reflected body joint refused");
    b=pose;b.local[223][10]=NAN;require(!validNpcPose(b,client,host),"nonfinite body joint refused");
    // 1113: pose health is descriptive and never applied; death comes only from the native
    // combat outcome. Corpse poses after a shared death may carry 0, never a negative value.
    b=pose;b.health=-1;require(!validNpcPose(b,client,host),"pose packet cannot carry negative health");
    auto paused=client;paused.gameLogicState=2;require(!validNpcPose(pose,paused,host),"menu cannot apply world bodies");
    NpcPoseBuffer timeline;require(timeline.push(pose,2000.),"first NPC sample clock anchored");
    auto next=pose;next.h.sequence=2;next.sampleUs+=100000;next.root[0]=1;next.local[17][9]=.1f;
    require(timeline.push(next,2100.),"second source-timed full-body sample accepted");
    CoopNpcPose rendered{};require(timeline.sample(2130.,rendered)&&std::fabs(rendered.root[0]-.5f)<.0001f&&std::fabs(rendered.local[17][9]-.05f)<.0001f,"root and body share80ms delayed interpolation");
    require(!timeline.push(pose,2140.),"old sample cannot rewind body");
    next.h.sequence=3;next.sampleUs+=100000;next.root[0]=10;
    require(timeline.push(next,2200.)&&timeline.count==1&&timeline.sample(2201.,rendered)&&rendered.root[0]==10,"teleport clears interpolation instead of sweeping NPC through geometry");
    require(!timeline.sample(2600.,rendered),"stale body is not extrapolated indefinitely");
    {CoopNpcPose frozen{};require(timeline.latest(frozen)&&frozen.root[0]==10&&frozen.h.sequence==3,"1214fj: the newest pose stays readable after the 350 ms window (frozen host corpse)");}
    timeline.resume();require(!timeline.sample(2601.,rendered)&&!timeline.push(next,2601.),"menu resume clears prior interpolation while rejecting replay of the last source pose");
    ++next.h.sequence;next.sampleUs+=6000000;require(timeline.push(next,8200.)&&timeline.sample(8201.,rendered),"fresh post-menu source pose anchors resumed body clock");
    const auto session=doorSessionScope(host,client);auto menuHost=host;menuHost.gameLogicState=2;
    require(doorSessionCanRetainMenu(session,menuHost,client)&&!validNpcPose(pose,client,menuHost),"retained NPC session never permits pose mutation in a peer menu");
    auto newHost=host,newClient=client;++newHost.worldEpoch;
    require(!doorSessionRenewalReady(session,newHost,newClient),"NPC checkpoint renewal requires both new worlds");++newClient.worldEpoch;
    require(doorSessionRenewalReady(session,newHost,newClient)&&!validNpcPose(pose,newClient,newHost),"coordinated new worlds can rebind but old body envelopes stay invalid");
    NativeSkeleton skeleton{};std::strcpy(skeleton.modelName,"model");std::strcpy(skeleton.skeletonName,"native_skeleton");
    skeleton.parents={-1,0};skeleton.bindLocal.resize(2);skeleton.localPose.resize(2);
    for(auto& q:skeleton.bindLocal){q[3]=1;q[8]=q[9]=q[10]=1;}
    const auto rig=npcAuthorityRig(skeleton);require(rig!=0,"bounded arbitrary native rig hashed");skeleton.jointIds={123,456};require(npcAuthorityRig(skeleton)==rig,"process-local name intern IDs excluded from wire hash");
    skeleton.bindLocal[1][4]=.1f;require(npcAuthorityRig(skeleton)!=rig,"different bind layout cannot share a rig binding");
    std::cout<<"PASS: generic NPC Steam scope, rig/variable-length body validation, source clock, interpolation and replay rejection\n";
}

#pragma once
#include "usable_protocol.h"
#include "usable_catalog.h"
inline void usableProtocolTests(){
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.session=11;c.session=22;h.worldEpoch=3;c.worldEpoch=5;
    h.mapId=c.mapId=9;h.flags=c.flags=1;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    CoopUsablePacket p{};p.sequence=1;p.sourceSession=11;p.targetSession=22;p.sourceEpoch=3;p.targetEpoch=5;p.mapId=9;p.sourceArm=7;
    p.stateCount=2;p.rulesHash=123;std::strcpy(p.name,"window_1");std::strcpy(p.definition,"definition_1");
    require(validCoopUsable(p,c,h),"usable hello validates semantic descriptor and paired world");
    p.kind=UsableMessage::State;p.targetArm=8;p.revision=1;p.target=1;
    require(validCoopUsable(p,c,h),"host owns native indexed desired state");
    for(unsigned field=0;field<17;++field){auto bad=p;switch(field){
        case 0:++bad.sourceSession;break;case 1:++bad.targetSession;break;case 2:++bad.sourceEpoch;break;case 3:++bad.targetEpoch;break;
        case 4:++bad.mapId;break;case 5:bad.target=2;break;case 6:bad.stateCount=33;break;case 7:bad.stateCount=0;break;
        case 8:bad.rulesHash=0;break;case 9:bad.sourceArm=0;break;case 10:bad.targetArm=0;break;case 11:bad.revision=0;break;
        case 12:bad.role=1;break;case 13:bad.kind=UsableMessage::Request;break;case 14:bad.reserved=1;break;
        case 15:bad.name[191]='x';break;case 16:bad.magic=0x4f443244;break;}
        require(!validCoopUsable(bad,c,h),"malformed, foreign, stale-world or wrong-authority usable packet rejected");
    }
    p.role=1;p.sourceSession=22;p.targetSession=11;p.sourceEpoch=5;p.targetEpoch=3;p.kind=UsableMessage::Request;
    require(validCoopUsable(p,h,c),"client can request an indexed state");
    p.requestAck=1;require(!validCoopUsable(p,h,c),"client cannot acknowledge its own request as host");p.requestAck=0;
    auto paused=h;paused.gameLogicState=2;require(!validCoopUsable(p,paused,c),"native menu cannot apply indexed state");
    UsableOrder order;require(order.set(0)&&!order.set(0)&&order.revision==1,"duplicate desired state is not a toggle");
    require(order.set(1)&&order.set(0)&&order.revision==3,"host serializes opposite indexed requests");
    require(!order.accept(2,1)&&order.target==0,"old revision cannot reopen a window");
    require(order.accept(4,1)&&!order.accept(4,1)&&!order.set(32),"monotonic bounded indexed order");
    NativeUsable n{};n.name=n.definitionName="ld_rich_small_01_sndctr_4";n.declarationName="map_cpntusablewithstatelist_decl_4";
    n.stateCount=2;n.initial=0;n.current=0;n.pending=-1;n.enabled=1;n.entries[0].exit=0;n.entries[1].exit=1;
    require(nativeUsableToggleFamily(n),"observed native window rules select a family member");
    const auto hash=nativeUsableRulesHash(n);auto other=n;other.actor=100;other.current=1;other.pending=0;other.generation=44;
    require(hash==nativeUsableRulesHash(other),"descriptor hash ignores local pointers, generations and mutable states");
    other.entries[1].exit=2;require(hash!=nativeUsableRulesHash(other)&&!nativeUsableToggleFamily(other),"different animation entry changes descriptor and eligibility");
    other=n;other.entries[1].customPrompt=1;require(!nativeUsableToggleFamily(other),"quest/custom-prompt state list requires its own semantics");
    other=n;other.name=other.definitionName="ld_rich_small_01_sndctr_5";
    auto cat=selectNativeUsableCatalog({other,n});require(cat.entries.size()==2&&cat.entries[0].name==n.name,"all matching authored windows share one family adapter");
    cat=selectNativeUsableCatalog({other,n,n});require(cat.entries.size()==1&&cat.ambiguous==2,"ambiguous authored identities never get a network binding");
    other.flags=2;cat=selectNativeUsableCatalog({other,n});require(cat.entries.size()==1&&cat.ineligible==1,"moving window cannot seed a stable catalog baseline");
    other=n;other.enabled=0;require(!nativeUsableToggleFamily(other),"disabled use remains disabled");
    other=n;other.name=other.definitionName="esc_env_cupboard_01_28";other.declarationName="map_cpntusablewithstatelist_decl_14";other.entries[0].customPrompt=other.entries[1].customPrompt=1;
    require(nativeUsableToggleFamily(other),"uniform Open/Close prompt toggles on any map join the family");
    other.name=other.definitionName="esc_env_safe_door_4";require(!nativeUsableToggleFamily(other),"vault doors keep their own semantics");
    other=n;other.flags=1;require(!nativeUsableToggleFamily(other),"flagged quest devices keep their own semantics");
    other=n;other.stateCount=5;require(!nativeUsableToggleFamily(other),"multi-state devices are not toggles");
    other=n;other.entries[0].exit=1;other.entries[1].exit=0;require(nativeUsableToggleFamily(other)&&nativeUsableRulesHash(other)!=hash,"uniform reversed toggles join with a distinct descriptor");
    other.entries[1].exit=1;require(!nativeUsableToggleFamily(other),"non-permutation exits are refused");
    std::cout<<"PASS: native state-list catalog, semantic descriptors, authority, world/replay scope and indexed desired states\n";
}

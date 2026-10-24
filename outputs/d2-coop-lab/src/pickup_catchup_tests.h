#pragma once
#include "pickup_catchup.h"
inline void pickupCatchUpTests(){
    CoopPosePacket host{},client{};host.role=0;client.role=1;host.session=11;client.session=22;host.worldEpoch=3;client.worldEpoch=5;
    host.mapId=client.mapId=9;host.flags=client.flags=1;host.gameLogicState=client.gameLogicState=1;host.cinematicMetadataValid=client.cinematicMetadataValid=1;
    // route-resumed-join-v1875a: handles are (entity slot << 16) | salt.
    const std::vector<PickupPresenceItem> world{{"pickups_coin1_lowvalue_7",0x0d000101u,{}},{"pickups_mana_potion_01_17",0x0e000202u,{}},
        {"ammo_bolt_pack_regular_12358",(12358u<<16)|5u,{0,0,0}},{"outsider_rune_12363",(12363u<<16)|9u,{-48.3f,93.9f,22.2f}},{"pickups_news_kg_ontherun_corvo",0x0f000303u,{}}};
    const auto entries=pickupPresenceEntries(world);
    PickupPresencePacket p{};p.header.role=0;p.header.sequence=1;p.header.sourceSession=11;p.header.targetSession=22;p.header.sourceEpoch=3;p.header.targetEpoch=5;p.header.mapId=9;
    p.header.count=static_cast<std::uint32_t>(entries.size());std::copy(entries.begin(),entries.end(),p.entries);
    const auto bytes=pickupPresenceBytes(p.header.count);
    require(entries.size()==5&&bytes==64+5*32,"one presence entry per name, only the entries in use travel");
    require(validPickupPresence(p,bytes,client,host),"the host's pickup presence reaches the partner");
    require(!validPickupPresence(p,bytes,host,client),"a partner's presence never reaches the host");
    require(!validPickupPresence(p,bytes+32,client,host)&&!validPickupPresence(p,bytes-1,client,host),"the byte count must match the entries");
    for(unsigned field=0;field<10;++field){auto bad=p;switch(field){case 0:bad.header.magic=1;break;case 1:bad.header.sourceSession++;break;case 2:bad.header.targetEpoch++;break;
        case 3:bad.header.mapId++;break;case 4:bad.header.version=1;break;case 5:bad.entries[1]=bad.entries[0];break;case 6:bad.entries[0].reserved=1;break;
        case 7:bad.header.sequence=0;break;case 8:bad.entries[0].name=0;break;case 9:for(auto& e:bad.entries)if(!e.base){e.position[0]=1;break;}break;}
        require(!validPickupPresence(bad,bytes,client,host),"malformed or out-of-scope presence rejected");}
    require(pickupRuntimeNamed("amber_01_12151",(12151u<<16)|11162u)&&!pickupRuntimeNamed("pickups_fruit_banana_02_6",(8072u<<16)|7074u)&&
        !pickupRuntimeNamed("pickups_coin1_lowvalue_7",0x0d000101u)&&!pickupRuntimeNamed("pickups",1u<<16),"runtime names end with their own entity slot");
    // The partner's world, loaded from the mission start.
    const PickupPresenceItem coin{"pickups_coin1_lowvalue_7",0x0d000101u,{}},banana{"pickups_fruit_banana_02_6",(8072u<<16)|7074u,{}},
        bolts{"ammo_bolt_pack_regular_12140",(12140u<<16)|4u,{0,0,0}},rune{"outsider_rune_12364",(12364u<<16)|8u,{-48.3f,93.9f,22.2f}},
        farRune{"outsider_rune_12364",(12364u<<16)|8u,{-40.f,93.9f,22.2f}},pouch{"pouch_small_03_12130",(12130u<<16)|3u,{3.9f,23.1f,19.f}},
        emilyNews{"pickups_news_kg_ontherun_emily",0x0f000303u,{}};
    require(!pickupCatchUpTarget(coin,entries),"a pickup present in the host's world, active or dormant, stays");
    require(pickupCatchUpTarget(banana,entries),"an authored pickup the host's world no longer has is caught up");
    require(!pickupCatchUpTarget(bolts,entries)&&!pickupCatchUpTarget(rune,entries),"runtime pickups match by base name within 3 m");
    require(pickupCatchUpTarget(farRune,entries)&&pickupCatchUpTarget(pouch,entries),"a runtime pickup with no namesake nearby is caught up");
    require(!pickupCatchUpTarget(emilyNews,entries)&&!pickupCatchUpTarget(emilyNews,{}),"character variants are never caught up");
    require(pickupNameHash("")!=0&&pickupNameHash("a")!=pickupNameHash("b"),"name hashes are nonzero and distinct");
    std::cout<<"PASS: pickup presence packets and the join catch-up targets\n";
}

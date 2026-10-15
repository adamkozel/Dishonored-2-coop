#pragma once
#include "npc_combat_protocol.h"
#include "native_trigger_identity_tests.h"
void npcCombatProtocolTests(){
    require(npcCombatChannelBlocksRebind(true,false,false,0,9000)&&npcCombatChannelBlocksRebind(false,true,false,1000,9000)&&npcCombatChannelBlocksRebind(false,false,true,1000,9000)&&
            npcCombatChannelBlocksRebind(false,false,false,8000,9000)&&!npcCombatChannelBlocksRebind(false,false,false,7000,9000),"a retired living NPC re-binds after 2 s; deaths and live channels keep blocking (1258d)");
    nativeTriggerIdentityTests();
    require(npcCombatNativeLethal(24.8662033f,24.8662033f)&&!npcCombatNativeLethal(40,15.1337957f),"native final amount, not base weapon amount, determines ownership retirement");
    require(npcCombatNativeLethal(1,0.6f)&&!npcCombatNativeLethal(1,0.5f)&&!npcCombatNativeLethal(40,NAN),"native half-health death threshold and finite values enforced");
    require(npcCombatClose(40.f-24.8662033f,15.1337957f)&&!npcCombatClose(15,15.01f)&&!npcCombatClose(NAN,0),"fractional native hit survives float subtraction rounding but real divergence is rejected");
    require(npcCombatReplicaWorldEvent(1,false)&&!npcCombatReplicaWorldEvent(1,true)&&!npcCombatReplicaWorldEvent(0,false)&&!npcCombatReplicaWorldEvent(0,true),"client world callbacks cannot request or duplicate authoritative damage; owned local attacks still enter validation");
    NpcCombatPendingRequests pending;
    require(!pending.push(0)&&pending.push(1)&&pending.push(2)&&pending.front()==1&&pending.count==2,"compound native contacts retain both requests before a reply");
    require(!pending.acknowledge(2)&&pending.count==2&&pending.acknowledge(1)&&pending.front()==2,"out of order response cannot remove the first request");
    require(!pending.acknowledge(1)&&pending.acknowledge(2)&&!pending.front(),"duplicate acknowledgement does not consume the following request");
    for(std::uint64_t id=3;id<=34;++id)require(pending.push(id),"bounded pending request ring accepts capacity across wrap");
    require(!pending.push(35)&&pending.count==32&&pending.front()==3,"overflow preserves all unacknowledged native contacts");
    for(std::uint64_t id=3;id<=34;++id)require(pending.acknowledge(id),"wrapped requests drain in their original order");
    require(pending.push(35)&&!pending.push(37)&&pending.count==1,"gapped native request counter rejected");
    pending.clear();require(!pending.count&&!pending.front()&&!pending.acknowledge(35),"terminal or new-world cleanup removes old pending contacts");
    require(npcStreamPoseStale(2000,100,1000)&&!npcStreamPeerExpired(2000,100,1950,true),"missing pose expires presentation but fresh reliable heartbeat retains gameplay authority");
    require(!npcStreamPoseStale(2020,100,2010)&&!npcStreamPeerExpired(2020,100,2010,true),"fresh pose recovers without changing actor incarnation");
    require(npcStreamPeerExpired(4000,100,1950,true),"pose gap cannot retain authority after reliable peer also times out");
    require(npcStreamPeerExpired(5200,100,0,false)&&!npcStreamPeerExpired(100,101,0,false),"unbound negotiation is bounded and old frame tick does not underflow");
    CoopPosePacket h{},c{};h.role=0;c.role=1;h.session=11;c.session=22;h.worldEpoch=3;c.worldEpoch=5;h.mapId=c.mapId=9;
    h.flags=c.flags=1;h.gameLogicState=c.gameLogicState=1;h.cinematicMetadataValid=c.cinematicMetadataValid=1;
    CoopNpcCombat p{};auto& b=p.binding;b.kind=NpcAuthorityMessage::Stop;b.size=sizeof(CoopNpcControl);b.sequence=1;
    b.sourceSession=11;b.targetSession=22;b.sourceEpoch=3;b.targetEpoch=5;b.map=9;b.sourceArm=7;b.targetArm=8;b.sourceHandle=0x10003;b.targetHandle=0x50009;b.rig=456;b.joints=223;
    p.sourceNonce=99;p.before=57;p.maximum=73;
    require(validNpcCombat(p,c,h),"combat handshake accepts generic non40HP actor bound to native NPC stream");
    p.kind=NpcCombatMessage::Commit;p.targetNonce=88;p.sourceRole=0;p.event=1;p.after=47;p.inflicted=10;p.damage=NpcCombatDamage::Sword;
    {EnemyMeleeGeometry k{};k.region=1;k.regions=2;k.offset[0]=.1f;k.offset[2]=1.05f;k.direction[0]=.035f;k.direction[1]=-.957f;k.direction[2]=-.287f;
     require(npcCombatGeometry(NpcCombatDamage::Kick,k),"native kick geometry: zero normal, unit direction (1107)");
     auto bad=k;bad.normal[2]=1;require(!npcCombatGeometry(NpcCombatDamage::Kick,bad),"kick geometry refuses a sword-like normal");
     bad=k;bad.region=2;bad.regions=4;require(!npcCombatGeometry(NpcCombatDamage::Kick,bad),"kick geometry refuses other regions");}
    p.geometry.region=5;p.geometry.regions=32;p.geometry.normal[1]=1;p.geometry.direction[0]=1;
    require(validNpcCombat(p,c,h),"host native result can carry sequential arbitrary living health values");
    auto ranged=p;ranged.damage=NpcCombatDamage::RangedSpread;ranged.geometry.region=1;ranged.geometry.regions=255;
    ranged.geometry.normal[1]=0;ranged.ranged={npcDamageKey("components/base/damage/gun_spread_damage.damage"),15.1337957f,3.7f,1};
    ranged.before=40;ranged.maximum=40;ranged.after=24.8662033f;ranged.inflicted=15.1337957f;
    require(validNpcCombat(ranged,c,h),"native spread preserves variable base amount, zero normal and multiple hit regions");
    auto subset=ranged;subset.geometry.regions=254;require(validNpcCombat(subset,c,h),"native spread may hit a subset of body regions as observed in compound multi-victim shot945");
    auto bullet=ranged;bullet.damage=NpcCombatDamage::RangedContact;bullet.geometry.region=2;bullet.geometry.regions=4;bullet.geometry.normal[1]=1;
    bullet.ranged={npcDamageKey("components/equipment/damage/bullet_regular.damage"),20,0,1};bullet.before=ranged.after;bullet.after=0;bullet.inflicted=bullet.before;
    require(validNpcCombat(bullet,c,h),"native bullet can produce a lethal region-modified outcome after spread");
    for(unsigned n=0;n<9;++n){auto bad=ranged;switch(n){case 0:bad.ranged.amount=21;break;case 1:bad.ranged.amount=NAN;break;
        case 2:bad.ranged.weapon=2;break;case 3:bad.ranged.declaration=bullet.ranged.declaration;break;case 4:bad.geometry.normal[1]=1;break;
        case 5:bad.geometry.regions=256;break;case 6:bad.ranged.distance=33;break;case 7:bad.ranged.distance=-1;break;case 8:bad.version=4;break;}
        require(!validNpcCombat(bad,c,h),"ranged descriptor rejects mixed profiles, invalid geometry, amount, distance and old wire");}
    auto mixed=p;mixed.ranged=ranged.ranged;require(!validNpcCombat(mixed,c,h),"melee cannot carry hidden ranged parameters");
    auto bolt=bullet;bolt.ranged={npcDamageKey("components/equipment/damage/bolt.damage"),15,0,2};bolt.before=40;bolt.after=25;bolt.inflicted=15;
    require(validNpcCombat(bolt,c,h),"observed native bolt uses shared contact geometry and nonterminal outcome");
    bolt.after=0;bolt.inflicted=40;require(validNpcCombat(bolt,c,h),"native region modifier may make a bolt terminal without changing the base amount");
    for(unsigned n=0;n<6;++n){auto bad=bolt;switch(n){case 0:bad.ranged.weapon=1;break;case 1:bad.ranged.amount=16;break;
        case 2:bad.ranged.declaration=bullet.ranged.declaration;break;case 3:bad.damage=NpcCombatDamage::RangedSpread;break;
        case 4:bad.version=5;break;case 5:bad.ranged.amount=0;break;}
        require(!validNpcCombat(bad,c,h),"bolt profile rejects mixed weapon/declaration, invalid amount and older pistol-only peers");}
    auto world=p;world.sourceRole=2;world.damage=NpcCombatDamage::Corpse;world.sourceActorArm=16;world.after=56;world.inflicted=1;world.geometry.region=1;world.geometry.regions=0;
    require(validNpcCombat(world,c,h),"host corpse contact carries a bound source incarnation, native geometry and ordered damage");
    auto script=world;script.damage=NpcCombatDamage::MapTrigger;script.sourceActorArm=0;script.geometry={};
    script.script={npcDamageKey("authored_trigger"),npcDamageKey("components/equipment/damage/bolt_incendiary.damage"),1,0};
    require(validNpcCombat(script,c,h),"host map-trigger outcome carries stable source/declaration keys and bounded dynamic amount");
    auto scriptDeath=script;scriptDeath.geometry=world.geometry;scriptDeath.after=0;scriptDeath.inflicted=scriptDeath.before;
    scriptDeath.script.declaration=npcDamageKey("components/base/damage/incendiary_death.damage");scriptDeath.script.amount=0;
    require(validNpcCombat(scriptDeath,c,h),"native effect continuation may have contact geometry and terminal outcome with zero base damage");
    auto badGeometry=scriptDeath;badGeometry.geometry.offset[0]=100;require(!validNpcCombat(badGeometry,c,h),"script contact cannot smuggle an unbounded hit offset");
    badGeometry=scriptDeath;badGeometry.geometry.normal[0]=NAN;require(!validNpcCombat(badGeometry,c,h),"script contact cannot contain nonfinite geometry");
    auto badScript=script;badScript.script.declaration=123;require(!validNpcCombat(badScript,c,h),"unknown damage declaration cannot request native lookup");
    badScript=script;badScript.script.amount=NAN;require(!validNpcCombat(badScript,c,h),"nonfinite native amount refused");
    badScript=script;badScript.script.reserved=1;require(!validNpcCombat(badScript,c,h),"noncanonical descriptor refused");
    badScript=script;badScript.sourceActorArm=1;require(!validNpcCombat(badScript,c,h),"map trigger cannot smuggle corpse identity");
    badScript=script;badScript.damage=NpcCombatDamage::Sword;badScript.sourceRole=0;badScript.geometry=p.geometry;require(!validNpcCombat(badScript,c,h),"player action cannot carry scripted damage");
    badScript=script;badScript.kind=NpcCombatMessage::Request;badScript.request=1;badScript.event=0;badScript.before=badScript.after=badScript.maximum=badScript.inflicted=0;require(!validNpcCombat(badScript,h,c),"client cannot submit a map-trigger event");
    badScript=script;badScript.version=3;require(!validNpcCombat(badScript,c,h),"old corpse-only wire cannot be mistaken for descriptor packet");
    auto noDamage=world;noDamage.after=noDamage.before;noDamage.inflicted=0;require(validNpcCombat(noDamage,c,h),"host-owned harmless corpse result can preserve health without inventing damage");
    {// 1214o: host-world damage (creature or NPC source) follows the host's native loss.
     auto hw=world;hw.damage=NpcCombatDamage::HostWorld;hw.sourceActorArm=0;hw.geometry={};
     require(validNpcCombat(hw,c,h),"host-world loss is an ordered host commit without geometry or source identity");
     auto bad=hw;bad.after=0;bad.inflicted=bad.before;require(!validNpcCombat(bad,c,h),"a host-world loss is never lethal");
     bad=hw;bad.after=bad.before;bad.inflicted=0;require(!validNpcCombat(bad,c,h),"a host-world commit always removes health");
     bad=hw;bad.geometry=world.geometry;require(!validNpcCombat(bad,c,h),"host-world loss carries no contact geometry");
     bad=hw;bad.sourceActorArm=16;require(!validNpcCombat(bad,c,h),"host-world loss carries no source identity");
     bad=hw;bad.sourceRole=0;require(!validNpcCombat(bad,c,h),"host-world loss is never a player action");
     bad=hw;bad.kind=NpcCombatMessage::Request;bad.request=1;bad.event=0;bad.before=bad.after=bad.maximum=bad.inflicted=0;bad.sourceRole=1;require(!validNpcCombat(bad,h,c),"the client cannot request host-world damage");
     bad=hw;bad.inflicted=2;require(!validNpcCombat(bad,c,h),"host-world loss conserves health");
     require(npcCombatHostWorldSource(0,0x2dcd29e9u,0x2dcd29e9u,false,false),"creature-sourced damage on the host passes natively");
     require(!npcCombatHostWorldSource(1,0x2dcd29e9u,0x2dcd29e9u,false,false),"the client never passes replica damage");
     require(!npcCombatHostWorldSource(0,1,0x1234,false,true)&&!npcCombatHostWorldSource(0,0x2dcd29e9u,0x1234,false,true)&&!npcCombatHostWorldSource(0,0x2dcd29e9u,0x1234,true,false),"player-owned damage stays captured");
     require(!npcCombatHostWorldSource(0,0x3fff0010u,7,false,false)&&!npcCombatHostWorldSource(0,7,0x3fff0010u,false,false),"world sentinels (corpse contact, map triggers) stay captured");
     require(npcCombatHelloHealth(1,30,20)==1&&npcCombatHelloHealth(0,20,30)==2&&npcCombatHelloHealth(0,30,30)==0&&npcCombatHelloHealth(1,30,30)==0,"the client follows a lower host health; the host waits for it");
     require(npcCombatHelloHealth(1,20,30)==-1&&npcCombatHelloHealth(0,30,20)==-1&&npcCombatHelloHealth(1,30,0)==-1,"a replica never gains health and never dies by handshake");}
    auto badWorld=world;badWorld.sourceActorArm=0;require(!validNpcCombat(badWorld,c,h),"unattributed world collision rejected");
    badWorld=world;badWorld.sourceRole=1;require(!validNpcCombat(badWorld,c,h),"corpse cannot be disguised as player damage");
    badWorld=world;badWorld.kind=NpcCombatMessage::Request;badWorld.request=1;badWorld.event=0;badWorld.before=badWorld.after=badWorld.maximum=badWorld.inflicted=0;require(!validNpcCombat(badWorld,h,c),"client cannot request host-owned world damage");
    badWorld=p;badWorld.sourceActorArm=16;require(!validNpcCombat(badWorld,c,h),"player damage cannot smuggle a world-source incarnation");
    badWorld=world;badWorld.after=world.before+1;badWorld.inflicted=-1;require(!validNpcCombat(badWorld,c,h),"world damage cannot heal or prescribe negative damage");
    badWorld=world;badWorld.version=2;require(!validNpcCombat(badWorld,c,h),"pre-world-damage wire version rejected");
    for(unsigned i=0;i<18;++i){auto q=p;switch(i){case 0:q.binding.role=1;break;case 1:++q.binding.sourceEpoch;break;case 2:++q.binding.targetEpoch;break;
        case 3:++q.binding.sourceSession;break;case 4:++q.binding.targetSession;break;case 5:++q.binding.map;break;case 6:q.targetNonce=0;break;case 7:q.sourceNonce=0;break;
        case 8:q.event=0;break;case 9:q.after=0;break;case 10:q.inflicted=11;break;case 11:q.maximum=46;break;case 12:q.geometry.region=32;break;
        case 13:q.geometry.regions=1;break;case 14:q.geometry.direction[0]=NAN;break;case 15:q.request=1;break;case 16:q.sourceRole=2;break;case 17:--q.size;break;}
        require(!validNpcCombat(q,c,h),"malformed, stale, lethal or nonhost result rejected");}
    auto fatal=p;fatal.damage=NpcCombatDamage::Fatality;fatal.after=0;fatal.inflicted=fatal.before;
    require(validNpcCombat(fatal,c,h),"native fatality result permits zero health only with full native inflicted outcome");
    auto assassin=fatal;assassin.damage=NpcCombatDamage::Assassination;assassin.geometry={};
    require(validNpcCombat(assassin,c,h),"assassination carries no fabricated hit geometry");
    auto invalid=assassin;invalid.geometry.direction[0]=1;require(!validNpcCombat(invalid,c,h),"assassination does not borrow ordinary geometry");
    invalid=fatal;invalid.after=20;invalid.inflicted=37;require(!validNpcCombat(invalid,c,h),"instant kill cannot carry a nonterminal health outcome");
    invalid=fatal;invalid.damage=NpcCombatDamage::None;require(!validNpcCombat(invalid,c,h),"unknown damage kind rejected");
    invalid=fatal;invalid.version=1;require(!validNpcCombat(invalid,c,h),"older nonfatal protocol cannot be interpreted as lethal protocol");
    auto lastSword=p;lastSword.before=7;lastSword.after=0;lastSword.inflicted=7;require(validNpcCombat(lastSword,c,h),"ordinary sword can deliver native last hit");
    lastSword.inflicted=6;require(!validNpcCombat(lastSword,c,h),"zero health cannot hide insufficient native damage");
    require(npcCombatOrder(0,1)==NpcCombatOrder::Next&&npcCombatOrder(1,1)==NpcCombatOrder::Duplicate&&npcCombatOrder(1,3)==NpcCombatOrder::Gap,"exactly once stream distinguishes duplicate from missing ordered outcome");
    std::uint64_t applied=0;for(auto event:{1ull,1ull,2ull,2ull,3ull})if(npcCombatOrder(applied,event)==NpcCombatOrder::Next)applied=event;
    require(applied==3,"repeated hits advance and repeated packets cannot advance twice");
    auto paused=c;paused.gameLogicState=2;require(!validNpcCombat(p,paused,h),"native menu cannot mutate combat");
    p.kind=NpcCombatMessage::Request;p.binding.role=1;std::swap(p.binding.sourceSession,p.binding.targetSession);std::swap(p.binding.sourceEpoch,p.binding.targetEpoch);
    std::swap(p.binding.sourceArm,p.binding.targetArm);std::swap(p.binding.sourceHandle,p.binding.targetHandle);p.sourceRole=1;p.request=1;p.event=0;p.before=p.after=p.maximum=p.inflicted=0;
    require(validNpcCombat(p,h,c),"client submits geometry and request ID without prescribing authoritative health");
    p.after=10;require(!validNpcCombat(p,h,c),"client cannot prescribe result health");
    std::cout<<"PASS: NPC combat authority, exact incarnation, ordered repeated hits, duplicate and malformed result rejection\n";
}

"""Bind only v266-observed Swarm phases; keep world effects out of pose packets."""
from pathlib import Path
P=Path(__file__).resolve().parents[2];S=P/'outputs/d2-coop-lab/src'
def edit(name,old,new):
 p=S/name;s=p.read_text();assert old in s,(name,old[:100]);p.write_text(s.replace(old,new))
s=(S/'native_player_domino_scene.h').read_text()
s=s.replace('domino','swarm').replace('Domino','Swarm')
start=s.index('// Observed');end=s.index('template<class Read>')
s=s[:start]+'''// v266: exact native standing, armed Corvo input. Body and left arm are
// reciprocal bindings; UsePower63 retires during the 1600ms release scene.
// No rat spawn, damage or animation events are executed by this reader.
inline unsigned nativeSwarmVariant(const std::string& asset,const std::string& sub,unsigned duration){
    if(sub!="Scene_1")return 0;
    const std::string prefix="midnight/gameplay/player/base/powers/power_swarm_cast_";
    if(asset==prefix+"in.midnightscene"&&duration==400)return 23;
    if(asset==prefix+"loop.midnightscene"&&duration==333)return 24;
    if(asset==prefix+"out.midnightscene"&&duration==1600)return 25;
    return 0;
}
''' +s[end:]
s=s.replace('states.player.current>1','states.player.current!=0')
s=s.replace('!item.instantiated||!item.started||','!item.instantiated||!item.started||!item.player.sword.equipped||')
s=s.replace('if((states.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||\n           (states.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;', 'if(a.id==0&&a.vtable==base+0x1c85628)++ground;')
s=s.replace('a.id==72&&a.vtable==base+0x1c3eeb8','a.id==63&&a.vtable==base+0x1c80ed8')
s=s.replace('if(asset!="midnight/gameplay/player/base/powers/power_phantasmal_cast.midnightscene")continue;', 'if(asset.find("midnight/gameplay/player/base/powers/power_swarm_cast_")!=0)continue;')
start=s.index('    // v239:');end=s.index('       !read(s.instance+0x10',start)
s=s[:start]+'''    // v266 source body returns to Running0 while the owned release scene
    // continues. Entry/aim still require exact UsePower63, and release cannot
    // survive its bounded native clock or any equipment/scene retirement.
    if(!s.instance||(!casting&&s.variant!=25)||!read(s.instance+0x158,&s.handle,4)||!s.handle||!read(s.instance+0x160,&s.timeMs,4)||s.timeMs>3600000||
       (s.variant!=24&&s.timeMs>s.durationMs)||
''' +s[end:]
s=s.replace('s.arm.character!=2','s.arm.character!=1')
(S/'native_player_swarm_scene.h').write_text(s)
for name in ['shadow_stream.h','runtime_probe.cpp','world_identity_tests.cpp']:
 edit(name,'#include "native_player_domino_scene.h"','#include "native_player_domino_scene.h"\n#include "native_player_swarm_scene.h"')
edit('shadow_stream.h','||readNativePlayerDominoScene(memoryRead,base,states,power))','||readNativePlayerDominoScene(memoryRead,base,states,power)||readNativePlayerSwarmScene(memoryRead,base,states,power))')
edit('shadow_action_protocol.h','version=45','version=46');edit('shadow_action_protocol.h','p.version==45','p.version==46')
edit('shadow_action_protocol.h','variant<=22&&variant!=13&&(swordState==2||(swordState==1&&variant>=10))','variant<=25&&variant!=13&&(swordState==2||(swordState==1&&variant>=10&&variant<=22))')
edit('shadow_action_protocol.h','p.powerVariant<=3||(p.powerVariant>=11&&p.powerVariant<=15)','p.powerVariant<=3||(p.powerVariant>=11&&p.powerVariant<=15)||(p.powerVariant>=23&&p.powerVariant<=25)')
edit('power_presentation.h','666,266,1333,400,266};return v<=22','666,266,1333,400,266,400,333,1600};return v<=25')
edit('power_presentation.h','s.variant<=22','s.variant<=25');edit('power_presentation.h','v==17||v==20;','v==17||v==20||v==24;')
edit('movement_bank.h','32+301*','32+304*');edit('movement_bank.h','bool hasDomino()','bool hasSwarm()const{return version>=49;}\n    Joint sampleSwarm(unsigned variant,unsigned joint,float phase)const{return sampleParameter(301+variant-23,joint,phase);}\n    bool hasDomino()')
edit('movement_bank.h','289,301};return v<=48','289,301,304};return v<=49');edit('movement_bank.h','v<=48;++v','v<=49;++v')
edit('movement_playback.h','const bool powerActive=powerValid&&power.variant&&(dominoActive||','const bool swarmActive=power.variant>=23&&power.variant<=25&&p.bank.hasSwarm()&&ground&&p.presentedDrawn;\n        const bool powerActive=powerValid&&power.variant&&(swarmActive||dominoActive||')
edit('movement_playback.h','p.powerVariant<=22&&p.bank.hasDomino())))','p.powerVariant<=22&&p.bank.hasDomino())||(p.powerVariant>=23&&p.powerVariant<=25&&p.bank.hasSwarm())))')
edit('movement_playback.h','auto power=p.powerVariant>=19?', 'auto power=p.powerVariant>=23?p.bank.sampleSwarm(p.powerVariant,j,p.powerAmount):p.powerVariant>=19?')
edit('movement_bank_tests.cpp','(argc==1?1:48);format<=48','(argc==1?1:49);format<=49')
edit('movement_bank_tests.cpp','check(bank.hasDomino()==', '''check(bank.hasSwarm()==(bank.version>=49),"Swarm clips require bank49");
        if(bank.hasSwarm())for(unsigned v=23;v<=25;++v)for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float t:{0.f,.5f,1.f}){
            const auto a=bank.sampleSwarm(v,j,t),b=bank.sampleParameter(301+v-23,j,t);
            for(unsigned k=0;k<7;++k)check(std::isfinite(a[k])&&std::fabs(a[k]-b[k])<1e-6f,"Swarm phase stays in its own full-body clip and sword socket");
        }
        check(bank.hasDomino()==''')
# Strict read-only single and rapid watcher use the exact same source reader.
edit('runtime_probe.cpp','(argc==4&&std::strcmp(argv[2],"--watch-domino-links")','(argc==4&&std::strcmp(argv[2],"--watch-swarm-scenes")&&std::strcmp(argv[2],"--watch-domino-links")')
p=S/'runtime_probe.cpp';s=p.read_text();a=s.index('    if(argc==4&&!std::strcmp(argv[2],"--watch-domino-scenes"))');b=s.index('    if(argc==3&&!std::strcmp(argv[2],"--player-swarm"))',a)
s=s[:b]+s[a:b].replace('Domino','Swarm').replace('domino','swarm')+s[b:];p.write_text(s)
edit('shadow_action_protocol_tests.cpp','oldDark.powerVariant=23','oldDark.powerVariant=26')
edit('shadow_action_protocol_tests.cpp','      for(unsigned variant=19;', '''      for(unsigned variant=23;variant<=25;++variant){
        auto swarm=power;swarm.powerVariant=variant;swarm.powerDurationMs=PowerPresentation::duration(variant);
        check(validShadowAction(swarm,c,h)&&shadowPowerSwordContext(variant,2),"native standing armed Corvo Swarm phase accepted by sender and receiver");
        auto bad=swarm;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Emily cannot claim Swarm");
        bad=swarm;bad.swordState=1;check(!validShadowAction(bad,c,h)&&!shadowPowerSwordContext(variant,1),"unobserved holstered Swarm remains refused");
        bad=swarm;bad.airStage=2;check(!validShadowAction(bad,c,h),"unobserved airborne Swarm remains refused");
        bad=swarm;bad.powerDurationMs++;check(!validShadowAction(bad,c,h),"Swarm exact native duration required");
        bad=swarm;bad.version=45;check(!validShadowAction(bad,c,h),"old peer cannot authorize Swarm semantics");
      }
      {PowerPresentation t;PowerPresentation::Sample sample{};float amount=0;
       check(t.push({100,1,24,320,333})&&t.push({140,1,24,27,333})&&t.sample(140,sample,amount)&&std::fabs(amount-27.f/333)<.00001f,"Swarm native looping clock reset remains same semantic phase");
       check(!t.push({150,1,25,0,1600})&&t.push({150,2,25,0,1600}),"Swarm release requires a distinct event");
       check(!t.sample(351,sample,amount),"stale Swarm pose expires");}
      for(unsigned variant=19;''')
test='''                    {
                        auto d=cast;
                        d.str(strings,"models/powers/devouring_swarm/devouring_swarm.inventoryitem");d.str(strings+0x100,"components/characters/player/base/powers/devouring_swarm/power.cpntpowerdevouringswarm");
                        d.q(pc,base+0x1b51f28);d.q(ped,base+0x1b51de8);d.d(instance+0x18,2);d.q(bindings+0x28,actor);
                        constexpr auto use=inv+0x7b000;d.d(machine+0xb0,2);d.d(active+4,63);d.q(mode+0x10+63*8,use);d.q(use,base+0x1c80ed8);d.d(use+8,63);d.q(use+0x10,machine);
                        const auto inspect=[&](Memory& m){const auto rd=[&](auto a,auto b,auto n){return m.read(a,b,n);};NativePlayerStates st{};return readNativePlayerStates(rd,base,st)&&readNativePlayerSwarmScene(rd,base,st,observedCast);};
                        const char* parts[]={"in","loop","out"};const unsigned durations[]={400,333,1600};
                        for(unsigned k=0;k<3;++k){
                            d.str(texts+0x200,(std::string("midnight/gameplay/player/base/powers/power_swarm_cast_")+parts[k]+".midnightscene").c_str());d.d(scene+0x28,durations[k]);d.d(instance+0x160,80);
                            require(inspect(d)&&observedCast.variant==23+k&&observedCast.arm.character==1,"Swarm phase requires exact duration, Corvo body and owned left arm");
                            auto bad=d;bad.d(scene+0x28,durations[k]+1);require(!inspect(bad),"Swarm wrong duration refused");
                            bad=d;bad.q(bindings+0x28,leftArm);require(!inspect(bad),"Swarm duplicate arm cannot replace body");
                            bad=d;bad.d(leftArm+0xac,2);require(!inspect(bad),"Swarm foreign arm refused");
                            bad=d;bad.d(instance+0x18,1);require(!inspect(bad),"Swarm requires two bindings");
                            bad=d;bad.q(use,base+0x1c3eeb8);require(!inspect(bad),"Swarm requires exact UsePower63 class");
                            bad=d;bad.d(instance+0x27c,5);require(!inspect(bad),"retired Swarm cannot continue posing");
                            bad=d;bad.str(texts+0x300,"Scene_1_unknown");require(!inspect(bad),"Swarm subscene prefix is not authority");
                            bad=d;bad.d(physics+0x50,1);require(!inspect(bad),"unobserved crouched Swarm refused");
                            bad=d;bad.q(inv+0x68,0);bad.d(cpnt+0x1c,0x20000002);bad.q(frame+0x3c8,actor);bad.q(frame+0x3c0,names+0x200);bad.d(frame+0x3d0,2);require(!inspect(bad),"unobserved holstered Swarm refused");
                            bad=d;bad.d(machine+0xb0,1);require(inspect(bad)==(k==2),"only observed Swarm release may recover on Running0 alone");
                            bad=d;bad.d(instance+0x160,durations[k]+1);require(inspect(bad)==(k==1),"only native Swarm loop can exceed its cycle clock");
                            bad=d;NativePlayerStates st{};const auto rd=[&](auto a,auto b,auto n){return bad.read(a,b,n);};require(readNativePlayerStates(rd,base,st),"Swarm race fixture state valid");unsigned reads=0;
                            require(!readNativePlayerSwarmScene([&](auto a,auto b,auto n){const bool ok=bad.read(a,b,n);if(a==instance+0x158&&++reads==1)bad.d(a,220);return ok;},base,st,observedCast),"recycled Swarm scene incarnation refused");
                        }
                        std::cout<<"PASS: Swarm exact native phases, body/arm ownership, equipment, state and concurrent retirement guards\\n";
                    }
'''
edit('world_identity_tests.cpp','                    {\n                        auto d=cast;',test+'                    {\n                        auto d=cast;')
print('SWARM_BINDING_SOURCE_READY')

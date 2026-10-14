from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';N=R/'work/natural-moveset'
def rd(p):return p.read_text(encoding='utf-8-sig')
def wr(p,s):p.write_text(s,encoding='utf-8')
def one(s,a,b):
 assert s.count(a)==1,(a,s.count(a))
 return s.replace(a,b)
s=rd(S/'native_player_swarm_scene.h').replace('Swarm','Doppelganger').replace('swarm','doppelganger').replace('power_doppelganger_cast','power_decoy_cast')
a=s.index('// v266');b=s.index('inline unsigned',a)
s=s[:a]+'''// v406b exact native Emily standing armed cast: one reciprocal left-arm
// binding, Running0 with UsePower63, then release recovery on Running0.
// Source input alone owns clone and effects; this observer never invokes events.
'''+s[b:]
s=s.replace('duration==400)return 23','duration==466)return 29').replace('duration==333)return 24','duration==1200)return 30').replace('duration==1600)return 25','duration==666)return 31')
s=s.replace('states.player.current>1','states.player.current!=0').replace('!item.instantiated||!item.started||','!item.instantiated||!item.started||!item.player.sword.equipped||')
s=one(s,'if((states.player.current==0&&a.id==0&&a.vtable==base+0x1c85628)||(states.player.current==1&&a.id==6&&a.vtable==base+0x1c84258))++ground;','if(a.id==0&&a.vtable==base+0x1c85628)++ground;')
a=s.index('    // v266 source body');b=s.index('    if(!s.instance',a)
s=s[:a]+'    // Only the observed release may continue after UsePower retires.\n'+s[b:]
s=s.replace('s.variant!=25','s.variant!=31').replace('s.variant!=24','s.variant!=30').replace('(bindingFlags&0xffffff)!=2','(bindingFlags&0xffffff)!=1')
s=s.replace('std::array<std::uintptr_t,2> actors{};unsigned armCount=0,bodyCount=0;','std::array<std::uintptr_t,1> actors{};unsigned armCount=0;')
s=s.replace('for(unsigned i=0;i<2;++i){unsigned handle=0;','for(unsigned i=0;i<1;++i){unsigned handle=0;')
s=one(s,'        if(actors[i]==states.player.player){if(++bodyCount>1||handle!=1)return false;}\n        else if(', '        if(')
s=s.replace('s.arm.character!=1','s.arm.character!=2').replace('if(armCount!=1||bodyCount!=1)','if(armCount!=1)')
s=one(s,'    for(unsigned i=0;i<2;++i){\n        if(!read(bindings+i*0x20+8,&again,8)||again!=actors[i])return false;','    if(!read(bindings+8,&again,8)||again!=actors[0])return false;\n    for(unsigned i=0;i<2;++i){')
wr(S/'native_player_doppelganger_scene.h',s)
p=S/'shadow_stream.h';s=rd(p);s=one(s,'#include "native_player_swarm_scene.h"','#include "native_player_swarm_scene.h"\n#include "native_player_doppelganger_scene.h"')
s=one(s,'||readNativePlayerSwarmScene(memoryRead,base,states,power)','||readNativePlayerDoppelgangerScene(memoryRead,base,states,power)||readNativePlayerSwarmScene(memoryRead,base,states,power)');wr(p,s)
p=S/'runtime_probe.cpp';s=rd(p);s=one(s,'#include "native_player_swarm_scene.h"','#include "native_player_swarm_scene.h"\n#include "native_player_doppelganger_scene.h"')
s=one(s,'&&std::strcmp(argv[2],"--watch-swarm-scenes")','&&std::strcmp(argv[2],"--watch-doppelganger-scenes")&&std::strcmp(argv[2],"--watch-swarm-scenes")')
a=s.index('    if(argc==4&&!std::strcmp(argv[2],"--watch-swarm-scenes"))');b=s.index('\n    }',a)+6
s=s[:b]+'\n'+s[a:b].replace('Swarm','Doppelganger').replace('swarm','doppelganger')+s[b:];wr(p,s)
p=S/'power_presentation.h';s=rd(p).replace('333,200,400};return v<=28','333,200,400,466,1200,666};return v<=31').replace('||v==27;','||v==27||v==30;').replace('s.variant<=28','s.variant<=31');wr(p,s)
p=S/'shadow_action_protocol.h';s=rd(p).replace('version=51','version=52').replace('p.version==51','p.version==52')
s=one(s,'    if(variant==28)return swordState==1;','    if(variant==28)return swordState==1;\n    if(variant>=29&&variant<=31)return swordState==2;')
s=one(s,'||(p.powerVariant>=16&&p.powerVariant<=22)', '||(p.powerVariant>=16&&p.powerVariant<=22)||(p.powerVariant>=29&&p.powerVariant<=31)');wr(p,s)
p=S/'movement_bank.h';s=rd(p).replace('32+319*','32+322*').replace('318,319};return v<=54','318,319,322};return v<=55').replace('v<=54;++v','v<=55;++v')
s=one(s,'    bool hasPossessionCrouchRelease()', '    bool hasDoppelganger()const{return version>=55;}\n    Joint sampleDoppelganger(unsigned variant,unsigned joint,float phase)const{return sampleParameter(319+variant-29,joint,phase);}\n    bool hasPossessionCrouchRelease()');wr(p,s)
p=S/'movement_playback.h';s=rd(p)
s=one(s,'        const bool powerActive=','        const bool doppelgangerActive=power.variant>=29&&power.variant<=31&&p.bank.hasDoppelganger()&&ground&&p.presentedDrawn;\n        const bool powerActive=')
s=one(s,'(possessionActive||swarmActive||dominoActive','(doppelgangerActive||possessionActive||swarmActive||dominoActive')
s=one(s,'||(p.powerVariant==28&&p.bank.hasPossessionRelease()))','||(p.powerVariant==28&&p.bank.hasPossessionRelease())||(p.powerVariant>=29&&p.powerVariant<=31&&p.bank.hasDoppelganger()))')
s=one(s,'auto power=p.powerVariant>=26?', 'auto power=p.powerVariant>=29?p.bank.sampleDoppelganger(p.powerVariant,j,p.powerAmount):p.powerVariant>=26?');wr(p,s)
p=S/'world_identity_tests.cpp';s=rd(p);s=one(s,'#include "native_player_swarm_scene.h"','#include "native_player_swarm_scene.h"\n#include "native_player_doppelganger_scene.h"')
a=s.index('                    {\n                        auto d=cast;\n                        d.str(strings,"models/powers/devouring_swarm');b=s.index('                    {\n                        auto d=cast;',a+30)
block=s[a:b].replace('Swarm','Doppelganger').replace('swarm','doppelganger').replace('Corvo','Emily').replace('devouring_doppelganger/devouring_doppelganger.inventoryitem','doppelganger/doppelganger.inventoryitem').replace('devouring_doppelganger/power.cpntpowerdevouringdoppelganger','doppelganger/doppelganger_power.cpntpowerdoppelganger').replace('power_doppelganger_cast','power_decoy_cast').replace('0x1b51f28','0x1b52258').replace('0x1b51de8','0x1b52118')
block=one(block,'                        auto d=cast;','                        auto d=cast;\n                        d.str(names+0x400,"models/characters/player/emily/body.def");d.str(names+0x600,"models/characters/player/emily/right_arm.def");d.str(texts+0x100,"models/characters/player/emily/left_arm.def");')
block=block.replace('d.d(instance+0x18,2);d.q(bindings+0x28,actor);','d.d(instance+0x18,1);')
block=block.replace('durations[]={400,333,1600}','durations[]={466,1200,666}').replace('observedCast.variant==23+k','observedCast.variant==29+k').replace('observedCast.arm.character==1','observedCast.arm.character==2')
c=block.index('                            for(unsigned low=0;');e=block.index('                            auto bad=d;',c)
block=block[:c]+block[e:]
block=block.replace('bad.q(bindings+0x28,leftArm);','bad.q(bindings+8,actor);').replace('duplicate arm cannot replace body','body cannot impersonate casting arm').replace('bad.d(instance+0x18,1);','bad.d(instance+0x18,2);').replace('requires two bindings','requires one binding')
block=block.replace('require(inspect(bad)&&!observedCast.power.player.sword.equipped,"v273 native holstered Doppelganger preserves exact ownership")','require(!inspect(bad),"unobserved holstered Doppelganger remains refused")')
s=s[:b]+block+s[b:];wr(p,s)
p=S/'shadow_action_protocol_tests.cpp';s=rd(p)
a='      check(validShadowAction(emily,ec,eh),"native Emily Far Reach pull phase packet");'
block='''
      for(unsigned v=29;v<=31;++v){
        auto cast=emily;cast.powerVariant=v;cast.powerDurationMs=PowerPresentation::duration(v);
        check(validShadowAction(cast,ec,eh),"observed Emily Doppelganger exact phase accepted");
        auto bad=cast;bad.role=0;bad.character=1;check(!validShadowAction(bad,c,h),"Corvo cannot claim campaign Emily Doppelganger");
        bad=cast;bad.swordState=1;check(!validShadowAction(bad,ec,eh),"unobserved empty-hand Doppelganger refused");
        bad=cast;bad.airStage=2;check(!validShadowAction(bad,ec,eh),"unobserved airborne Doppelganger refused");
        bad=cast;bad.powerDurationMs++;check(!validShadowAction(bad,ec,eh),"Doppelganger exact native timing required");
        bad=cast;bad.version=51;check(!validShadowAction(bad,ec,eh),"pre-Doppelganger protocol refused");
      }
      {PowerPresentation clock;PowerPresentation::Sample sample{};float amount=0;
        check(clock.push({100,1,30,1190,1200})&&clock.push({140,1,30,20,1200})&&clock.sample(140,sample,amount)&&std::fabs(amount-20.f/1200)<.00001f,"native Doppelganger hold reset stays within same event");
        check(!clock.push({150,1,31,0,666})&&clock.push({150,2,31,0,666}),"Doppelganger release requires distinct native event");
        check(!clock.sample(351,sample,amount),"stale Doppelganger presentation retires");}
'''
s=one(s,a,a+block);wr(p,s)
p=S/'movement_bank_tests.cpp';s=rd(p).replace('argc==1?1:54);format<=54','argc==1?1:55);format<=55')
a='        check(bank.hasPossessionCrouchRelease()'
block='''        check(bank.hasDoppelganger()==(bank.version>=55),"Doppelganger requires bank55");
        if(bank.hasDoppelganger())for(unsigned v=29;v<=31;++v)for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float t:{0.f,.5f,1.f}){
            const auto a=bank.sampleDoppelganger(v,j,t),b=bank.sampleParameter(319+v-29,j,t);
            for(unsigned k=0;k<7;++k)check(std::isfinite(a[k])&&std::fabs(a[k]-b[k])<1e-6f,"Doppelganger uses its own complete body and attachment pose");
        }
'''
assert s.count(a)==1;s=s.replace(a,block+a);wr(p,s)
s=rd(W/'Build-DoppelgangerFixture-v405.ps1').replace('doppelganger-fixture','doppelganger-binding').replace('v405','v407').replace('Build405','Build407').replace('possession-crouch-release-banks-v371','doppelganger-banks-v407').replace('possession-crouch-release-bank.bin','doppelganger-bank.bin').replace('actual319','actual322')
wr(W/'Build-DoppelgangerBinding-v407.ps1',s)
s=rd(N/'build_swarm_bank_v267.py').replace('Swarm','Doppelganger').replace('swarm','doppelganger').replace('v267','v407')
s=s.replace("domino-context-banks-v243","possession-crouch-release-banks-v371").replace('domino-context-bank.bin','possession-crouch-release-bank.bin').replace("b'D2MOVE48' and version==48 and count==301","b'D2MOVE54' and version==54 and count==319").replace("b'D2MOVE49',49,n,304","b'D2MOVE55',55,n,322").replace("[('in',400),('loop',333),('out',1600)]","[('in',466),('loop',1200),('out',666)]").replace('power_devouring_doppelganger_fire_','power_doppelganger_fire_').replace('301+offset','319+offset').replace('23+offset','29+offset').replace('doppelganger-native-standing-observe-v266','native-doppelganger-source-v406b').replace('power_doppelganger_cast','power_decoy_cast').replace("'D2MOVE49',clips=304","'D2MOVE55',clips=322").replace("native_character='corvo'","native_character='emily'").replace('Standing armed Corvo candidate','Standing armed Emily candidate').replace('Emily is offline skeleton comparison only','Corvo is offline skeleton comparison only')
wr(N/'build_doppelganger_bank_v407.py',s)
print('Doppelganger standing armed binding407 source and bank generator prepared')

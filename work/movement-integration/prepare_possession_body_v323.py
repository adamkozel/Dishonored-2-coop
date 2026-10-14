from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';W=R/'work/movement-integration';P=R/'work/natural-moveset'
def replace(path,old,new):
 s=path.read_text();assert s.count(old)==1,(path,old,s.count(old));path.write_text(s.replace(old,new))
p=S/'power_presentation.h';s=p.read_text();s=s.replace('400,333,1600};return v<=25','400,333,1600,333,200};return v<=27').replace('||v==24;','||v==24||v==27;').replace('s.variant<=25','s.variant<=27');p.write_text(s)
p=S/'shadow_action_protocol.h';s=p.read_text().replace('version=47,','version=48,').replace('p.version==47','p.version==48')
old='return variant>=1&&variant<=25&&variant!=13&&(swordState==2||(swordState==1&&variant>=10&&variant<=25));';assert old in s
s=s.replace(old,'return variant>=1&&variant<=27&&variant!=13&&(swordState==2||(swordState==1&&variant>=10&&variant<=25));')
s=s.replace('(p.powerVariant>=23&&p.powerVariant<=25)', '(p.powerVariant>=23&&p.powerVariant<=27)');p.write_text(s)
p=S/'shadow_stream.h';s=p.read_text().replace('#include "native_player_swarm_scene.h"','#include "native_player_swarm_scene.h"\n#include "native_player_possession_scene.h"')
old='||readNativePlayerSwarmScene(memoryRead,base,states,power))';assert old in s;s=s.replace(old,'||readNativePlayerSwarmScene(memoryRead,base,states,power)||readNativePlayerPossessionScene(memoryRead,base,states,power))');p.write_text(s)
p=S/'movement_bank.h';s=p.read_text().replace('32+313*','32+315*').replace('304,313};return v<=50','304,313,315};return v<=51').replace('for(unsigned v=1;v<=50;++v)','for(unsigned v=1;v<=51;++v)')
s=s.replace('    bool hasSwarm()const', '    bool hasPossession()const{return version>=51;}\n    bool hasSwarm()const');p.write_text(s)
p=S/'movement_playback.h';s=p.read_text();s=s.replace('        const bool powerActive=powerValid', '        const bool possessionActive=power.variant>=26&&power.variant<=27&&p.bank.hasPossession()&&ground&&p.presentedDrawn;\n        const bool powerActive=powerValid')
s=s.replace('(swarmActive||dominoActive||','(possessionActive||swarmActive||dominoActive||')
old='||(p.powerVariant>=23&&p.powerVariant<=25&&p.bank.hasSwarm()))';assert old in s
s=s.replace(old,'||(p.powerVariant>=23&&p.powerVariant<=25&&p.bank.hasSwarm())||(p.powerVariant>=26&&p.powerVariant<=27&&p.bank.hasPossession()))')
s=s.replace('auto power=p.powerVariant>=23?', 'auto power=p.powerVariant>=26?p.bank.sampleParameter(287+p.powerVariant,j,p.powerAmount):p.powerVariant>=23?');p.write_text(s)
p=S/'shadow_action_protocol_tests.cpp';s=p.read_text().replace('oldDark.powerVariant=26;', 'oldDark.powerVariant=28;')
anchor='      for(unsigned variant=23;variant<=25;++variant){';assert anchor in s
s=s.replace(anchor,'''      for(unsigned variant=26;variant<=27;++variant){
        auto possession=power;possession.powerVariant=variant;possession.powerDurationMs=PowerPresentation::duration(variant);
        check(validShadowAction(possession,c,h)&&shadowPowerSwordContext(variant,2),"observed standing armed Corvo Possession entry/aim accepted");
        auto bad=possession;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Emily cannot claim Possession");
        bad=possession;bad.swordState=1;check(!validShadowAction(bad,c,h)&&!shadowPowerSwordContext(variant,1),"unobserved holstered Possession refused");
        bad=possession;bad.airStage=2;check(!validShadowAction(bad,c,h),"unobserved airborne Possession refused");
        bad=possession;bad.powerDurationMs++;check(!validShadowAction(bad,c,h),"Possession requires exact observed native duration");
        bad=possession;bad.version=47;check(!validShadowAction(bad,c,h),"older peer cannot authorize Possession");
        bad=possession;bad.powerVariant=28;bad.powerDurationMs=0;check(!validShadowAction(bad,c,h),"unobserved successful-target release remains refused");
      }
      {PowerPresentation t;PowerPresentation::Sample sample{};float amount=0;
       check(t.push({100,1,27,80,200})&&t.push({140,1,27,120,200})&&t.sample(120,sample,amount)&&std::fabs(amount-.5f)<.00001f,"Possession aim samples native elapsed time");
       check(t.push({160,2,0,0,0})&&t.sample(160,sample,amount)&&sample.variant==0,"empty-target cancellation follows native phase retirement");
       check(!t.sample(361,sample,amount),"stale Possession state cannot hold remote pose");}
'''+anchor);p.write_text(s)
p=P/'build_possession_bank_v323.py';assert not p.exists()
s=(P/'build_swarm_bank_v267.py').read_text().replace('Swarm','Possession').replace('swarm','possession').replace('v267','v323').replace('power_devouring_possession_fire_','power_possession_fire_')
s=s.replace("P/'domino-context-banks-v243'", "P/'swarm-context-banks-v272'").replace("f'{role}-domino-context-bank.bin'", "f'{role}-swarm-context-bank.bin'")
s=s.replace("b'D2MOVE48' and version==48 and count==301", "b'D2MOVE50' and version==50 and count==313")
s=s.replace("b'D2MOVE49',49,n,304", "b'D2MOVE51',51,n,315").replace("[('in',400),('loop',333),('out',1600)]", "[('in',333),('loop',200)]")
s=s.replace('301+offset','313+offset').replace('23+offset','26+offset').replace("'possession-native-standing-observe-v266'", "'possession-empty-v322'")
s=s.replace("format='D2MOVE49',clips=304", "format='D2MOVE51',clips=315").replace('Native Possession phases from full-body studies','Observed Possession entry/aim from full-body studies')
assert 'devouring'not in s and 'D2MOVE48'not in s and '301+'not in s
p.write_text(s)
print('Prepared323 armed grounded Possession entry/aim protocol48, bank51/315 and targeted wire/retirement tests; native integration still pending')

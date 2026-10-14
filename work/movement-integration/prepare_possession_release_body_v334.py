from pathlib import Path
R=Path(__file__).resolve().parents[2];S=R/'outputs/d2-coop-lab/src';P=R/'work/natural-moveset'
def change(name,old,new):
 p=S/name;s=p.read_text();assert s.count(old)==1,(name,old,s.count(old));p.write_text(s.replace(old,new),encoding='utf-8')
change('power_presentation.h','1600,333,200};return v<=27','1600,333,200,400};return v<=28')
change('power_presentation.h','s.variant<=27','s.variant<=28')
change('shadow_action_protocol.h','version=48,','version=49,')
change('shadow_action_protocol.h','p.version==48','p.version==49')
change('shadow_action_protocol.h','return variant>=1&&variant<=27&&variant!=13&&(swordState==2||(swordState==1&&variant>=10&&variant<=25));','// Native successful host entry holsters the sword before release400ms.\n    if(variant==28)return swordState==1;\n    return variant>=1&&variant<=27&&variant!=13&&(swordState==2||(swordState==1&&variant>=10&&variant<=25));')
change('shadow_action_protocol.h','p.powerVariant>=23&&p.powerVariant<=27','p.powerVariant>=23&&p.powerVariant<=28')
change('shadow_stream.h','readNativePlayerPossessionScene(memoryRead,base,states,power)','readNativePlayerPossessionScene(memoryRead,base,states,power,true)')
change('movement_bank.h','maximumBytes=32+315*','maximumBytes=32+316*')
change('movement_bank.h','    bool hasPossession()','    bool hasPossessionRelease()const{return version>=52;}\n    bool hasPossession()')
change('movement_bank.h','304,313,315};return v<=51','304,313,315,316};return v<=52')
change('movement_bank.h','for(unsigned v=1;v<=51;++v)','for(unsigned v=1;v<=52;++v)')
change('movement_playback.h','const bool possessionActive=power.variant>=26&&power.variant<=27&&p.bank.hasPossession()&&ground&&p.presentedDrawn;','// Release has its own exact native context; it does not advertise\n        // generic ground locomotion while entering a possessed host.\n        const bool possessionActive=ground&&((power.variant>=26&&power.variant<=27&&p.bank.hasPossession()&&p.presentedDrawn)||\n            (power.variant==28&&p.bank.hasPossessionRelease()&&!p.presentedDrawn));')
change('movement_playback.h','||(p.powerVariant>=26&&p.powerVariant<=27&&p.bank.hasPossession()))','||(p.powerVariant>=26&&p.powerVariant<=27&&p.bank.hasPossession())||(p.powerVariant==28&&p.bank.hasPossessionRelease()))')
change('movement_bank_tests.cpp','for(unsigned format=(argc==1?1:51);format<=51;++format)','for(unsigned format=(argc==1?1:52);format<=52;++format)')
change('movement_bank_tests.cpp','        check(bank.hasPossession()==','        check(bank.hasPossessionRelease()==(bank.version>=52),"successful Possession release requires bank52");\n        if(bank.hasPossessionRelease()){\n            for(unsigned j:{2u,9u,16u,17u,20u,24u})for(float phase:{0.f,.5f,1.f})for(float x:bank.sampleParameter(315,j,phase))check(std::isfinite(x),"Possession release finite native skeleton");\n            if(argc==3)check(std::fabs(bank.clips[315].angle-.4f)<1e-6f,"Possession release matches400ms native scene");\n        }\n        check(bank.hasPossession()==')
change('shadow_action_protocol_tests.cpp','oldDark.powerVariant=28;','oldDark.powerVariant=29;')
change('shadow_action_protocol_tests.cpp','"unobserved successful-target release remains refused"','"release without native duration remains refused"')
change('shadow_action_protocol_tests.cpp','      for(unsigned variant=26;variant<=27;++variant){','''      {
        auto release=power;release.powerVariant=28;release.powerDurationMs=400;release.swordState=1;
        check(validShadowAction(release,c,h)&&shadowPowerSwordContext(28,1),"native acquired Corvo release requires holstered sword");
        auto bad=release;bad.swordState=2;check(!validShadowAction(bad,c,h)&&!shadowPowerSwordContext(28,2),"drawn-sword release refused");
        bad=release;bad.role=1;bad.character=2;check(!validShadowAction(bad,ec,eh),"Emily cannot send successful Possession release");
        bad=release;bad.airStage=2;check(!validShadowAction(bad,c,h),"airborne Possession release refused");
        bad=release;bad.powerDurationMs=399;check(!validShadowAction(bad,c,h),"release duration must match observed native400ms");
        bad=release;bad.version=48;check(!validShadowAction(bad,c,h),"older protocol cannot authorize release");
        PowerPresentation timeline;PowerPresentation::Sample sample{};float amount=0;
        check(timeline.push({100,1,28,100,400})&&timeline.push({150,1,28,150,400})&&timeline.sample(125,sample,amount)&&std::fabs(amount-.3125f)<.00001f,"release presentation follows native source clock");
        check(!timeline.sample(351,sample,amount),"stale release expires without inventing occupied host state");
      }
      for(unsigned variant=26;variant<=27;++variant){''')
# New body data is a finite, cosmetic release. Native root and outcomes are external.
s=(P/'build_possession_bank_v323.py').read_text()
s=s.replace('v323','v334').replace("P/'swarm-context-banks-v272'", "P/'possession-banks-v323'").replace("f'{role}-swarm-context-bank.bin'", "f'{role}-possession-bank.bin'")
s=s.replace("b'D2MOVE50' and version==50 and count==313", "b'D2MOVE51' and version==51 and count==315")
s=s.replace("b'D2MOVE51',51,n,315", "b'D2MOVE52',52,n,316").replace("[('in',333),('loop',200)]", "[('out',400)]")
s=s.replace('313+offset','315+offset').replace('26+offset','28+offset').replace('possession-empty-v322','possession-release-v333')
s=s.replace("format='D2MOVE51',clips=315", "format='D2MOVE52',clips=316")
s=s.replace('   for j in fingers:pose[j]=grip[j].copy()','   # Native entry already holsters the sword; retain authored open hand.').replace('   pose[17]=grip[17].copy();preview_helpers(rig,pose);poses.append(pose)','   pose[17]=rig.bind[17].copy();preview_helpers(rig,pose);poses.append(pose)')
s=s.replace('Standing armed Corvo candidate.', 'Corvo native holstered release candidate.').replace('and sword grip/socket retained', 'retained; authored empty hand with native sword socket')
s=s.replace('Observed Possession entry/aim', 'Observed successful Possession release').replace('SWARM_BANKS_GENERATED','POSSESSION_RELEASE_BANKS_GENERATED')
(P/'build_possession_bank_v334.py').write_text(s,encoding='utf-8')
print('Prepared protocol49 release and bank52/316; host occupation still expires existing stream, no host control or VFX')
